#include "impact_rating.h"

void rating_asset(const char *csv_file_dir, const char *default_output_file_path, const char *default_stride_file_name)
{
    FILE *intputF;
    FILE *outputF;
    char input_file_path[80];  // input 파일 경로를 저장할 버퍼
    char output_file_path[80];  // output 파일 경로를 저장할 버퍼

    sprintf(input_file_path, "%s%s", csv_file_dir, default_stride_file_name);   // 위협 시나리오 식별 csv 파일명
    sprintf(output_file_path, "%s%s", csv_file_dir, default_output_file_path);   // 영향 평가 csv파일명
    
    // 파일 열기
    intputF = fopen(input_file_path, "r");
    outputF = fopen(output_file_path, "w");

    // printf("%s\n", input_file_path);
    // printf("%s\n", output_file_path);
    if (intputF == NULL || outputF == NULL) 
    {
        perror("fopen error");
        exit(1);
    }
    else
    {
        printf("파일이 성공적으로 열렸습니다: %s\n", input_file_path);
        printf("파일이 성공적으로 열렸습니다: %s\n", output_file_path);
    }    
    
    char line[MAX_LINE_LENGTH];
    int threat_rate_safety, threat_rate_financial, threat_rate_operational, threat_rate_privacy;

    while(fgets(line, sizeof(line), intputF))
    {
        // 개행 문자 제거
        line[strcspn(line, "\n")] = 0;

        // line 복사본 생성
        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);

        // 구분자 0 : ecu 일 때
        if(strncmp(line, "0", 1) == 0)
        {
            strtok(line_copy, ","); // 복사본에서 첫 번째 열 제거
            char *second_column = strtok(NULL, ",");    // 복사본에서 두 번째 열 추출
            if (second_column != NULL) 
            {
                printf("\nECU: %s\n", second_column);
                printf("4 : Severe / 3 : Major / 2 : Moderate / 1 : Negligible\n");

                do {
                    printf("\nSafety : ");
                    scanf(" %d", &threat_rate_safety);
                    if (threat_rate_safety < 0 || threat_rate_safety > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_safety < 0 || threat_rate_safety > 4);

                do {
                    printf("\nFinancial : ");
                    scanf(" %d", &threat_rate_financial);
                    if (threat_rate_financial < 0 || threat_rate_financial > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_financial < 0 || threat_rate_financial > 4);

                do {
                    printf("\nOperational : ");
                    scanf(" %d", &threat_rate_operational);
                    if (threat_rate_operational < 0 || threat_rate_operational > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_operational < 0 || threat_rate_operational > 4);

                do {
                    printf("\nPrivacy : ");
                    scanf(" %d", &threat_rate_privacy);
                    if (threat_rate_privacy < 0 || threat_rate_privacy > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_privacy < 0 || threat_rate_privacy > 4);

                // 원본 line + threat_rate 파일에 출력
                fprintf(outputF, "%s,%d,%d,%d,%d\n", line, threat_rate_safety, threat_rate_financial, threat_rate_operational, threat_rate_privacy);
                continue;
            } 
            else 
            {
                break;
            }
        }

        // 구분자 1 : can message 일 때
        if(strncmp(line, "1", 1) == 0)
        {
            strtok(line_copy, ","); // 복사본에서 첫 번째 열 제거
            char *second_column = strtok(NULL, ",");    // 복사본에서 두 번째 열 추출
            if (second_column != NULL) 
            {
                printf("\nCAN 메시지 ID: %s\n", second_column);
                printf("4 : Severe / 3 : Major / 2 : Moderate / 1 : Negligible\n");

                do {
                    printf("\nSafety : ");
                    scanf(" %d", &threat_rate_safety);
                    if (threat_rate_safety < 0 || threat_rate_safety > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_safety < 0 || threat_rate_safety > 4);

                do {
                    printf("\nFinancial : ");
                    scanf(" %d", &threat_rate_financial);
                    if (threat_rate_financial < 0 || threat_rate_financial > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_financial < 0 || threat_rate_financial > 4);

                do {
                    printf("\nOperational : ");
                    scanf(" %d", &threat_rate_operational);
                    if (threat_rate_operational < 0 || threat_rate_operational > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_operational < 0 || threat_rate_operational > 4);

                do {
                    printf("\nPrivacy : ");
                    scanf(" %d", &threat_rate_privacy);
                    if (threat_rate_privacy < 0 || threat_rate_privacy > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_privacy < 0 || threat_rate_privacy > 4);

                // 원본 line + threat_rate 파일에 출력
                fprintf(outputF, "%s,%d,%d,%d,%d\n", line, threat_rate_safety, threat_rate_financial, threat_rate_operational, threat_rate_privacy);
                continue;
            }
            else
            {
                break;
            }
        }
        
        // 구분자 2 : 시그널 일 때
        if(strncmp(line, "2", 1) == 0)
        {
            strtok(line_copy, ","); // 복사본에서 첫 번째 열 제거
            char *second_column = strtok(NULL, ",");    // 복사본에서 두 번째 열 추출
            if (second_column != NULL) 
            {
                printf("\n시그널: %s\n", second_column);
                printf("4 : Severe / 3 : Major / 2 : Moderate / 1 : Negligible\n");

                do {
                    printf("\nSafety : ");
                    scanf(" %d", &threat_rate_safety);
                    if (threat_rate_safety < 0 || threat_rate_safety > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_safety < 0 || threat_rate_safety > 4);

                do {
                    printf("\nFinancial : ");
                    scanf(" %d", &threat_rate_financial);
                    if (threat_rate_financial < 0 || threat_rate_financial > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_financial < 0 || threat_rate_financial > 4);

                do {
                    printf("\nOperational : ");
                    scanf(" %d", &threat_rate_operational);
                    if (threat_rate_operational < 0 || threat_rate_operational > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_operational < 0 || threat_rate_operational > 4);

                do {
                    printf("\nPrivacy : ");
                    scanf(" %d", &threat_rate_privacy);
                    if (threat_rate_privacy < 0 || threat_rate_privacy > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (threat_rate_privacy < 0 || threat_rate_privacy > 4);

                // 원본 line + threat_rate 파일에 출력
                fprintf(outputF, "%s,%d,%d,%d,%d\n", line, threat_rate_safety, threat_rate_financial, threat_rate_operational, threat_rate_privacy);
                continue;
            } 
            else 
            {
                break;
            }
        }
    }

    fclose(intputF);
    fclose(outputF);
    return ;
}