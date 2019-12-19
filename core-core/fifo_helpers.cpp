#include "fifo_helpers.h"

int countNumberOfFIFOExperiments(std::vector<size_t> array_lengths, std::vector<int32_t> writes_per_transactions){
    int result_count = 0;

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];

        for(int j = 0; j<writes_per_transactions.size(); j++)
        {
            int32_t writes_per_transaction = writes_per_transactions[j];

            if(writes_per_transaction <= array_length) //Check if we should bother running this case
            {
                result_count++;
            }
        }
    }

    return result_count;
}