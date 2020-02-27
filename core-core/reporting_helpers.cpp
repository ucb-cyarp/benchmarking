#include "reporting_helpers.h"

/**
 * Prints the title of the given test case to the console)
 */
void printTitle(std::string title){
    time_t currentTime = time(NULL);
    struct tm * localT;
    localT = localtime(&currentTime);
    printf("%s | Start Time: %s", title.c_str(), asctime(localT));
}

/**
 * Prints the title of an array test case
 */
void printTitleArray(bool report_standalone, std::string title, size_t array_length){
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printTitle(title);
        printf("Array Length: %lu int32_t Elements\n", array_length);
        fflush(stdout);
    }
    #endif
}

/**
 * Writes the CSV headers for a typical benchmark
 */
void writeCSVHeader(FILE* file, std::ofstream* raw_file){
    #if WRITE_CSV == 1
    fprintf(file, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    raw_file->flush();
    #endif
}

/**
 * Given a vector of CPUs, get a list of sockets, cores, and dies that encompas those CPUs
 */
void getGranularityIndexsOfInterest(std::vector<int> cpus, Profiler *profiler, std::vector<int> &sockets, std::vector<int> &cores, std::vector<int> &dies, std::vector<int> &threads){
    std::set<int> socketSet;
    for(int i = 0; i<cpus.size(); i++){
        int socket = profiler->cpuTopology[cpus[i]].socket;
        if(socketSet.find(socket) == socketSet.end()){
            socketSet.insert(socket);
            sockets.push_back(socket);
        }
    }

    std::set<int> coreSet;
    for(int i = 0; i<cpus.size(); i++){
        int core = profiler->cpuTopology[cpus[i]].core;
        if(coreSet.find(core) == coreSet.end()){
            coreSet.insert(core);
            cores.push_back(core);
        }
    }

    std::set<int> dieSet;
    for(int i = 0; i<cpus.size(); i++){
        int die = profiler->cpuTopology[cpus[i]].die;
        if(dieSet.find(die) == dieSet.end()){
            dieSet.insert(die);
            dies.push_back(die);
        }
    }

    std::set<int> threadSet;
    for(int i = 0; i<cpus.size(); i++){
        if(threadSet.find(cpus[i]) == threadSet.end()){
            threadSet.insert(cpus[i]);
            threads.push_back(cpus[i]);
        }
    }
}

/**
 * Prints the title and a table header for a typical benchmark measuring latency and data rate from one thread to another (one stream)
 * Also writes the CSV headers
 * 
 * Return a format string used to format results when printing to the console
 */
std::string tableHeaderArray1Stream(std::string title, FILE* file, std::ofstream* raw_file){
    #if PRINT_TITLE == 1
    printf("\n");
    printTitle(title);
    printf("        ==========================================================================================\n");
    printf("          Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
    printf("        (int32_t Elements) |       Avg, StdDev        |                         |                 \n");
    printf("        ==========================================================================================\n");
    #endif

    writeCSVHeader(file, raw_file);

    fflush(stdout);

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";
    return format;
}

/**
 * Prints the title and a table header for a typical benchmark measuring latency and data rate when two streams of data are in flight
 * Also writes the CSV headers
 * 
 * Return a format string used to format results when printing to the console
 */
std::string tableHeaderArray2Streams(std::string title, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool bidirectional){
    //The printed header is the same as the 1 stream version
    tableHeaderArray1Stream(title, file_a, raw_file_a);

    //Write the header for the second stream csv files (first stream is handled by tableHeaderArray1Stream)
    writeCSVHeader(file_b, raw_file_b);

    std::string format = "";
    if(bidirectional){
        format = "          %3d -> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";
    }else{
        format = "         %3d <-> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";
    }

    return format;
}

void tableFooter(){
    printf("        ==========================================================================================\n");
    fflush(stdout);
}

/**
 * Prints the title for a FIFO benchmark allong with the header for a table of results writen to the console.
 * The results are in a 2D table with parameters being swept in the 2 axes of the table
 */
void printTitleFIFO(std::string title, int columns, int column_width){
    #if PRINT_TITLE == 1
        printf("\n");
        printTitle(title);
        printf("        Lengths in int32_t Elements, Data Rates in Mbps\n");
        printf("        ===========================");
        for(int i = 0; i<columns; i++)
        {
            for(int j = 0; j<column_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
        fflush(stdout);
    #endif
}

/**
 * Writes the summary csv file header for the open loop benchmarks.
 */
void writeCSVSummaryHeaderOpenLoop(FILE* file){
    #if WRITE_CSV == 1
    fprintf(file, "\"Array Length (Blocks)\",\"Block Size (int32_t Elements)\",\"Balancing NOPs\",\"Time To Failure (ms) - Avg\",\"Time to Failure (ms) - StdDev\"\n");
    fflush(file);
    #endif
}

/**
 * Writes the summary csv file header for the open loop benchmarks.
 */
void writeCSVSummaryHeaderClosedLoop(FILE* file){
    #if WRITE_CSV == 1
    fprintf(file, "\"Array Length (Blocks)\",\"Block Size (int32_t Elements)\",\"Server Control Check Period\",\"Client Control Check Period\",\"Control Gain P\",\"Control Gain I\",\"Initial NOPs\",\"Time To Failure (ms) - Avg\",\"Time to Failure (ms) - StdDev\"\n");
    fflush(file);
    #endif
}

/**
 * Prints the table header to the consile for the open loop benchmarks.  Also writes the summary csv file header.
 */
std::string tableHeaderOpenLoop(std::string title, FILE* file){
    #if PRINT_TITLE == 1
        printf("\n");
        printTitle(title);
        printf("        ==================================================================================================\n");
        printf("          Array Length  |      Block Size      |  Balancing NOPs  | Initial NOPs | Time to Failure (ms)   \n");
        printf("            (Blocks)    |  (int32_t Elements)  |                  |              | Avg, StdDev            \n");
        printf("        ==================================================================================================\n");

        fflush(stdout);
    #endif

    writeCSVSummaryHeaderOpenLoop(file);

    return "        %15d | %20d | %16d | %12d | %10.4e, %10.4e\n";
}

/**
 * Prints the table headers (to the console, the summary CSV file, and the raw CSV file) for FIFO benchmark
 * 
 * The console and summary CSV tables are 2D table with parameters being swept in the 2 axes of the table.
 * 
 * The raw CSV file contains a row for each trial and is not in the same 2D style as the console or summary CSV file
 * 
 * Returns a format string to be used when printing the data to the console
 */
std::string printAndWriteHeadersFIFO(std::string secondary_label_printcsv, std::string secondary_label_rawcsv, std::vector<int32_t> secondary_dimension_items, int data_col_width, FILE* file, std::ofstream* raw_file){
    #if PRINT_STATS == 1
        printf("        Array Len \\ %s ", secondary_label_printcsv.c_str());
        for(int i = 0; i<secondary_dimension_items.size(); i++)
        {
            printf("|%9.2d", secondary_dimension_items[i]); //12 becaused of spaces
        }
        printf("\n");
        printf("        ===========================");
        for(int i = 0; i<secondary_dimension_items.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++) 
            {
                printf("=");
            }
        }
        fflush(stdout);
    #endif

    #if WRITE_CSV == 1
        fprintf(file, "\"Array Len \\ %s (int32_t elements)\"", secondary_label_printcsv.c_str());//Command inserted below
        for(int i = 0; i<secondary_dimension_items.size(); i++)
        {
            fprintf(file, ",%d", secondary_dimension_items[i]);
        }
        //fprintf(file, "\n"); //Done below
        fflush(file);
        *raw_file << "\"Transfer Length (int32_t Elements)\",\"" << secondary_label_rawcsv << " (int32_t Elements)\",\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        raw_file->flush();
    #endif

    return "|%9.2f";
}

/**
 * Prints information about a proceeding FIFO data point to the console if reporting in standalone mode.
 */
void printTitleFIFOPoint(bool report_standalone, std::string title, size_t array_length, std::string second_param_label, int32_t second_param){
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printTitle(title);
        printf("Array Length: %lu int32_t Elements, %s: %d\n", array_length, second_param_label.c_str(), second_param);
        fflush(stdout);
    }
    #endif
}

void writeRawHeaderOpenLoop(std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResults, std::ofstream* raw_file){
    #if WRITE_CSV == 1
        *raw_file << "\"Array Length (Blocks)\","
                  << "\"Block Size (int32_t Elements)\","
                  << "\"Balancing NOPs\","
                  << "\"Initial NOPs\","
                  << "\"Steady Clock - Walltime (ms)\","
                  << "\"Clock - Cycles/Cycle Time (ms)\","
                  << "\"Clock - rdtsc\"";

        for(int i = 0; i<implSpecificResults.size(); i++){
            if(implSpecificResults[i] != nullptr){
                std::string trialCSVHeader = implSpecificResults[i]->getTrialCSVHeader();
                *raw_file << "," << trialCSVHeader;
            }
        }

        *raw_file << std::endl;

        raw_file->flush();
    #endif
}

/**
 * Prints the table header to the consile for the closed loop benchmarks.  Also writes the summary csv file header.
 */
std::string tableHeaderClosedLoopBang(std::string title, FILE* file){
    #if PRINT_TITLE == 1
        printf("\n");
        printTitle(title);
        printf("        ====================================================================================================================================\n");
        printf("          Array Length  |      Block Size      | Server Control      | Client Control      | Control Gain | Initial |  Time to Failure (ms) \n");
        printf("            (Blocks)    |  (int32_t Elements)  | Period (Iterations) | Period (Iterations) |    (NOPs)    |   NOPs  |      Avg, StdDev      \n");
        printf("        ====================================================================================================================================\n");

        fflush(stdout);
    #endif

    writeCSVSummaryHeaderClosedLoop(file);

    return "         %14d | %20d | %19d | %19d | %12.2f | %7.2f | %10.4e, %10.4e\n";
}

void writeRawHeaderClosedLoopBang(std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResults, std::ofstream* raw_file){
    #if WRITE_CSV == 1
        *raw_file << "\"Array Length (Blocks)\","
                  << "\"Block Size (int32_t Elements)\","
                  << "\"Server Control Period (Iterations)\","
                  << "\"Client Control Period (Iterations)\","
                  << "\"Control Gain (NOPs)\","
                  << "\"Initial NOPs\","
                  << "\"Steady Clock - Walltime (ms)\","
                  << "\"Clock - Cycles/Cycle Time (ms)\","
                  << "\"Clock - rdtsc\"";

        for(int i = 0; i<implSpecificResults.size(); i++){
            if(implSpecificResults[i] != nullptr){
                std::string trialCSVHeader = implSpecificResults[i]->getTrialCSVHeader();
                *raw_file << "," << trialCSVHeader;
            }
        }

        *raw_file << std::endl;

        raw_file->flush();
    #endif
}

/**
 * Prints the table header to the consile for the closed loop benchmarks.  Also writes the summary csv file header.
 */
std::string tableHeaderClosedLoopPI(std::string title, FILE* file){
    #if PRINT_TITLE == 1
        printf("\n");
        printTitle(title);
        printf("        =================================================================================================================================================================================\n");
        printf("          Array Length  |      Block Size      | Server Control      | Client Control      | Control Gain P | Control Gain I | Control Gain Base  | Initial |  Time to Failure (ms) \n");
        printf("            (Blocks)    |  (int32_t Elements)  | Period (Iterations) | Period (Iterations) |    (NOPs)      |    (NOPs)      |        (NOPs)      |   NOPs  |      Avg, StdDev      \n");
        printf("        =================================================================================================================================================================================\n");

        fflush(stdout);
    #endif

    writeCSVSummaryHeaderClosedLoop(file);

    return "         %14d | %20d | %19d | %19d | %14.2f | %14.2f | %18.2f | %7.2f | %10.4e, %10.4e\n";
}

void writeRawHeaderClosedLoopPI(std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResults, std::ofstream* raw_file){
    #if WRITE_CSV == 1
        *raw_file << "\"Array Length (Blocks)\","
                  << "\"Block Size (int32_t Elements)\","
                  << "\"Server Control Period (Iterations)\","
                  << "\"Client Control Period (Iterations)\","
                  << "\"Control Gain P (NOPs)\","
                  << "\"Control Gain I (NOPs)\","
                  << "\"Control Gain Base (NOPs)\","
                  << "\"Initial NOPs\","
                  << "\"Steady Clock - Walltime (ms)\","
                  << "\"Clock - Cycles/Cycle Time (ms)\","
                  << "\"Clock - rdtsc\"";

        for(int i = 0; i<implSpecificResults.size(); i++){
            if(implSpecificResults[i] != nullptr){
                std::string trialCSVHeader = implSpecificResults[i]->getTrialCSVHeader();
                *raw_file << "," << trialCSVHeader;
            }
        }

        *raw_file << std::endl;

        raw_file->flush();
    #endif
}

void closeEntry(FILE* file, std::ofstream* raw_file){
    //Print the newline
    #if WRITE_CSV == 1
    fprintf(file, "\n");
    fflush(file);
    #endif

    #if PRINT_TITLE == 1
    tableFooter();
    #endif
}