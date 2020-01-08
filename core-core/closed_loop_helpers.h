#ifndef _CLOSED_LOOP_HELPERS_H
#define _CLOSED_LOOP_HELPERS_H

#include "intrin_bench_default_defines.h"
#include "results.h"
#include "mallocHelpers.h"
#include "open_loop_helpers.h"

#include <vector>
#include <atomic>
#include <stdlib.h>
#include <stdint.h>

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
    int64_t serverNops;
    int64_t clientNops;
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
        nopCountType *nopControlConstructed = new (nopControl) nopCountType();
        std::atomic_init(nopControl, 0); //Init of the shared ptrs
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

#endif