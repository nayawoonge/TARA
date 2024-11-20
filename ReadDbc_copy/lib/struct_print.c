#include "util.h"
#include "struct_print.h"

void struct_print(struct CAN_Message *CAN_m)
{
    // 구조체 안에 값들 확인용
    for (int i=0x000 ; i<0x800 ; i++)
    {
        if((CAN_m[i].id != 0) && CAN_m[i].id < 0x800)
        {
            printf("\n메시지 ID: 0x%X | 메시지 이름: %s | DLC: %d | 송신 노드: %s\n", CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
            for (int j=0 ; j < MAX_SIG_COUNTER; j++)
            {
                if (strlen(CAN_m[i].CAN_s[j].name) > 0)
                {
                    printf("\t신호 이름: %s | 시작 비트: %d | 길이: %d | 바이트오더: %d | 부호: %c | 팩터: %.2f | 오프셋: %.2f | 최소값: %g | 최대값: %g | 단위: %s | 수신ECU: %s\n", 
                        CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length, 
                        CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor, 
                        CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val, 
                        CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver);
                }
            }
        }
    }
}

void output_file(const char *csv_file_dir, const char *file_name, struct CAN_Message *CAN_m, struct ECU *ecu, int ecu_count)
{
    char filename[64] = {0,};
    sprintf(filename, "%s%s", csv_file_dir, file_name);
    printf("%s\n", filename);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("file open error");
        exit(1);
    }

     // ECU 정보 출력 (구분자 0)
    for (int i = 0; i < ecu_count ; i++)
    {
        fprintf(file, "0,%s\n", ecu[i].name);
    }

    // CAN 메시지와 신호 정보 출력
    // CAN 메시지 (구분자 1)
    for (int i = 0x000; i < 0x800; i++) 
    {
        if ((CAN_m[i].id != 0) && CAN_m[i].id < 0x800) 
        {
            // CAN_message 정보 앞에 구분자 '1'을 추가
            fprintf(file, "1,0x%X,%s,%d,%s\n", 
                    CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
            
            for (int j = 0; j < MAX_SIG_COUNTER; j++) 
            {
                if (strlen(CAN_m[i].CAN_s[j].name) > 0) 
                {
                    // receiver에서 ','를 ' '로 치환
                    char modified_receiver[210];
                    strncpy(modified_receiver, CAN_m[i].CAN_s[j].receiver, 209);
                    
                    // strlen은 항상 0 이상의 값을 반환하므로 부호 없는 정수(size_t)를 반환한다. 따라서 k를 size_t로 선언하여 두 값의 자료형이 동일하게 함
                    for (size_t k = 0; k < 210; k++) 
                    {
                        if (modified_receiver[k] == ',') 
                        {
                            modified_receiver[k] = ' ';
                        }
                    }

                    // CAN_signal 정보 앞에 구분자 '2'을 추가
                    fprintf(file, "2,%s,%d,%d,%d,%c,%lf,%lf,%lf,%lf,%s,%s\n",
                            CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                            CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                            CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                            CAN_m[i].CAN_s[j].unit, modified_receiver);
                }
            }
        }
    }

    fclose(file);
    printf("데이터가 %s 파일에 CSV 형식으로 저장되었습니다.\n", file_name);
}