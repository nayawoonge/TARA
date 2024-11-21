#include "stride.h"

void stride_asset(const char *csv_file_dir, const char *default_output_file_path, struct CAN_Message *CAN_m, struct ECU *ecu, int ecu_count)
{
    char file_path[80];  // 파일 경로를 저장할 버퍼
    FILE *outputF;

    sprintf(file_path, "%s%s", csv_file_dir, default_output_file_path);   // 위협 시나리오 식별 csv파일명

    // 파일 열기
    outputF = fopen(file_path, "w");
    printf("%s\n", file_path);
    if (outputF == NULL) 
    {
        perror("파일 열기 오류");
        exit(1);
    }
    else
        printf("파일이 성공적으로 열렸습니다: %s\n", file_path);
    
    char threat_type ='\0';

    // ECU STRIDE 입력
    printf("\nS(Spoofing) / T(Tampering) / R(Repudiation) / I(Information Disclosure / D(Denial of Service) / E(Elevation of Privilege)\n");
    printf("===============ECU STRIDE 입력===============");
    for (int i = 0; i < ecu_count ; i++) 
    {
        // printf("ecu count %d", ecu_count);
        // printf("i = %d\n",i);
        printf("\nECU: %s\n", ecu[i].name);
        threat_type ='\0';
        do {
            scanf(" %c", &threat_type); // 공백 포함해서 입력 처리(이전 입력에서 남은 개행문자 제거)
            switch (threat_type)
            {
            case 's':
                fprintf(outputF, "0,%s,Spoofing\n", ecu[i].name);
                break;
            case 't':
                fprintf(outputF, "0,%s,Tampering\n", ecu[i].name);
                break;
            case 'r':
                fprintf(outputF, "0,%s,Repudiation\n", ecu[i].name);
                break;
            case 'i':
                fprintf(outputF, "0,%s,Information Disclosure\n", ecu[i].name);
                break;
            case 'd':
                fprintf(outputF, "0,%s,Denial of Service\n", ecu[i].name);
                break;
            case 'e':
                fprintf(outputF, "0,%s,Elevation of Privilege\n", ecu[i].name);
                break;
            default:
                printf("invalid input\n");
                threat_type = '\0';
            }
        } while(threat_type == '\0');
    }

    // CAN STRIDE 입력
    printf("\n===============Message STRIDE 입력===============");
    for (int i = 0; i < MAX_ID; i++) 
    {
        if (CAN_m[i].id != 0 && CAN_m[i].id < 0x800) 
        {
            printf("\nCAN 메시지 ID: 0x%X, Name: %s\n", CAN_m[i].id, CAN_m[i].name);
            threat_type ='\0';
            do {
                scanf(" %c", &threat_type);
                switch (threat_type)
                {
                case 's':
                    fprintf(outputF, "1,0x%X,%s,%d,%s,Spoofing\n", CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
                    break;
                case 't':
                    fprintf(outputF, "1,0x%X,%s,%d,%s,Tampering\n", CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
                    break;
                case 'r':
                    fprintf(outputF, "1,0x%X,%s,%d,%s,Repudiation\n", CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
                    break;
                case 'i':
                    fprintf(outputF, "1,0x%X,%s,%d,%s,Information Disclosure\n", CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
                    break;
                case 'd':
                    fprintf(outputF, "1,0x%X,%s,%d,%s,Denial of Service\n", CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
                    break;
                case 'e':
                    fprintf(outputF, "1,0x%X,%s,%d,%s,Elevation of Privilege\n", CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender);
                    break;
                default:
                    printf("invalid input\n");
                    threat_type ='\0';
                }
            } while (threat_type == '\0');
        }
    }

    // 시그널 STRIDE 입력
    printf("\n===============Signal STRIDE 입력===============");
    for (int i = 0; i < MAX_ID; i++) 
    {
        if (CAN_m[i].id != 0 && CAN_m[i].id < 0x800) 
        {
            for (int j = 0; j < 256; j++) {
                if (CAN_m[i].CAN_s[j].name[0] != '\0') 
                {
                    printf("\n시그널: %s (ID: 0x%X)\n", CAN_m[i].CAN_s[j].name, CAN_m[i].id);
                    threat_type ='\0';
                    do {
                        scanf(" %c", &threat_type);
                        switch (threat_type)
                        {
                        case 's':
                            fprintf(outputF, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,Spoofing\n", 
                                    CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                                    CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                                    CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                                    CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver);
                            break;
                        case 't':
                            fprintf(outputF, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,Tampering\n", 
                                    CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                                    CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                                    CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                                    CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver);
                            break;
                        case 'r':
                            fprintf(outputF, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,Repudiation\n", 
                                    CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                                    CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                                    CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                                    CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver);
                            break;
                        case 'i':
                            fprintf(outputF, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,Information Disclosure\n", 
                                    CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                                    CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                                    CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                                    CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver);
                            break;
                        case 'd':
                            fprintf(outputF, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,Denial of Service\n", 
                                    CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                                    CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                                    CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                                    CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver);
                            break;
                        case 'e':
                            fprintf(outputF, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,Elevation of Privilege\n", 
                                    CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                                    CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                                    CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                                    CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver);
                            break;
                        default:
                            printf("invalid input\n");
                            threat_type ='\0';
                        }
                    } while(threat_type == '\0');

                }
            }
        }
    }
    // 파일 닫기
    fclose(outputF);
    printf("\n새로운 파일이 저장되었습니다: %s\n", file_path);

    return ;
}
