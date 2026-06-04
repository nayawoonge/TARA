#ifndef ECU_DIAGRAM_H
#define ECU_DIAGRAM_H

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.h"

#define MAX_DIR_PATH_LEN 100 

int export_ECU_Diagram(const char *defluat_dot_dir_path, struct CAN_Message *CAN_m);

#endif