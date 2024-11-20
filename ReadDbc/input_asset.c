#include "input_asset.h"

void input_asset(const char *output_file_path, struct CAN_Message *CAN_m, struct ECU *ecu, int ecu_count)
{
    char filename[256];  // 입력 파일 경로를 저장할 버퍼
    FILE *input_file, *output_file;

    // 입력 파일 열기
    while (1) 
    {
        printf("파일 경로를 입력하세요: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            printf("입력이 잘못되었습니다. 다시 시도해주세요.\n");
            continue;
        }

        // 개행 문자 제거
        filename[strcspn(filename, "\n")] = 0;

        // 빈 입력일 경우 다시 요청
        if (strlen(filename) == 0) {
            printf("빈 입력입니다. 다시 시도해주세요.\n");
            continue;
        }

        // 파일 열기
        input_file = fopen(filename, "r");
        if (input_file == NULL) {
            perror("파일 열기 오류");
            printf("파일 경로가 잘못되었습니다. 다시 입력해주세요.\n");
            continue;
        }

        break;  // 파일이 성공적으로 열리면 루프 종료
    }

    printf("파일이 성공적으로 열렸습니다: %s\n", filename);

    // 출력 파일 열기
    output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        perror("출력 파일 열기 오류");
        fclose(input_file);
        exit(1);
    }

    int confidentiality, availability, integrity;
    char line[512];

    // ECU 중요도 입력
    for (int i = 0; i < ecu_count ; i++) {
        printf("\nECU: %s\n", ecu[i].name);
        printf("기밀성 (1-낮음, 2-중간, 3-높음): ");
        scanf("%d", &confidentiality);
        printf("가용성 (1-낮음, 2-중간, 3-높음): ");
        scanf("%d", &availability);
        printf("무결성 (1-낮음, 2-중간, 3-높음): ");
        scanf("%d", &integrity);
        // printf("%s", ecu[i].name);

        fprintf(output_file, "0,%s,%d,%d,%d,%d\n", ecu[i].name, confidentiality, availability, integrity, confidentiality + availability + integrity);
    }

    // CAN 메시지 중요도 입력
    for (int i = 0; i < MAX_ID; i++) {
        if (CAN_m[i].id != 0 && CAN_m[i].id < 0x800) {
            printf("\nCAN 메시지 ID: 0x%X, Name: %s\n", CAN_m[i].id, CAN_m[i].name);
            printf("기밀성 (1-낮음, 2-중간, 3-높음): ");
            scanf("%d", &confidentiality);
            printf("가용성 (1-낮음, 2-중간, 3-높음): ");
            scanf("%d", &availability);
            printf("무결성 (1-낮음, 2-중간, 3-높음): ");
            scanf("%d", &integrity);

            fprintf(output_file, "1,0x%X,%s,%d,%s,%d,%d,%d,%d\n",
                    CAN_m[i].id, CAN_m[i].name, CAN_m[i].dlc, CAN_m[i].sender, confidentiality, availability, integrity, confidentiality + availability + integrity);
        }
    }

    // 시그널 중요도 입력
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

                    fprintf(output_file, "2,%s,%d,%d,%d,%c,%f,%f,%f,%f,%s,%s,%d,%d,%d,%d\n",
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
    fclose(input_file);
    fclose(output_file);

    printf("\n새로운 파일이 저장되었습니다: %s\n", output_file_path);
}
