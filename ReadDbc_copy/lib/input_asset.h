#ifndef INPUT_ASSET_H // 헤더 가드 추가
#define INPUT_ASSET_H

#include "util.h"

// input_asset 함수 선언
void input_asset(const char *csv_dir_path, const char *output_file_path, struct CAN_Message *can_m, struct ECU *ecu, int ecu_count);

#endif  // 헤더 가드 끝