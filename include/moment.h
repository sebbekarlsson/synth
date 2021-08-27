#ifndef ROB_MOMENT_H
#define ROB_MOMENT_H
#include <sys/time.h>
typedef struct timeval moment;


moment get_time();


double time_diff_micro_s(moment start, moment end);

double time_diff_milli_s(moment start, moment end);

double time_diff_seconds(moment start, moment end);
#endif
