#include "parse_dbc_file.h"

int parse_dbc_file(const char *file_path, struct CAN_Message *CAN_m, struct ECU *ecu) {
    FILE *dbc_file = fopen(file_path, "r");
    if (dbc_file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 0;
    }

    char line[3000];
    int ecu_count = 0;
    int msg_id;
    char msg_name[32];
    int msg_dlc;
    char msg_sender[32];

    while (fgets(line, sizeof(line), dbc_file)) {
        // BU_ 섹션 파싱
        if (strncmp(line, "BU_", 3) == 0) {
            char *token = strtok(line + 4, " \n");
            while (token != NULL) {
                strncpy(ecu[ecu_count].name, token, sizeof(ecu[ecu_count].name) - 1);
                ecu[ecu_count].name[sizeof(ecu[ecu_count].name) - 1] = '\0';
                // printf("ECU: %s\n", ecu[ecu_count].name);
                ecu_count++;
                token = strtok(NULL, " \n");
            }
            // ecu count 테스트
            // printf("\n ecu count : %d\n", ecu_count);
        }
        // 메시지 파싱
        else if (strncmp(line, "BO_", 3) == 0) {
            sscanf(line, "BO_ %d %31[^:]: %d %31s", &msg_id, msg_name, &msg_dlc, msg_sender);
            CAN_m[msg_id].id = msg_id;
            strncpy(CAN_m[msg_id].name, msg_name, sizeof(CAN_m[msg_id].name) - 1);
            CAN_m[msg_id].dlc = msg_dlc;
            strncpy(CAN_m[msg_id].sender, msg_sender, sizeof(CAN_m[msg_id].sender) - 1);

            // 메시지 출력
            // printf("\n메시지 ID: 0x%X | 메시지 이름: %s | DLC: %d | 송신 노드: %s\n", CAN_m[msg_id].id, CAN_m[msg_id].name, CAN_m[msg_id].dlc, CAN_m[msg_id].sender);
        }
        // 신호 파싱
        else if (strncmp(line, " SG_", 4) == 0) {
            char sig_name[32];
            int unit_length;
            int sig_start_bit;
            int sig_length;
            int sig_byteorder;
            char sig_sign;
            float sig_factor;
            float sig_offset;
            float sig_max;
            float sig_min;
            char sig_unit[32] = {0};
            char sig_receiver[256] = {0};
            //printf("%s\n",line);

            sscanf(line, " SG_ %31s : %d|%d@%d%c (%f,%f) [%f|%f] \"%31s  %255s\n",
                sig_name, &sig_start_bit, &sig_length, &sig_byteorder, &sig_sign, 
                &sig_factor, &sig_offset, &sig_min, &sig_max, sig_unit, sig_receiver);
            
            unit_length = strlen(sig_unit);
            sig_unit[unit_length-1] = '\0';

            strncpy(CAN_m[msg_id].CAN_s[sig_start_bit].name, sig_name, sizeof(CAN_m[msg_id].CAN_s[sig_start_bit].name) - 1);
            CAN_m[msg_id].CAN_s[sig_start_bit].start_bit = sig_start_bit;
            CAN_m[msg_id].CAN_s[sig_start_bit].length = sig_length;
            CAN_m[msg_id].CAN_s[sig_start_bit].byteorder = sig_byteorder;
            CAN_m[msg_id].CAN_s[sig_start_bit].sign = sig_sign;
            CAN_m[msg_id].CAN_s[sig_start_bit].factor = sig_factor;
            CAN_m[msg_id].CAN_s[sig_start_bit].offset = sig_offset;
            CAN_m[msg_id].CAN_s[sig_start_bit].min_val = sig_min;
            CAN_m[msg_id].CAN_s[sig_start_bit].max_val = sig_max;

        
            // 단위가 빈 문자열일 경우 처리
            if (sig_unit[0] == 0) {
                // printf("단위가 없습니다.\n");
                // 단위가 없는 경우 기본값을 지정할 수도 있습니다.
                strncpy(CAN_m[msg_id].CAN_s[sig_start_bit].unit, "없음", sizeof(CAN_m[msg_id].CAN_s[sig_start_bit].unit) - 1);
            } else {
                // ecu가 여러개일 경우 ','를 ' '로 치환해서 저장
                strncpy(CAN_m[msg_id].CAN_s[sig_start_bit].unit, sig_unit, sizeof(CAN_m[msg_id].CAN_s[sig_start_bit].unit) - 1);
            }

            // 수신 ECU가 빈 문자열일 경우 처리
            if (sig_receiver[0] == '\0') {
                // printf("수신 ECU가 없습니다.\n");
                // 수신 ECU가 없는 경우 기본값을 지정할 수도 있습니다.
                strncpy(CAN_m[msg_id].CAN_s[sig_start_bit].receiver, "없음", sizeof(CAN_m[msg_id].CAN_s[sig_start_bit].receiver) - 1);
            } else {
                strncpy(CAN_m[msg_id].CAN_s[sig_start_bit].receiver, sig_receiver, sizeof(CAN_m[msg_id].CAN_s[sig_start_bit].receiver) - 1);
            }

            // 신호 출력
            // printf("\t신호 이름: %s | 시작 비트: %d | 길이: %d | 바이트오더: %d | 부호: %c | 팩터: %.2f | 오프셋: %.2f | 최소값: %g | 최대값: %g | 단위: %s | 수신ECU: %s\n",
            //        CAN_m[msg_id].CAN_s[sig_start_bit].name, CAN_m[msg_id].CAN_s[sig_start_bit].start_bit, CAN_m[msg_id].CAN_s[sig_start_bit].length,
            //        CAN_m[msg_id].CAN_s[sig_start_bit].byteorder, CAN_m[msg_id].CAN_s[sig_start_bit].sign, CAN_m[msg_id].CAN_s[sig_start_bit].factor,
            //        CAN_m[msg_id].CAN_s[sig_start_bit].offset, CAN_m[msg_id].CAN_s[sig_start_bit].min_val, CAN_m[msg_id].CAN_s[sig_start_bit].max_val,
            //        CAN_m[msg_id].CAN_s[sig_start_bit].unit, CAN_m[msg_id].CAN_s[sig_start_bit].receiver);
        }
    }

    fclose(dbc_file);
    return ecu_count;
}
