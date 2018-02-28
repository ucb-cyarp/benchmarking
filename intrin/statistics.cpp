#include "intrin_bench_default_defines.h"

void statistics(std::chrono::duration<double, std::ratio<1, 1000>>* durations, double* durations_clock)
{
    //Print Average & StdDev
    double avg_duration = 0;
    double avg_duration_clock = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        avg_duration += (double) durations[i].count();
        avg_duration_clock += (double) durations_clock[i];
    }
    avg_duration /= TRIALS;
    avg_duration_clock /= TRIALS;

    double std_dev_duration = 0;
    double std_dev_duration_clock = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        double tmp = durations[i].count() - avg_duration;
        double tmp_clock = durations_clock[i] - avg_duration_clock;
        tmp = tmp*tmp;
        tmp_clock = tmp_clock*tmp_clock;

        std_dev_duration += tmp;
        std_dev_duration_clock += tmp_clock;
    }
    std_dev_duration /= (TRIALS - 1);
    std_dev_duration_clock /= (TRIALS - 1);
    std_dev_duration = sqrt(std_dev_duration);
    std_dev_duration_clock = sqrt(std_dev_duration_clock);

    printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);
    printf("         Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock, std_dev_duration_clock);

    printf("High Res Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration));
    printf("         Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration_clock));
}