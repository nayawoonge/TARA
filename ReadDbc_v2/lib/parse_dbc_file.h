#ifndef PARSE_DBC_FILE_H  // 헤더 가드 추가
#define PARSE_DBC_FILE_H

#include "util.h"

// parse_dbc_file 함수 선언
int parse_dbc_file(const char *file_path, struct CAN_Message *CAN_m, struct ECU *ecu);

#endif  // 헤더 가드 끝
