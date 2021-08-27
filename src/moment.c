#include <moment.h>

moment get_time() {
  struct timeval tv;
  gettimeofday(&tv, 0);

  return tv;
}

double time_diff_micro_s(moment start, moment end) {
  double x_ms, y_ms, diff;

  x_ms = (double)start.tv_sec * 1000000 + (double)start.tv_usec;
  y_ms = (double)end.tv_sec * 1000000 + (double)end.tv_usec;

  diff = (double)y_ms - (double)x_ms;

  return diff;
}

double time_diff_milli_s(moment start, moment end) {
  return time_diff_micro_s(start, end) / 1000.0f;
}

double time_diff_seconds(moment start, moment end) {
  return time_diff_milli_s(start, end) / 1000.0f;
}
