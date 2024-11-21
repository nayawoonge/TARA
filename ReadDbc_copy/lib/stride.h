#ifndef STRIDE_H // 헤더 가드 추가
#define STRICE_H

#include "util.h"

// stride_asset 함수 선언
void stride_asset(const char *csv_dir_path, const char *output_file_path, struct CAN_Message *CAN_m, struct ECU *ecu, int ecu_count);

#endif  // 헤더 가드 끝