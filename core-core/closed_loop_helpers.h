#ifndef _CLOSED_LOOP_HELPERS_H
#define _CLOSED_LOOP_HELPERS_H

#include "intrin_bench_default_defines.h"
#include "results.h"
#include "mallocHelpers.h"
#include "reporting_helpers.h"
#include "open_loop_helpers.h"
#include "fifoless_helpers.h"
#include "sir.h"

#include <vector>
#include <atomic>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <immintrin.h>

// #define CLOSED_LOOP_DISABLE_INTERRUPTS 1

//src should be aligned to 
inline void* fast_copy(void* src, void* dst, size_t len, size_t dst_padding){
    //Handle small cases
    if(len == 0){
        return dst;
    }

    #ifdef __AVX__
        size_t align = 32; //Can use 256 bit load/store instructions

        if(dst_padding < align-1){
            #ifdef __SSE__
                align = 16;
                if(dst_padding < align-1){
                    align = 8;
                }
            #else
                align = 8;
            #endif
            
            //At this point, align is either 16 or 8.  The only way it
            //will be 16 is if the dist_padding >= 16

            if(dst_padding < align-1){
                align = 4;
            }
            if(dst_padding < align-1){
                align = 2;
            }
            if(dst_padding < align-1){
                align = 1;
            }

        }
    #elif defined (__SSE2__)
        size_t align = 16; //Can use 128 bit load/store instructions

        if(dst_padding < align-1){
            align = 8;
        }
        if(dst_padding < align-1){
            align = 4;
        }
        if(dst_padding < align-1){
            align = 2;
        }
        if(dst_padding < align-1){
            align = 1;
        }
    #else
        size_t align = 8; //Can use 64 bit load/store instructions

        if(dst_padding < align-1){
            align = 4;
        }
        if(dst_padding < align-1){
            align = 2;
        }
        if(dst_padding < align-1){
            align = 1;
        }
    #endif

    //Want to align dst to src (with respect to the max allowed alignment - based on vector unit length and padding bytes)
    char* srcCursor = (char*) src;
    size_t srcCursorAlignment = ((size_t)src)%align;
    size_t dstCursorInitAlignment = ((size_t)dst)%align;
    size_t dstOffset = (srcCursorAlignment - dstCursorInitAlignment + align) % align;
    char* dstStart = ((char*) dst + dstOffset);
    char* dstCursor = dstStart;

    size_t bytesCopied = 0;
    size_t bytesToTransfer = 1;

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

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 2 != 0){
                    //Need to copy a byte because of misalignmnet and more than 1 byte needs to be copied
                    *dstCursor = *srcCursor;
                    dstCursor += 1;
                    srcCursor += 1;
                    bytesCopied += 1;

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

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 4 != 0){
                    //Need to copy a 2 byte word because of misalignmnet and more than 2 bytes need to be copied
                    *((int16_t*) dstCursor) = *((int16_t*) srcCursor);
                    dstCursor += 2;
                    srcCursor += 2;
                    bytesCopied += 2;

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

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 8 != 0){
                    //Need to copy a 4 byte word because of misalignmnet and more than 4 bytes need to be copied
                    *((int32_t*) dstCursor) = *((int32_t*) srcCursor);
                    dstCursor += 4;
                    srcCursor += 4;
                    bytesCopied += 4;

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

                    //Do not step up the size
                }else if(((size_t)srcCursor) % 16 != 0){
                    //Need to copy a 4 byte word because of misalignmnet and more than 4 bytes need to be copied
                    *((int64_t*) dstCursor) = *((int64_t*) srcCursor);
                    dstCursor += 8;
                    srcCursor += 8;
                    bytesCopied += 8;

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

    return (void*) dstStart;
}

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClient,
         typename nopsLocal> //Note, this can be changed to a float for PI controllers
class ClosedLoopBufferArgs : public FifolessConfig{
public:
    indexType *read_offset_ptr;
    indexType *write_offset_ptr;
    void *array;
    std::atomic_flag *start_flag; //Used by the client to signal to the server that it should start writing
    std::atomic_flag *stop_flag; //Used by the client to signal to the server that an error occured and that it should stop
    std::atomic_flag *ready_flag; //Used by the server to signal to the client that it is ready to begin
    int array_length; //In Blocks  //Note: The FIFO Array has an additional block allocated to resolve the Empty/Full Ambiguity
    int64_t max_block_transfers; //The maximum number of blocks to transfer in this test
    int blockSize; //The block size (in elementType elements)
    nopsClient *clientNops; //A pointer to a shared integer which controls how many nops per loop itteration the client uses
    int32_t control_check_period; //The number of itterations of the main loop between control checks on the controller side
    int32_t control_client_check_period; //The number of itterations of the main loop before the client checks for a new control signal
    int alignment; //The alignment in bytes of the components within the block (the IDs and the Buffer)
    int core_client; //The core the client is executing on
    int core_server; //The core the server is executing on
    nopsLocal initialNops; //The initial NOPs for the client and server
    FILE* writerSirFile; //The file pointer to the sir driver for the writer (used for disabling interrupts)
    FILE* readerSirFile; //The file pointer to the sir driver for the reader (used for disabling interrupts)

protected:
    virtual void printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file) = 0;
};

//See ==== Entries Available to read ==== In Lockless Thread Crossing FIFO (coppied here for convenience):
/*
 * ==== Entries Available to read ====
 * % 1 2 3 _ _ _ ((readOffset < writeOffset) : writeOffset - readOffset - 1 = 3)
 * r       w
 *
 * 7 _ _ % 4 5 6 ((readOffset >= writeOffset) : arrayLength - (readOffset - writeOffset + 1) = 4) | arrayLength includes the extra entry
 *   w   r
 *
 * 7 8 9 % 4 5 6 ((readOffset >= writeOffset) : arrayLength - (readOffset - writeOffset + 1) = 6)
 *       r
 *       w
 *
 * _ % _ _ _ _ _ ((readOffset < writeOffset) : writeOffset - readOffset - 1 = 0)
 *   r w
 *
 * _ _ _ _ _ _ _ ((readOffset >= writeOffset) : arrayLength - (readOffset - writeOffset + 1) = 0)
 * w           r
 *
 * Avail to read: (readOffset < writeOffset) ? writeOffset - readOffset - 1 : arrayLength - (readOffset - writeOffset + 1)
 * Avail to read: (readOffset < writeOffset) ? writeOffset - readOffset - 1 : arrayLength - readOffset + writeOffset - 1
 */

template<typename T>
inline T getNumItemsInBuffer(T readOffset, T writeOffset, T fifoLength){
    int arrayLength = fifoLength+1;

    return (readOffset < writeOffset) ? writeOffset - readOffset - 1 : arrayLength - readOffset + writeOffset - 1;
}

class ClosedLoopServerEndCondition : public BenchmarkSpecificResult{
public:
    int64_t controlChecks;
    int64_t speed_up_count;
    int64_t slow_down_count;
    float serverNops;
    float clientNops;
    bool errored;
    int64_t transaction;

    std::string getTrialResultsHeader() override;
    std::string getTrialResults() override;
    std::string getTrialCSVHeader() override;
    std::string getTrialCSVData() override;

    ClosedLoopServerEndCondition();
};

template<typename elementType, typename atomicIdType, typename atomicIndexType, typename nopCountType>
size_t closedLoopAllocate(std::vector<elementType*> &shared_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<nopCountType*> &nopControls, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags){
    int maxBufferSize = openLoopAllocate<elementType, atomicIdType, atomicIndexType>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, array_lengths, block_lengths, cpus, alignment, circular, include_dummy_flags);

    for(int i = 1; i<cpus.size(); i++){//Do not need one for the controller
        nopCountType *nopControl = (nopCountType*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(nopCountType), cpus[i]);
        nopCountType *nopControlConstructed = new (nopControl) nopCountType(0);
        //std::atomic_init(nopControl, 0); //Init of the shared ptrs
        nopControls.push_back(nopControl);
    }

    return maxBufferSize;
}

template<typename atomicIndexType, typename nopCountType>
void destructSharedIDs(std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<nopCountType*> &nopControl){
    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag);

    for(int i = 0; i<nopControl.size(); i++){
        nopControl[i]->~nopCountType();
    }
}

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClientType,
         typename nopsClientTypeLocal>
void* closed_loop_buffer_reset(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientTypeLocal> *args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientTypeLocal>*) arg;

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, indexType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    std::atomic_thread_fence(std::memory_order_acquire);

    int numberInitBlocks = args->array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Using memory order released so that initialization occures in the order it occures in steady state operation

    //Reset the index pointers (they are initialized outside)
    std::atomic_store_explicit(args->read_offset_ptr, 0, std::memory_order_release); //Initialized to 0.  This is the index last read.  Index 1 contains the first initial value
    std::atomic_store_explicit(args->write_offset_ptr, numberInitBlocks+1, std::memory_order_release); //Initialized to index after that last initial value in the FIFO (note, the initial elements start at index 1)

    //=== Init ===
    //Reset the block ids and data

    //Init Blocks After Initial Conditions (these are initialized first because, after the reset, these will be the first blocks to be written to by the writer)
    for(int i = numberInitBlocks+1; i <= args->array_length; i++){ //Note, there is an extra element in the array
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        idType *block_id_start_constructed = new (block_id_start) idType();
        std::atomic_init(block_id_start, 0);
        std::atomic_store_explicit(block_id_start, 0, std::memory_order_release);

        elementType *data_array = (elementType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes*2);
        for(int j = 0; j<args->blockSize; j++){
            data_array[j] = -1;
        }
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes*2);
        idType *block_id_end_constructed = new (block_id_end) idType();
        std::atomic_init(block_id_end, 0);
        std::atomic_store_explicit(block_id_end, 0, std::memory_order_release);
    }

    //Init ID Zero
    idType *block_id_start = (idType*) (((char*) args->array));
    idType *block_id_start_constructed = new (block_id_start) idType();
    std::atomic_init(block_id_start, 0);
    std::atomic_store_explicit(block_id_start, 0, std::memory_order_release);

    elementType *data_array = (elementType*) (((char*) args->array) + idCombinedBytes*2);
    for(int j = 0; j<args->blockSize; j++){
        data_array[j] = -1;
    }

    idType *block_id_end = (idType*) (((char*) args->array) + idCombinedBytes);
    idType *block_id_end_constructed = new (block_id_end) idType();
    std::atomic_init(block_id_end, 0);
    std::atomic_store_explicit(block_id_end, 0, std::memory_order_release);

    //Initial Conditions
    for(int i = 1; i<numberInitBlocks+1; i++){
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        idType *block_id_start_constructed = new (block_id_start) idType();
        std::atomic_init(block_id_start, i);
        std::atomic_store_explicit(block_id_start, i, std::memory_order_release);

        elementType *data_array = (elementType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes*2);
        for(int j = 0; j<args->blockSize; j++){
            data_array[j] = (i+1)%2;
        }
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes);
        idType *block_id_end_constructed = new (block_id_end) idType();
        std::atomic_init(block_id_end, i);
        std::atomic_store_explicit(block_id_end, i, std::memory_order_release);
    }

    //Reset the clientNops control (this is written to after the array is written to in the steady state case)
    std::atomic_store_explicit(args->clientNops, (nopsClientTypeLocal) args->initialNops, std::memory_order_release);
    
    //Flags are initialized and destructed outside
    std::atomic_flag_test_and_set_explicit(args->start_flag, std::memory_order_release);
    std::atomic_flag_test_and_set_explicit(args->stop_flag, std::memory_order_release);
    std::atomic_flag_test_and_set_explicit(args->ready_flag, std::memory_order_release);

    std::atomic_thread_fence(std::memory_order_release);
}

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClientType,
         typename nopsLocalType>
void* closed_loop_buffer_cleanup(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsLocalType> *args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsLocalType>*) arg;

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, indexType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    std::atomic_thread_fence(std::memory_order_acquire);

    int numberInitBlocks = args->array_length/2; //Initialize FIFO to be half full (round down if odd number)

    std::atomic_thread_fence(std::memory_order_acquire);

    //=== Init ===
    //Reset the block ids and data
    //Init ID Zero
    idType *block_id_start = (idType*) (((char*) args->array));
    block_id_start->~idType();

    idType *block_id_end = (idType*) (((char*) args->array) + idCombinedBytes);
    block_id_end->~idType();

    //Initial Conditions
    for(int i = 1; i<numberInitBlocks+1; i++){
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        block_id_start->~idType();
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes);
        block_id_end->~idType();
    }

    //Init After
    for(int i = numberInitBlocks+1; i <= args->array_length; i++){ //Note, there is an extra element in the array
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        block_id_start->~idType();
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes);
        block_id_end->~idType();
    }

    std::atomic_thread_fence(std::memory_order_release);
}

//For versions where the number of NOPs is an integer
template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename idLocalType, 
         typename indexLocalType, 
         typename nopsClientType, 
         typename nopsClientLocalType, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_int_client(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>* args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;

    int core = args->core_client;

    FILE* sirFile = args->readerSirFile;

    int32_t control_client_check_period = args->control_client_check_period;
    nopsClientType *clientNops = args->clientNops;
    nopsClientLocalType clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
    
    elementType expectedSampleVals = 0;
    idLocalType readBlockInd = 0;

    idLocalType newBlockIDStart = -1;
    idLocalType newBlockIDEnd = -1;

    idLocalType expectedBlockID = -1;

    elementType localBuffer[blockSize];

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, idType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    //==== Load the initial read offset ====
    indexLocalType readOffset = std::atomic_load_explicit(read_offset_ptr, std::memory_order_acquire);

    int numberInitBlocks = array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Disable Interrupts For This Trial (Before Signalling Ready)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        int status = ioctl(fileno(sirFile), SIR_IOCTL_DISABLE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Disable Interrupts" << std::endl;
            exit(1);
        }
    #endif

    //==== Wait for ready ====
    bool ready = false;
    while(!ready){
        ready = !std::atomic_flag_test_and_set_explicit(ready_flag, std::memory_order_acq_rel);
    }

    //==== Read the uninitialized entries in the FIFO ====
    //The writer will have written the initial conditions into the FIFO and therefor will have
    //exclusive access to the cache lines in the array.  Reading the entries will force them
    //into the shared state which will force the writer to invalidate there entries in the reader.
    //This is what it needs to do in the steady state case and should do at the start as well.
    
    //Read Elements beyond initial condition
    for(int i = numberInitBlocks+1; i <= array_length+1; i++){ //Note, there is an extra element in the array
        int ind = i<=array_length ? i : 0;
        idType *block_id_end = (idType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        asm volatile(""
        :
        : "r" (newBlockIDEnd)
        :);
        
        //Read elements
        //Read backwards to avoid cache thrashing
        elementType *data_array = (elementType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes*2);
        for(int sample = 0; sample<blockSize; sample++){
            localBuffer[sample] = data_array[sample];
            asm volatile(""
            :
            : "r" (localBuffer[sample])
            :);
        }

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        idType *block_id_start = (idType*) (((char*) array) + ind*blockSizeBytes);
        newBlockIDStart = std::atomic_load_explicit(block_id_start, std::memory_order_acquire);
        asm volatile(""
        :
        : "r" (newBlockIDStart)
        :);
    }

    //==== Singal start ====
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(start_flag, std::memory_order_release);

    bool failureDetected = false;
    int64_t transfer;
    int32_t controlCounter = 0;
    for(transfer = 0; transfer<(max_block_transfers+numberInitBlocks); transfer++){ //Need to do numberInitBlocks extra reads
        //---- Read from Input Buffer Unconditionally ----
        //Load Read Ptr
        if (readOffset >= array_length) //(note, there is an extra array element)
        {
            readOffset = 0;
        }
        else
        {
            readOffset++;
        }

        //+++ Read from array +++
        //The end ID is read first to check that the values being read from the array were completly written before this thread started reading.
        idType *block_id_end = (idType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        
        //Read elements
        //Read backwards to avoid cache thrashing
        //Read forwards to allow compiler to optimize
        elementType *data_array = (elementType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes*2);

        //Need to align localBuffer to data_array so that AVX load instructions can be used
        //localBuffer is oversized by 256 bits (32 bytes) to allow for this shift

        //The localBufferAlignedBase will be set so that it has the same position inside of a 32 byte block as
        //data_array.
        //It will use standard load/store instructions until the cursors are aligned to a 16 byte boundary
        //If the 16 byte boundary is not a 32 byte boundary, it will use a SSE load/store instruction
        //It will then use the AVX load/store instructions to copy data.
        //It will then use an SSE instruction to copy remaining data (if applicable)
        //It will then use standard load/store instructions to copy the remaining data

        for(int sample = 0; sample<blockSize; sample++){
            localBuffer[sample] = data_array[sample];
        }

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        idType *block_id_start = (idType*) (((char*) array) + readOffset*blockSizeBytes);
        newBlockIDStart = std::atomic_load_explicit(block_id_start, std::memory_order_acquire);
        //+++ End read from array +++

        //Update Read Ptr
        std::atomic_store_explicit(read_offset_ptr, readOffset, std::memory_order_release);

        //Check the read block IDs
        expectedBlockID = readBlockInd == UINT32_MAX ? 0 : readBlockInd+1;
        if(expectedBlockID != newBlockIDStart || expectedBlockID != newBlockIDEnd){
            //Will signal failure outside of loop
            failureDetected = true;
            readBlockInd = expectedBlockID;
            break;
        }
        readBlockInd = expectedBlockID;

        //Check elements
        for(int sample = 0; sample<blockSize; sample++){
            if(localBuffer[sample] != expectedSampleVals){
                std::cerr << "Unexpected array data!" << std::endl;
                exit(1);
            }
        }

        expectedSampleVals = (expectedSampleVals+1)%2;

        //Check for new control
        if(controlCounter < control_client_check_period){
            controlCounter++;
        }else{
            controlCounter = 0;

            clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
        }

        //Perform NOPs
        for(nopsClientLocalType nop = 0; nop<clientNopsLocal; nop++){
            asm volatile ("nop" : : :);
        }
    }

    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(stop_flag, std::memory_order_release);

    //Re-enable interrupts before processing results (which dynamically allocates memory which can result in a system call)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        status = ioctl(fileno(sirFile), SIR_IOCTL_RESTORE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Restore Interrupts" << std::endl;
            exit(1);
        }
    #endif

    FifolessBufferEndCondition *rtn = new FifolessBufferEndCondition;
    rtn->expectedBlockID = readBlockInd;
    rtn->startBlockID = newBlockIDStart;
    rtn->endBlockID = newBlockIDEnd;
    rtn->wasErrorSrc = true;
    rtn->errored = failureDetected;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;
    rtn->transaction = transfer;

    return (void*) rtn;
}

//For versions where the number of NOPs is kept as a float
template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename idLocalType, 
         typename indexLocalType, 
         typename nopsClientType, 
         typename nopsClientLocalType, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_float_client(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>* args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;

    int core = args->core_client;

    FILE* sirFile = args->readerSirFile;

    int32_t control_client_check_period = args->control_client_check_period;
    nopsClientType *clientNops = args->clientNops;
    nopsClientLocalType nops_current = 0; //Is used to carry over residual between nop cycles
    nopsClientLocalType clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
    
    elementType expectedSampleVals = 0;
    idLocalType readBlockInd = 0;

    idLocalType newBlockIDStart = -1;
    idLocalType newBlockIDEnd = -1;

    idLocalType expectedBlockID = -1;

    size_t bufferBytesForLocal = 32;//Statically define for now.  TODO, change?
    size_t localElementPadding = bufferBytesForLocal/sizeof(elementType);
    elementType localBufferRaw[blockSize+localElementPadding]; //Automatic allocation

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, idType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    //==== Load the initial read offset ====
    indexLocalType readOffset = std::atomic_load_explicit(read_offset_ptr, std::memory_order_acquire);

    int numberInitBlocks = array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Disable Interrupts For This Trial (Before Signalling Ready)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        int status = ioctl(fileno(sirFile), SIR_IOCTL_DISABLE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Disable Interrupts" << std::endl;
            exit(1);
        }
    #endif

    //==== Wait for ready ====
    bool ready = false;
    while(!ready){
        ready = !std::atomic_flag_test_and_set_explicit(ready_flag, std::memory_order_acq_rel);
    }

    //==== Read the uninitialized entries in the FIFO ====
    //The writer will have written the initial conditions into the FIFO and therefor will have
    //exclusive access to the cache lines in the array.  Reading the entries will force them
    //into the shared state which will force the writer to invalidate there entries in the reader.
    //This is what it needs to do in the steady state case and should do at the start as well.
    
    //Read Elements beyond initial condition
    for(int i = numberInitBlocks+1; i <= array_length+1; i++){ //Note, there is an extra element in the array
        int ind = i<=array_length ? i : 0;
        idType *block_id_end = (idType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        asm volatile(""
        :
        : "r" (newBlockIDEnd)
        :);
        
        //Read elements
        elementType *data_array = (elementType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes*2);
        for(int sample = 0; sample<blockSize; sample++){
            localBufferRaw[sample] = data_array[sample];
            asm volatile(""
            :
            : "r" (localBufferRaw[sample])
            :);
        }

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        idType *block_id_start = (idType*) (((char*) array) + ind*blockSizeBytes);
        newBlockIDStart = std::atomic_load_explicit(block_id_start, std::memory_order_acquire);
        asm volatile(""
        :
        : "r" (newBlockIDStart)
        :);
    }

    //==== Singal start ====
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(start_flag, std::memory_order_release);

    bool failureDetected = false;
    int64_t transfer;
    int32_t controlCounter = 0;
    for(transfer = 0; transfer<(max_block_transfers+numberInitBlocks); transfer++){ //Need to do numberInitBlocks extra reads
        //---- Read from Input Buffer Unconditionally ----
        //Load Read Ptr
        if (readOffset >= array_length) //(note, there is an extra array element)
        {
            readOffset = 0;
        }
        else
        {
            readOffset++;
        }

        //+++ Read from array +++
        //The end ID is read first to check that the values being read from the array were completly written before this thread started reading.
        idType *block_id_end = (idType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        
        //Read elements
        elementType *data_array = (elementType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes*2);
        // for(int sample = 0; sample<blockSize; sample++){
        //     localBuffer[sample] = data_array[sample];
        // }
        elementType* localBuffer = (elementType*) fast_copy(data_array, localBufferRaw, blockSize*sizeof(elementType), bufferBytesForLocal);

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        idType *block_id_start = (idType*) (((char*) array) + readOffset*blockSizeBytes);
        newBlockIDStart = std::atomic_load_explicit(block_id_start, std::memory_order_acquire);
        //+++ End read from array +++

        //Update Read Ptr
        std::atomic_store_explicit(read_offset_ptr, readOffset, std::memory_order_release);

        //Check the read block IDs
        expectedBlockID = readBlockInd == UINT32_MAX ? 0 : readBlockInd+1;
        if(expectedBlockID != newBlockIDStart || expectedBlockID != newBlockIDEnd){
            //Will signal failure outside of loop
            failureDetected = true;
            readBlockInd = expectedBlockID;
            break;
        }
        readBlockInd = expectedBlockID;

        //Check elements
        for(int sample = 0; sample<blockSize; sample++){
            if(localBuffer[sample] != expectedSampleVals){
                std::cerr << "Unexpected array data!" << std::endl;
                exit(1);
            }
        }

        expectedSampleVals = (expectedSampleVals+1)%2;

        //Check for new control
        if(controlCounter < control_client_check_period){
            controlCounter++;
        }else{
            controlCounter = 0;

            clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
        }

        //Perform NOPs
        nops_current += clientNopsLocal;//nop_current contains the resitual from the last round
        int32_t nops_current_trunk = std::floor(nops_current); //Truncate to a fixed number of NOPs
        for(int32_t nop = 0; nop<nops_current_trunk; nop++){
            asm volatile ("nop" : : :);
        }
        nops_current -= nops_current_trunk;//Get the residual for the next round
    }

    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(stop_flag, std::memory_order_release);

    //Re-enable interrupts before processing results (which dynamically allocates memory which can result in a system call)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        status = ioctl(fileno(sirFile), SIR_IOCTL_RESTORE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Restore Interrupts" << std::endl;
            exit(1);
        }
    #endif

    FifolessBufferEndCondition *rtn = new FifolessBufferEndCondition;
    rtn->expectedBlockID = readBlockInd;
    rtn->startBlockID = newBlockIDStart;
    rtn->endBlockID = newBlockIDEnd;
    rtn->wasErrorSrc = true;
    rtn->errored = failureDetected;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;
    rtn->transaction = transfer;

    return (void*) rtn;
}

#endif

