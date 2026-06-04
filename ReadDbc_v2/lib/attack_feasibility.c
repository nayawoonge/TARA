#include "attack_feasibility.h"

void attack_vector(const char *csv_file_dir, const char *default_output_file_path, const char *default_rating_file_name)
{
    FILE *inputF;
    FILE *outputF;

    char input_file_path[80];
    char output_file_path[80];

    sprintf(input_file_path, "%s%s", csv_file_dir, default_rating_file_name);
    sprintf(output_file_path, "%s%s", csv_file_dir, default_output_file_path);

    inputF = fopen(input_file_path, "r");
    outputF = fopen(output_file_path, "w");

    if(inputF == NULL || outputF == NULL)
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
    int attack_feasibility_rating;
    while(fgets(line, sizeof(line), inputF))
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
                printf("4 : High / 3 : Medium / 2 : Low / 1 : Very Low\n");

                do {
                    scanf(" %d", &attack_feasibility_rating);
                    if (attack_feasibility_rating < 0 || attack_feasibility_rating > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (attack_feasibility_rating < 0 || attack_feasibility_rating > 4);

                // 원본 line + threat_rate 파일에 출력
                fprintf(outputF, "%s,%d\n", line, attack_feasibility_rating);
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
                printf("4 : High / 3 : Medium / 2 : Low / 1 : Very Low\n");
                do {
                    scanf(" %d", &attack_feasibility_rating);
                    if (attack_feasibility_rating < 0 || attack_feasibility_rating > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (attack_feasibility_rating < 0 || attack_feasibility_rating > 4);

                // 원본 line + threat_rate 파일에 출력
                fprintf(outputF, "%s,%d\n", line, attack_feasibility_rating);
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
                printf("4 : High / 3 : Medium / 2 : Low / 1 : Very Low\n");

                do {
                    scanf(" %d", &attack_feasibility_rating);
                    if (attack_feasibility_rating < 0 || attack_feasibility_rating > 4) 
                    {
                        printf("잘못된 입력입니다. 0~4 사이의 값을 입력하세요.\n");
                    }
                } while (attack_feasibility_rating < 0 || attack_feasibility_rating > 4);

                // 원본 line + threat_rate 파일에 출력
                fprintf(outputF, "%s,%d\n", line, attack_feasibility_rating);
                continue;
            } 
            else 
            {
                break;
            }
        }
    }

    fclose(inputF);
    fclose(outputF);
    return ;
}