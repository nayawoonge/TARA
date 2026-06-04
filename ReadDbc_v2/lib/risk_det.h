#ifndef RISK_DET_H // 헤더 가드 추가
#define RISK_DET_H

#include "util.h"
#define MAX_LINE_LENGTH 1024

void risk_determination(const char *csv_dir_path, const char *default_output_file_path, const char *default_feasibility_file_name);
const char *determine_risk_value(int sum);

#endif  // 헤더 가드 끝