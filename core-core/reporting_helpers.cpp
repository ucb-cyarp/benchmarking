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
        fflush(stdout);
    }
    #endif
}

void writeCSVHeader(FILE* file, std::ofstream* raw_file){
    #if WRITE_CSV == 1
    fprintf(file, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    raw_file->flush();
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

    fflush(stdout);

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
    fflush(stdout);
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
        fflush(stdout);
    #endif
}

void printTitleOpenLoop(std::string title, int columns, int column_width){
    #if PRINT_TITLE == 1
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("%s | Start Time: %s", title.c_str(), asctime(localT));
        printf("        Array Sizes in Blocks, Blocks in int32_t elements, Time in ns\n");
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
        fflush(stdout);
    }
    #endif
}

//Because more than 2 axes are being considered, all of the reporting follows the same basic formatting with the parameters in the first colums and the results in the latter colums.  A trial or configuration is on each row
std::string  printAndWriteHeadersOver3Params(std::vector<std::string> param_lbls, std::vector<std::string> reporting_lbls = {"Steady Clock - Walltime (ms)", "Clock - Cycles/Cycle Time (ms)", "Clock - rdtsc"}, int data_col_width, FILE* file, std::ofstream* raw_file){
    #if PRINT_STATS == 1
        printf("        ");
        for(int i = 0; i<param_lbls.size(); i++)
        {
            printf("|%"+ std::to_string(data_col_width) +"s", param_lbls[i]);
        }

        for(int i = 0; i<reporting_lbls.size(); i++)
        {
            printf("|%"+ std::to_string(data_col_width) +"s", reporting_lbls[i]);
        }
        printf("|\n");
        printf("        ");
        for(int i = 0; i<(param_lbls.size()+reporting_lbls.size()); i++)
        {
            for(int j = 0; j<data_col_width; j++) 
            {
                printf("=");
            }
        }

        printf("\n");
        fflush(stdout);
    #endif

    #if WRITE_CSV == 1
        for(int i = 0; i<param_lbls.size(); i++)
        {
            fprintf(file, (i>0 ? ",%s" : "%s"), param_lbls[i]);
            *raw_file << param_lbls[i] << (i>0 ? "," : "");
        }

        for(int i = 0; i<reporting_lbls.size(); i++)
        {
            fprintf(file, (i>0 ? ",%s" : "%s"), reporting_lbls[i]);
            *raw_file << reporting_lbls[i] << (i>0 ? "," : "");
        }

        fprintf(file, "\n");
        *raw_file << std::endl;

        fflush(file);
        raw_file->flush();

        fprintf(file, "\"Array Len (Blocks) \\ %s (int32_t Elements)\"", secondary_label_printcsv.c_str());//Command inserted below
        for(int i = 0; i<secondary_dimension_items.size(); i++)
        {
            fprintf(file, ",%d", secondary_dimension_items[i]);
        }
        //fprintf(file, "\n"); //Done below
        fflush(file);
        *raw_file << "\"Array Len (int32_t Elements)\",\"" << secondary_label_rawcsv << " (int32_t Elements)\",\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif

    return "|%9.2f";
}