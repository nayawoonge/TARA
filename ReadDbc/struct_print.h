#ifndef STRUCT_PRINT_H //헤더가드
#define STRUCT_PRINT_H

#include "util.h"

void struct_print(struct CAN_Message *CAN_m);
void output_file(const char *file_name, struct CAN_Message *CAN_m, struct ECU *ecu, int ecu_count);

#endif
