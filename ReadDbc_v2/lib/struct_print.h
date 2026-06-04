#ifndef STRUCT_PRINT_H //헤더가드
#define STRUCT_PRINT_H

#include <sys/stat.h>
#include <errno.h>

#include "util.h"

void struct_print(struct CAN_Message *CAN_m);
void identify_asset(const char *csv_file_dir, const char *file_name, struct CAN_Message *CAN_m, struct ECU *ecu, int ecu_count);

#endif
