#ifndef IMPACT_RATING_H // 헤더 가드 추가
#define IMPACT_RATING_H

#include "util.h"

#define MAX_LINE_LENGTH 1024

// safety, financial, operational, privacy 영역 별 영향평가 함수 선언
void rating_asset(const char *csv_dir_path, const char *default_output_file_path, const char *default_stride_file_name);

#endif  // 헤더 가드 끝