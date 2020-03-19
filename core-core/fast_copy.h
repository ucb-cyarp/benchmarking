#ifndef _H_FAST_COPY
#define _H_FAST_COPY

#include <cstring>
#include <immintrin.h>

#ifdef __AVX__
    #define FAST_COPY_ALIGNED_PADDING (32)
#elif defined (__SSE2__)
    #define FAST_COPY_ALIGNED_PADDING (16)
#else
    #define FAST_COPY_ALIGNED_PADDING (8)
#endif

// #define FAST_COPY_ALIGN_ON_SRC //If defined, fast_copy_unaligned aligns to source.  If not defined, fast_copy_unaligned aligns to the destination.

#ifdef FAST_COPY_ALIGN_ON_SRC
    #define FAST_COPY_ALIGN_CURSOR (srcCursor)
#else
    #define FAST_COPY_ALIGN_CURSOR (dstCursor)
#endif

//This version of fast copy forces the source and destination arrays to be aligned.  This allows SSE and AVX instructions
//which rely on aligned memory to be used.  This is accomplished by padding in the destination array.
//If the particular CPU in use does not suffer large performance penalties for unaligned access,
//fast_copy_unaligned may be a better option as it does not require padding the destination.
// Note: It is assumed that src and dst are aligned to the elementType
template <typename elementType>
inline elementType* fast_copy_aligned(elementType* src, elementType* dst, size_t len_elements, size_t dst_padding_elements){ //Length and dst_padding are in elements (not bytes)
    //Handle small cases
    if(len_elements == 0){
        return dst;
    }

    size_t len = len_elements*sizeof(elementType);
    size_t dst_padding = dst_padding_elements*sizeof(elementType);

    #ifdef __AVX__
        size_t align = 32; //Can use 256 bit load/store instructions

        //Do not need to step down alignment for unsufficient padding if already aligned
        //Any remaining blocks will be cleaned up at the end
        if(dst_padding < align-1 && ((size_t)((char*)src)) % 32 != 0){
            #ifdef __SSE__
                align = 16;
                if(dst_padding < align-1 && (((size_t)((char*)src)) % 16 != 0)){
                    align = 8;
                }
            #else
                align = 8;
            #endif
            
            //At this point, align is either 16 or 8.  The only way it
            //will be 16 is if the dist_padding >= 16

            if(dst_padding < align-1 && (((size_t)((char*)src)) % 8 != 0)){
                align = 4;
            }
            if(dst_padding < align-1 && (((size_t)((char*)src)) % 4 != 0)){
                align = 2;
            }
            if(dst_padding < align-1 && (((size_t)((char*)src)) % 2 != 0)){
                align = 1;
            }

        }
    #elif defined (__SSE2__)
        size_t align = 16; //Can use 128 bit load/store instructions

        if(dst_padding < align-1 && (((size_t)((char*)src)) % 16 != 0){
            align = 8;
        }
        if(dst_padding < align-1 && (((size_t)((char*)src)) % 8 != 0){
            align = 4;
        }
        if(dst_padding < align-1 && (((size_t)((char*)src)) % 4 != 0){
            align = 2;
        }
        if(dst_padding < align-1 && (((size_t)((char*)src)) % 2 != 0){
            align = 1;
        }
    #else
        size_t align = 8; //Can use 64 bit load/store instructions

        if(dst_padding < align-1 && (((size_t)((char*)src)) % 8 != 0){
            align = 4;
        }
        if(dst_padding < align-1 && (((size_t)((char*)src)) % 4 != 0){
            align = 2;
        }
        if(dst_padding < align-1 && (((size_t)((char*)src)) % 2 != 0){
            align = 1;
        }
    #endif

    //Want to align dst to src (with respect to the max allowed alignment - based on vector unit length and padding bytes)
    char* srcCursor = (char*) src;
    size_t srcCursorAlignment;
    size_t dstCursorInitAlignment;
    size_t dstOffset;

    //Attempting to avoid remainder operator use with variable
    switch(align){
        case 32:
            srcCursorAlignment = ((size_t) ((char*)src))%32;
            dstCursorInitAlignment = ((size_t) ((char*)dst))%32;
            dstOffset = ((32 + srcCursorAlignment) - dstCursorInitAlignment) % 32;
            break;
        case 16:
            srcCursorAlignment = ((size_t) ((char*)src))%16;
            dstCursorInitAlignment = ((size_t) ((char*)dst))%16;
            dstOffset = ((16 + srcCursorAlignment) - dstCursorInitAlignment) % 16;
            break;
        case 8:
            srcCursorAlignment = ((size_t) ((char*)src))%8;
            dstCursorInitAlignment = ((size_t) ((char*)dst))%8;
            dstOffset = ((8 + srcCursorAlignment) - dstCursorInitAlignment) % 8;
            break;
        case 4:
            srcCursorAlignment = ((size_t) ((char*)src))%4;
            dstCursorInitAlignment = ((size_t) ((char*)dst))%4;
            dstOffset = ((4 + srcCursorAlignment) - dstCursorInitAlignment) % 4;
            break;
        case 2:
            srcCursorAlignment = ((size_t) ((char*)src))%2;
            dstCursorInitAlignment = ((size_t) ((char*)dst))%2;
            dstOffset = ((2 + srcCursorAlignment) - dstCursorInitAlignment) % 2;
            break;
        case 1:
            dstOffset = 0;
            break;
        default:
            std::cerr << "Error!  Impossible Alignment" << std::endl;
            exit(1);
            break;
    }

    char* dstStart = ((char*) dst + dstOffset);
    char* dstCursor = dstStart;

    size_t bytesCopied = 0;

    //We can skip the initial ramp-up from byte transfers if we know the element sizes
    size_t bytesToTransfer = sizeof(elementType) > align ? align : sizeof(elementType); //Protect against large types (ex. structures)

    // printf("Align: %d, Bytes to Transfer: %d\n", align, bytesToTransfer);

    while(bytesCopied < len){
        switch(bytesToTransfer){
            case 1:
                //Impossible to have fewer byte to copy than 1
                if(align == 1 || len - bytesCopied < 2){ //The next level copies 2 bytes at a time
                    //Need to copy a byte because there is only 1 byte left to copy or the alignment only allows byte copies
                    *dstCursor = *srcCursor;
                    dstCursor += 1;
                    srcCursor += 1;
                    bytesCopied += 1;
                    // printf("Transfered Byte\n");

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 2 != 0){
                    //Need to copy a byte because of misalignmnet and more than 1 byte needs to be copied
                    *dstCursor = *srcCursor;
                    dstCursor += 1;
                    srcCursor += 1;
                    bytesCopied += 1;
                    // printf("Transfered Byte\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 2;
                }else{
                    //Is already aligned for the next level and has more than 1 block to write, step up the size and do not copy.
                    bytesToTransfer = 2;
                }
                break;
            case 2:
                if(len - bytesCopied < 2){
                    //There are fewer than 2 bytes to write, step down the size
                    bytesToTransfer = 1;
                }
                else if(align == 2 || len - bytesCopied < 4){ // The next level transfers 4 bytes
                    //Need to copy a 2 byte word because there is less than a 4 byte  copy or the alignment only allows 2 byte copies
                    *((int16_t*) dstCursor) = *((int16_t*) srcCursor);
                    dstCursor += 2;
                    srcCursor += 2;
                    bytesCopied += 2;
                    // printf("Transfered 2 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 4 != 0){
                    //Need to copy a 2 byte word because of misalignmnet and more than 2 bytes need to be copied
                    *((int16_t*) dstCursor) = *((int16_t*) srcCursor);
                    dstCursor += 2;
                    srcCursor += 2;
                    bytesCopied += 2;
                    // printf("Transfered 2 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 4;
                }else{
                    //Is already aligned for the next level and has more than 2 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 4;
                }
                break;
            case 4:
                if(len - bytesCopied < 4){
                    //There are fewer than 4 bytes to write, step down the size
                    bytesToTransfer = 2;
                }
                else if(align == 4 || len - bytesCopied < 8){ // The next level transfers 8 bytes
                    //Need to copy a 4 byte word because there is less than an 8 byte copy or the alignment only allows 4 byte copies
                    *((int32_t*) dstCursor) = *((int32_t*) srcCursor);
                    dstCursor += 4;
                    srcCursor += 4;
                    bytesCopied += 4;
                    // printf("Transfered 4 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 8 != 0){
                    //Need to copy a 4 byte word because of misalignmnet and more than 4 bytes need to be copied
                    *((int32_t*) dstCursor) = *((int32_t*) srcCursor);
                    dstCursor += 4;
                    srcCursor += 4;
                    bytesCopied += 4;
                    // printf("Transfered 4 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 8;
                }else{
                    //Is already aligned for the next level and has more than 4 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 8;
                }
                break;
            case 8:
                if(len - bytesCopied < 8){
                    //There are fewer than 8 bytes to write, step down the size
                    bytesToTransfer = 4;
                }
                else if(align == 8 || len - bytesCopied < 16){ // The next level transfers 16 bytes
                    //Need to copy a 8 byte word because there is less than an 16 byte copy or the alignment only allows 8 byte copies
                    *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
                    dstCursor += 8;
                    srcCursor += 8;
                    bytesCopied += 8;
                    // printf("Transfered 8 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 16 != 0){
                    //Need to copy a 4 byte word because of misalignmnet and more than 4 bytes need to be copied
                    *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
                    dstCursor += 8;
                    srcCursor += 8;
                    bytesCopied += 8;
                    // printf("Transfered 8 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 16;
                }else{
                    //Is already aligned for the next level and has more than 8 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 16;
                }
                break;
            case 16:
                if(len - bytesCopied < 16){
                    //There are fewer than 16 bytes to write, step down the size
                    bytesToTransfer = 8;
                }
                else if(align == 16 || len - bytesCopied < 32){ // The next level transfers 16 bytes
                    //Need to copy a 16 byte word because there is less than an 32 byte copy or the alignment only allows 16 byte copies
                    #ifdef __SSE2__
                        //double type is a dummy type to use the intrinsic
                        __m128d tmp = _mm_load_pd((double*) srcCursor);
                        _mm_store_pd((double*) dstCursor, tmp);
                    #else
                        //Should never happen
                        std::cerr << "Error!  Tried to use SSE2 instruction when not supported" << std::endl;
                        exit(1);
                    #endif
                    dstCursor += 16;
                    srcCursor += 16;
                    bytesCopied += 16;
                    // printf("Transfered 16 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 32 != 0){
                    //Need to copy a 16 byte word because of misalignmnet and more than 16 bytes need to be copied
                    #ifdef __SSE2__
                        //double type is a dummy type to use the intrinsic
                        __m128d tmp = _mm_load_pd((double*) srcCursor);
                        _mm_store_pd((double*) dstCursor, tmp);
                    #else
                        //Should never happen
                        std::cerr << "Error!  Tried to use SSE2 instruction when not supported" << std::endl;
                        exit(1);
                    #endif
                    dstCursor += 16;
                    srcCursor += 16;
                    bytesCopied += 16;
                    // printf("Transfered 16 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 32;
                }else{
                    //Is already aligned for the next level and has more than 8 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 32;
                }
                break;
            case 32:
                if(len - bytesCopied < 32){
                    //There are fewer than 8 bytes to write, step down the size
                    bytesToTransfer = 16;
                }
                else{ // This is the end of the line for now.  If AVX512 used, extend
                    #ifdef __AVX__
                        //double type is a dummy type to use the intrinsic
                        __m256d tmp = _mm256_load_pd((double*) srcCursor);
                        _mm256_store_pd((double*) dstCursor, tmp);
                    #else
                        //Should never happen
                        std::cerr << "Error!  Tried to use AVX instruction when not supported" << std::endl;
                        exit(1);
                    #endif
                    dstCursor += 32;
                    srcCursor += 32;
                    bytesCopied += 32;
                    // printf("Transfered 32 Bytes\n");

                    //Do not step up the size, this is the end of the line for now
                }
                break;
            default:
                //Should never happen
                std::cerr << "Error!  Impossible bytes to transfer" << std::endl;
                exit(1);
                break;
        }
    }

    //Use byte copy until able to use 16 bit copy (max 1 iteration, or repeat if alignment is 1)
    //Use 16 bit copy until able to use 32 bit copy (max 1 iteration, or repeat if alignment is 2)
    //Use 32 but copy until able to use 64 bit copy (max 1 iteration), or repeat if alignment is 4)

    //Use 64 bit copy until able to use 128 bit copy (max 1 iteration, or repeat if alignment is 8)
    //Use 128 bit copy until able to use 256 bit copy (max 1 iteration, or repeat if alignment is 16 - also check if SSE is present)
    //Use 256 bit copy 

    //Cleanup uses the same basic procedure except that the decision on when to drop down to a reduced width is
    //based on if the number of bytes left to copy is less than what the instruction accesses

    return (elementType*) dstStart;
}

//This version of fast copy allows for unaligned arrays and makes use of AVX and SSE instructions
//which are not restricted to aligned entries.  It alignes to either the source or destination array
// Note: It is assumed that src and dst are aligned to the elementType
template<typename elementType>
inline elementType* fast_copy_semialigned(elementType* src, elementType* dst, size_t len_elements){
    //It is still best if we can leverage alignment if possible.
    //Will copy in smaller increments until either the source or destination are aligned (controlled by a #define)
    //Will then use the largest transfer possible (with the version of the instruction that supports unaligned memory access)
    //We will then step down the transfer size if nessisary to transfer the tail of the data

    //Handle small cases
    if(len_elements == 0){
        return dst;
    }

    size_t len = len_elements*sizeof(elementType);

    #ifdef __AVX__
        size_t align = 32; //Can use 256 bit load/store instructions
    #elif defined (__SSE2__)
        size_t align = 16; //Can use 128 bit load/store instructions
    #else
        size_t align = 8; //Can use 64 bit load/store instructions
    #endif


    char* dstCursor = (char*) dst;
    char* srcCursor = (char*) src;

    size_t bytesCopied = 0;
    size_t bytesToTransfer = sizeof(elementType) > align ? align : sizeof(elementType); //Protect against large types (ex. structures)

    while(bytesCopied<len){
        switch(bytesToTransfer){
            case 1:
                //Impossible to have fewer byte to copy than 1
                if(len - bytesCopied < 2){ //The next level copies 2 bytes at a time
                    //Need to copy a byte because there is only 1 byte left to
                    *dstCursor = *srcCursor;
                    dstCursor += 1;
                    srcCursor += 1;
                    bytesCopied += 1;
                    // printf("Transfered Byte\n");

                    //Do not step up the size
                }else if(((size_t)FAST_COPY_ALIGN_CURSOR) % 2 != 0){
                    //Need to copy a byte because of misalignmnet and more than 1 byte needs to be copied
                    *dstCursor = *srcCursor;
                    dstCursor += 1;
                    srcCursor += 1;
                    bytesCopied += 1;
                    // printf("Transfered Byte\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 2;
                }else{
                    //Is already aligned for the next level and has more than 1 block to write, step up the size and do not copy.
                    bytesToTransfer = 2;
                }
                break;
            case 2:
                if(len - bytesCopied < 2){
                    //There are fewer than 2 bytes to write, step down the size
                    bytesToTransfer = 1;
                }
                else if(len - bytesCopied < 4){ // The next level transfers 4 bytes
                    //Need to copy a 2 byte word because there is less than a 4 byte copy
                    *((int16_t*) dstCursor) = *((int16_t*) srcCursor);
                    dstCursor += 2;
                    srcCursor += 2;
                    bytesCopied += 2;
                    // printf("Transfered 2 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)FAST_COPY_ALIGN_CURSOR) % 4 != 0){
                    //Need to copy a 2 byte word because of misalignmnet and more than 2 bytes need to be copied
                    *((int16_t*) dstCursor) = *((int16_t*) srcCursor);
                    dstCursor += 2;
                    srcCursor += 2;
                    bytesCopied += 2;
                    // printf("Transfered 2 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 4;
                }else{
                    //Is already aligned for the next level and has more than 2 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 4;
                }
                break;
            case 4:
                if(len - bytesCopied < 4){
                    //There are fewer than 4 bytes to write, step down the size
                    bytesToTransfer = 2;
                }
                else if(len - bytesCopied < 8){ // The next level transfers 8 bytes
                    //Need to copy a 2 byte word because there is less than a 8 byte copy
                    *((int32_t*) dstCursor) = *((int32_t*) srcCursor);
                    dstCursor += 4;
                    srcCursor += 4;
                    bytesCopied += 4;
                    // printf("Transfered 4 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)FAST_COPY_ALIGN_CURSOR) % 8 != 0){
                    //Need to copy a 4 byte word because of misalignmnet and more than 4 bytes need to be copied
                    *((int32_t*) dstCursor) = *((int32_t*) srcCursor);
                    dstCursor += 4;
                    srcCursor += 4;
                    bytesCopied += 4;
                    // printf("Transfered 4 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 8;
                }else{
                    //Is already aligned for the next level and has more than 2 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 8;
                }
                break;
            case 8:
                if(len - bytesCopied < 8){
                    //There are fewer than 4 bytes to write, step down the size
                    bytesToTransfer = 4;
                }
                else if(align == 8 || len - bytesCopied < 16){ // The next level transfers 16 bytes, cap out here if this is the max supported by the platform
                    //Need to copy a 2 byte word because there is less than a 8 byte copy
                    *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
                    dstCursor += 8;
                    srcCursor += 8;
                    bytesCopied += 8;
                    // printf("Transfered 8 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)FAST_COPY_ALIGN_CURSOR) % 16 != 0){
                    //Need to copy a 8 byte word because of misalignmnet and more than 8 bytes need to be copied
                    *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
                    dstCursor += 8;
                    srcCursor += 8;
                    bytesCopied += 8;
                    // printf("Transfered 8 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 16;
                }else{
                    //Is already aligned for the next level and has more than 2 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 16;
                }
                break;
            case 16:
                if(len - bytesCopied < 16){
                    //There are fewer than 16 bytes to write, step down the size
                    bytesToTransfer = 8;
                }
                else if(align == 16 || len - bytesCopied < 32){ // The next level transfers 16 bytes, cap out here if this is the max that the system supports
                    //Need to copy a 16 byte word because there is less than an 32 byte copy or the alignment only allows 16 byte copies
                    #ifdef __SSE2__
                        //double type is a dummy type to use the intrinsic
                        #ifdef FAST_COPY_ALIGN_ON_SRC
                            __m128d tmp = _mm_load_pd((double*) srcCursor); //load is aligned
                            _mm_storeu_pd((double*) dstCursor, tmp);
                        #else
                            __m128d tmp = _mm_loadu_pd((double*) srcCursor);
                            _mm_store_pd((double*) dstCursor, tmp); //store is aligned
                        #endif
                    #else
                        //Should never happen
                        std::cerr << "Error!  Tried to use SSE2 instruction when not supported" << std::endl;
                        exit(1);
                    #endif
                    dstCursor += 16;
                    srcCursor += 16;
                    bytesCopied += 16;
                    // printf("Transfered 16 Bytes\n");

                    //Do not step up the size
                }else if(((size_t)FAST_COPY_ALIGN_CURSOR) % 32 != 0){
                    //Need to copy a 16 byte word because of misalignmnet and more than 16 bytes need to be copied
                    #ifdef __SSE2__
                        //double type is a dummy type to use the intrinsic
                        #ifdef FAST_COPY_ALIGN_ON_SRC
                            __m128d tmp = _mm_load_pd((double*) srcCursor); //load is aligned
                            _mm_storeu_pd((double*) dstCursor, tmp);
                        #else
                            __m128d tmp = _mm_loadu_pd((double*) srcCursor);
                            _mm_store_pd((double*) dstCursor, tmp); //store is aligned
                        #endif
                    #else
                        //Should never happen
                        std::cerr << "Error!  Tried to use SSE2 instruction when not supported" << std::endl;
                        exit(1);
                    #endif
                    dstCursor += 16;
                    srcCursor += 16;
                    bytesCopied += 16;
                    // printf("Transfered 16 Bytes\n");

                    //Will now be alligned to next level, step up size.  If next size is too large, it will be corrected for in the next iteration
                    bytesToTransfer = 32;
                }else{
                    //Is already aligned for the next level and has more than 8 blocks to write, step up the size and do not copy.
                    bytesToTransfer = 32;
                }
                break;
            case 32:
                if(len - bytesCopied < 32){
                    //There are fewer than 8 bytes to write, step down the size
                    bytesToTransfer = 16;
                }
                else{ // This is the end of the line for now.  If AVX512 used, extend
                    #ifdef __AVX__
                        //double type is a dummy type to use the intrinsic
                        #ifdef FAST_COPY_ALIGN_ON_SRC
                            __m256d tmp = _mm256_load_pd((double*) srcCursor); //src is aligned
                            _mm256_storeu_pd((double*) dstCursor, tmp);
                        #else
                            __m256d tmp = _mm256_loadu_pd((double*) srcCursor);
                            _mm256_store_pd((double*) dstCursor, tmp); //dst is aligned
                        #endif
                    #else
                        //Should never happen
                        std::cerr << "Error!  Tried to use AVX instruction when not supported" << std::endl;
                        exit(1);
                    #endif
                    dstCursor += 32;
                    srcCursor += 32;
                    bytesCopied += 32;
                    // printf("Transfered 32 Bytes\n");

                    //Do not step up the size, this is the end of the line for now
                }
                break;
            default:
                //Should never happen
                std::cerr << "Error!  Impossible bytes to transfer" << std::endl;
                exit(1);
                break;
        }
    }

    return dst;
}

//This version of fast copy allows for unaligned arrays and makes use of AVX and SSE instructions
//which are not restricted to aligned entries.  It does not align to either the src or destination
// Note: It is assumed that src and dst are aligned to the elementType
//
//This version uses standard load operations then moves on to AVX or SSE instructions
template<typename elementType>
inline elementType* fast_copy_unaligned_ramp_in(elementType* src, elementType* dst, size_t len_elements){

    //For now, structures will not be supported
    //TODO: expand support for strucures.

    if(sizeof(elementType) != 8 && sizeof(elementType) != 4 && sizeof(elementType) != 2 && sizeof(elementType) != 1){
        std::cerr << "Error!  Fast Copy Unaligned Currently Does not Support Elements > 8 Bytes or not a power of 2" << std::endl;
        exit(1);
    }

    //Determine how many standard element copies need to be conducted and how many larger (and potentially unaligned copies) need to happen
    #ifdef __AVX__
        size_t elementsPerBlockTransfer = 32/sizeof(elementType);
    #elif defined (__SSE2__)
        size_t elementsPerBlockTransfer = 16/sizeof(elementType);
    #else
        size_t elementsPerBlockTransfer = 8/sizeof(elementType);
    #endif

    size_t blocksToCopy = len_elements/elementsPerBlockTransfer;
    size_t elementsToCopy = len_elements%elementsPerBlockTransfer;

    //Copy Elements
    for(size_t i = 0; i<elementsToCopy; i++){
        dst[i] = src[i];
    }

    elementType* srcBlock = src+elementsToCopy;
    elementType* dstBlock = dst+elementsToCopy;

    //Copy Large Blocks
    for(size_t i = 0; i<blocksToCopy; i++){
        elementType* srcCursor = srcBlock+i*elementsPerBlockTransfer;
        elementType* dstCursor = dstBlock+i*elementsPerBlockTransfer;
        #ifdef __AVX__
            //double type is a dummy type to use the intrinsic
            __m256d tmp = _mm256_loadu_pd((double*) srcCursor);
            _mm256_storeu_pd((double*) dstCursor, tmp);
        #elif defined (__SSE2__)
            __128d tmp = _mm128_loadu_pd((double*) srcCursor);
            _mm128_storeu_pd((double*) dstCursor, tmp);
        #else
            *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
        #endif
    }

    return dst;
}

//This version of fast copy allows for unaligned arrays and makes use of AVX and SSE instructions
//which are not restricted to aligned entries.  It does not align to either the src or destination
// Note: It is assumed that src and dst are aligned to the elementType
//
//This version starts out using AVX or SSE instructions then ramps out
template<typename elementType>
inline elementType* fast_copy_unaligned(elementType* src, elementType* dst, size_t len_elements){

    //For now, structures will not be supported
    //TODO: expand support for strucures.

    if(sizeof(elementType) != 8 && sizeof(elementType) != 4 && sizeof(elementType) != 2 && sizeof(elementType) != 1){
        std::cerr << "Error!  Fast Copy Unaligned Currently Does not Support Elements > 8 Bytes or not a power of 2" << std::endl;
        exit(1);
    }

    //Determine how many standard element copies need to be conducted and how many larger (and potentially unaligned copies) need to happen
    #ifdef __AVX__
        size_t elementsPerBlockTransfer = 32/sizeof(elementType);
    #elif defined (__SSE2__)
        size_t elementsPerBlockTransfer = 16/sizeof(elementType);
    #else
        size_t elementsPerBlockTransfer = 8/sizeof(elementType);
    #endif

    size_t blocksToCopy = len_elements/elementsPerBlockTransfer;
    size_t elementsToCopy = len_elements%elementsPerBlockTransfer;

    //Copy Large Blocks
    for(size_t i = 0; i<blocksToCopy; i++){
        elementType* srcCursor = src+i*elementsPerBlockTransfer;
        elementType* dstCursor = dst+i*elementsPerBlockTransfer;
        #ifdef __AVX__
            //double type is a dummy type to use the intrinsic
            __m256d tmp = _mm256_loadu_pd((double*) srcCursor);
            _mm256_storeu_pd((double*) dstCursor, tmp);
        #elif defined (__SSE2__)
            __128d tmp = _mm128_loadu_pd((double*) srcCursor);
            _mm128_storeu_pd((double*) dstCursor, tmp);
        #else
            *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
        #endif
    }

    elementType* srcElements = src+blocksToCopy*elementsPerBlockTransfer;
    elementType* dstElements = dst+blocksToCopy*elementsPerBlockTransfer;

    //Copy Elements
    for(size_t i = 0; i<elementsToCopy; i++){
        dstElements[i] = srcElements[i];
    }

    return dst;
}

//This version of fast copy relies on the source and destination arrays being aligned and the lengths being a multiple of the vector length
//
//This version starts out using AVX or SSE instructions then ramps out
template<typename elementType>
inline elementType* fast_copy_aligned_multiple(elementType* src, elementType* dst, size_t len_elements){

    //For now, structures will not be supported
    //TODO: expand support for strucures.

    if(sizeof(elementType) != 8 && sizeof(elementType) != 4 && sizeof(elementType) != 2 && sizeof(elementType) != 1){
        std::cerr << "Error!  Fast Copy Unaligned Currently Does not Support Elements > 8 Bytes or not a power of 2" << std::endl;
        exit(1);
    }

    //Determine how many standard element copies need to be conducted and how many larger (and potentially unaligned copies) need to happen
    #ifdef __AVX__
        size_t bytesPerTransfer = 32;
        size_t elementsPerBlockTransfer = 32/sizeof(elementType);
    #elif defined (__SSE2__)
        size_t bytesPerTransfer = 16;
        size_t elementsPerBlockTransfer = 16/sizeof(elementType);
    #else
        size_t bytesPerTransfer = 8;
        size_t elementsPerBlockTransfer = 8/sizeof(elementType);
    #endif

    if((sizeof(elementType)*len_elements)%bytesPerTransfer != 0){
        std::cerr << "Error! Fast Copy Unaligned Multiple Requires that the array length (" << sizeof(elementType)*len_elements << ") be a multiple of the vector unit lenth (" << bytesPerTransfer << ")" << std::endl;
        exit(1);
    }

    size_t blocksToCopy = len_elements/elementsPerBlockTransfer;

    //Copy Large Blocks
    for(size_t i = 0; i<blocksToCopy; i++){
        elementType* srcCursor = src+i*elementsPerBlockTransfer;
        elementType* dstCursor = dst+i*elementsPerBlockTransfer;
        #ifdef __AVX__
            //double type is a dummy type to use the intrinsic
            __m256d tmp = _mm256_load_pd((double*) srcCursor);
            _mm256_store_pd((double*) dstCursor, tmp);
        #elif defined (__SSE2__)
            __128d tmp = _mm128_load_pd((double*) srcCursor);
            _mm128_store_pd((double*) dstCursor, tmp);
        #else
            *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
        #endif
    }

    return dst;
}

#endif