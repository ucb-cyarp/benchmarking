#include "reporting_helpers.h"

void printTitleArray(bool report_standalone, std::string title, size_t array_length){
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("%s | Start Time: %s", title.c_str(), asctime(localT));
        printf("Array Length: %lu int32_t Elements\n", array_length);
    }
    #endif
}

void writeCSVHeader(FILE* file, std::ofstream* raw_file){
    #if WRITE_CSV == 1
    fprintf(file, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif
}

//Return format
std::string tableHeaderArray1Stream(std::string title, FILE* file, std::ofstream* raw_file){
    time_t currentTime = time(NULL);
    struct tm * localT;
    localT = localtime(&currentTime);
    printf("%s | Start Time: %s", title.c_str(), asctime(localT));
    printf("        ==========================================================================================\n");
    printf("          Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
    printf("        (int32_t Elements) |       Avg, StdDev        |                         |                 \n");
    printf("        ==========================================================================================\n");

    writeCSVHeader(file, raw_file);

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";
    return format;
}

//Return format
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
}

void printTitleFIFO(std::string title, int columns, int column_width){
    #if PRINT_TITLE == 1
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("%s | Start Time: %s", title.c_str(), asctime(localT));
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
    #endif
}

//Returns format
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
    #endif

    return "|%9.2f";
}

void printTitleFIFOPoint(bool report_standalone, std::string title, size_t array_length, std::string second_param_label, int32_t second_param){
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("%s | Start Time: %s", title.c_str(), asctime(localT));
        printf("Array Length: %lu int32_t Elements, %s: %d\n", array_length, second_param_label.c_str(), second_param);
    }
    #endif
}