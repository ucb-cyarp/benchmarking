#ifndef _H_FIFOLESS_HELPERS
#define _H_FIFOLESS_HELPERS

#include "reporting_helpers.h"

class FifolessConfig : public Config{
    public:
        void printExportCorrespondingResult(Results &result, bool report_standalone, std::string title, Profiler* profiler, std::vector<int> cpus, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file) override;
    protected:
        virtual void printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file)=0;
        virtual void printResultsStandalone(Results &result);
};

#endif 