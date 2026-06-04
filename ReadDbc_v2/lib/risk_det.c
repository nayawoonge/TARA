#include "risk_det.h"

const char *determine_risk_value(int sum)
{
    if (sum >= 15 && sum <= 16) return "Very High";
    if (sum >= 12 && sum <= 14) return "High";
    if (sum >= 8 && sum <= 11) return "Medium";
    if (sum >= 5 && sum <= 7) return "Low";
    return "Very Low";
}

void risk_determination(const char *csv_dir_dir, const char *default_output_file_path, const char *default_feasibility_file_name)
{
    FILE *inputF;
    FILE *outputF;

    char input_file_path[80];
    char output_file_path[80];

    sprintf(input_file_path, "%s%s", csv_dir_dir, default_feasibility_file_name);
    sprintf(output_file_path, "%s%s", csv_dir_dir, default_output_file_path);

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

    while (fgets(line, sizeof(line), inputF)) 
    {
        // 개행 문자 제거
        line[strcspn(line, "\n")] = 0;

        // line 복사본 생성
        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);

        // 마지막 5개의 숫자를 합산
        int numbers[5] = {0};
        char *token = strtok(line_copy, ",");
        char *last_tokens[5] = {NULL};  // 마지막 5개의 포인터 저장
        int token_count = 0;

        // 모든 토큰을 순회하면서 마지막 5개의 값을 추적
        while (token != NULL) 
        {
            if (token_count >= 5) 
            {
                for (int i = 0; i < 4; i++) 
                {
                    last_tokens[i] = last_tokens[i + 1];
                }
                last_tokens[4] = token;
            } 
            else 
            {
                last_tokens[token_count] = token;
            }
            token_count++;
            token = strtok(NULL, ",");
        }

        // 마지막 5개의 토큰 합산
        int sum = 0;
        for (int i = 0; i < 5; i++) 
        {
            if (last_tokens[i] != NULL) 
            {
                numbers[i] = atoi(last_tokens[i]);
                sum += numbers[i];
            }
        }

        // 출력
        const char *risk_value = determine_risk_value(sum);
        fprintf(outputF, "%s,%d,%s\n", line, sum, risk_value);

        
    }
    fclose(inputF);
    fclose(outputF);
    return ;
}