#include "input_asset.h"

void input_asset(const char *csv_file_dir, const char *default_output_file_path, struct CAN_Message *CAN_m, struct ECU *ecu, int ecu_count)
{
    char file_name[20];  // 입력 파일 경로를 저장할 버퍼
    char file_path[80];  // 입력 파일 경로를 저장할 버퍼
    FILE *outputF;

    // 입력 파일 열기
    while (1) 
    {
        printf("출력 파일 경로 입력(default location [asset_importance.csv]): ");
        if (fgets(file_name, sizeof(file_name), stdin) == NULL) {
            printf("입력이 잘못되었습니다. 다시 시도해주세요.\n");
            continue;
        }

        // 개행 문자 제거
        file_name[strcspn(file_name, "\n")] = '\0';

        // 빈 입력일 경우 기본 경로로 설정
        if (strlen(file_name) == 0) {
            sprintf(file_path, "%s%s", csv_file_dir, default_output_file_path);   // 자산 중요도 csv파일명
        } else {
            sprintf(file_path, "%s%s", csv_file_dir, file_name);   // 자산 중요도 csv파일명
        }

        // 파일 열기
        outputF = fopen(file_path, "w");
        printf("%s\n", file_path);
        if (outputF == NULL) {
            perror("파일 열기 오류");
            printf("파일 경로가 잘못되었습니다. 다시 입력해주세요.\n");
            continue;
        }

        printf("파일이 성공적으로 열렸습니다: %s\n", file_path);
        break;  // 파일이 성공적으로 열리면 루프 종료
    }

    int confidentiality, availability, integrity;

    // ECU 중요도 입력
    printf("\n===============ECU 중요도 입력===============");
    for (int i = 0; i < ecu_count ; i++) {
        printf("\nECU: %s\n", ecu[i].name);
        printf("기밀성 (1-낮음, 2-중간, 3-높음): ");
        scanf("%d", &confidentiality);
        printf("가용성 (1-낮음, 2-중간, 3-높음): ");
        scanf("%d", &availability);
        printf("무결성 (1-낮음, 2-중간, 3-높음): ");
        scanf("%d", &integrity);
        // printf("%s", ecu[i].name);

        fprintf(outputF, "0,%s,%d,%d,%d,%d\n", ecu[i].name, confidentiality, availability, integrity, confidentiality + availability + integrity);
    }

    // CAN 메시지 중요도 입력
    printf("\n===============Message 중요도 입력===============");
    for (int i = 0; i < MAX_ID; i++) {
        if (CAN_m[i].id != 0 && CAN_m[i].id < 0x800) {
            printf("\nCAN 메시지 ID: 0x%X, Name: %s\n", CAN_m[i].id, CAN_m[i].name);
            printf("기밀성 (1-낮음, 2-중간, 3-높음): ");
            scanf("%d", &confidentiality);
            printf("가용성 (1-낮음, 2-중간, 3-높음): ");
            scanf("%d", &availability);
            printf("무결성 (1-낮음, 2-중간, 3-높음): ");
            scanf("%d", &integrity);

            fprintf(outputF, "1,0x%X,%s,%d,%s,%d,%d,%d,%d\n",
                    CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender, confidentiality, availability, integrity, confidentiality + availability + integrity);
        }
    }

    // 시그널 중요도 입력
    printf("\n===============Signal 중요도 입력===============");
    for (int i = 0; i < MAX_ID; i++) {
        if (CAN_m[i].id != 0 && CAN_m[i].id < 0x800) {
            for (int j = 0; j < 256; j++) {
                if (CAN_m[i].CAN_s[j].name[0] != '\0') {
                    printf("\n시그널: %s (ID: 0x%X)\n", CAN_m[i].CAN_s[j].name, CAN_m[i].id);
                    printf("기밀성 (1-낮음, 2-중간, 3-높음): ");
                    scanf("%d", &confidentiality);
                    printf("가용성 (1-낮음, 2-중간, 3-높음): ");
                    scanf("%d", &availability);
                    printf("무결성 (1-낮음, 2-중간, 3-높음): ");
                    scanf("%d", &integrity);

                    fprintf(outputF, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,%d,%d,%d,%d\n",
                            CAN_m[i].CAN_s[j].name, CAN_m[i].CAN_s[j].start_bit, CAN_m[i].CAN_s[j].length,
                            CAN_m[i].CAN_s[j].byteorder, CAN_m[i].CAN_s[j].sign, CAN_m[i].CAN_s[j].factor,
                            CAN_m[i].CAN_s[j].offset, CAN_m[i].CAN_s[j].min_val, CAN_m[i].CAN_s[j].max_val,
                            CAN_m[i].CAN_s[j].unit, CAN_m[i].CAN_s[j].receiver, confidentiality, availability, integrity,
                            confidentiality + availability + integrity);
                }
            }
        }
    }

    // 파일 닫기
    fclose(outputF);

    printf("\n새로운 파일이 저장되었습니다: %s\n", file_path);
}
