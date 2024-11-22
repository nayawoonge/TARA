#include "parse_dbc_file.h"

int parse_dbc_file(const char *file_path, struct CAN_Message *CAN_m, struct ECU *ecu) 
{
    FILE *dbc_file = fopen(file_path, "r");
    if (dbc_file == NULL) 
    {
        printf("파일을 열 수 없습니다.\n");
        return 0;
    }

    char line[800];
    int ecu_count = 0;
    int msg_id;
    char msg_name[32];
    int msg_dlc;
    char msg_sender[32];

    while (fgets(line, sizeof(line), dbc_file)) 
    {
        // BU_ 섹션 파싱
        if (strncmp(line, "BU_", 3) == 0) 
        {
            char *token = strtok(line + 4, " \n");
            while (token != NULL) {
                strncpy(ecu[ecu_count].name, token, sizeof(ecu[ecu_count].name) - 1);
                ecu[ecu_count].name[sizeof(ecu[ecu_count].name) - 1] = '\0';
                ecu_count++;
                token = strtok(NULL, " \n");
            }
        }
        // 메시지 파싱
        else if (strncmp(line, "BO_", 3) == 0) 
        {
            sscanf(line, "BO_ %d %15[^:]: %d %24s", &msg_id, msg_name, &msg_dlc, msg_sender);
            CAN_m[msg_id].id = msg_id;
            strncpy(CAN_m[msg_id].name, msg_name, sizeof(CAN_m[msg_id].name) - 1);
            CAN_m[msg_id].dlc = msg_dlc;
            strncpy(CAN_m[msg_id].sender, msg_sender, sizeof(CAN_m[msg_id].sender) - 1);
        }
        // 신호 파싱
        else if (strncmp(line, " SG_", 4) == 0) 
        {
            sscanf(line, " SG_ %31s : %d|%d@%d%c (%lf,%lf) [%lf|%lf] \"%31s  %209s\n",
                 CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].name,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].start_bit,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].length,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].byteorder,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].sign,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].factor,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].offset,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].min_val,
                &CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].max_val,
                 CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].unit,
                 CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].receiver);
            
            CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].unit[strlen(CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].unit) - 1] = '\0';

            if (CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].unit[0] == '\0') 
            {
                strncpy(CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].unit, "None", 32 - 1);
                // printf("DEBUG u0: %s\n", CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].receiver);
            }
            // 수신 ECU가 빈 문자열일 경우 처리
            if (CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].receiver[0] == '\0') 
            {
                strncpy(CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].receiver, "None", 210 - 1);
                // printf("DEBUG r0: %s\n", CAN_m[msg_id].CAN_s[CAN_m[msg_id].sig_counter].receiver);
            }
            CAN_m[msg_id].sig_counter++;
        }
    }

    fclose(dbc_file);
    return ecu_count;
}
