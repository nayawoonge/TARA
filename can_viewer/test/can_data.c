#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <ncurses.h>
#include <string.h>

#define MAX_DATA 50

typedef struct {
    int id;
    char data[64];  // CAN 데이터 내용을 저장
    int used;       // ID가 사용되고 있는지 표시
    char last_data[64]; // 이전 CAN 데이터를 저장하여 변경 확인
} CANData;

// candump 명령어로부터 데이터를 수집 및 업데이트
void fetch_can_data(FILE *fp, CANData *canData, int *count)
{
    int index = 0;
    char buffer[128];

    // 파일 포인터로부터 데이터를 읽어서 파싱
    while ((fgets(buffer, sizeof(buffer), fp) != NULL) && (index < MAX_DATA))
    {
        int id;
        int data_length;
        char data[64];

        // candump 출력 형식: can1 <id> [<length>] <data>
        // 예: can1 3C1 [8] 65 00 00 00 00 00 00 00
        if (sscanf(buffer, " can1 %x [%d] %[^\n]", &id, &data_length, data) == 3) 
        {
            if (canData[id].used) 
            {
                // 이미 사용된 ID인 경우, 데이터가 달라졌는지 확인
                if (strcmp(canData[id].last_data, data) != 0) 
                {
                    // 데이터가 달라졌을 때만 업데이트
                    strncpy(canData[id].data, data, sizeof(canData[id].data) - 1);
                    canData[id].data[sizeof(canData[id].data) - 1] = '\0';
                    // 마지막 데이터를 업데이트
                    strncpy(canData[id].last_data, data, sizeof(canData[id].last_data) - 1);
                    canData[id].last_data[sizeof(canData[id].last_data) - 1] = '\0';
                }
            } 
            else 
            {
                // 처음 수신된 ID일 경우
                strncpy(canData[id].data, data, sizeof(canData[id].data) - 1);
                canData[id].data[sizeof(canData[id].data) - 1] = '\0';
                // 마지막 데이터를 초기화
                strncpy(canData[id].last_data, data, sizeof(canData[id].last_data) - 1);
                canData[id].last_data[sizeof(canData[id].last_data) - 1] = '\0';
                canData[id].id = id;
                canData[id].used = 1;  // 해당 ID가 사용되었음을 표시
                
            }
            index++;
        }
    }
    *count = index;
}

int main() 
{
    CANData canData[MAX_DATA];
    int data_count = 0;

    // candump 명령어 실행
    FILE *fp = popen("candump can1", "r");
    if (fp == NULL) 
    {
        perror("Failed to run candump\n");
        return 1;
    }
    // ncurses 초기화
    initscr();
    noecho();
    cbreak();
    timeout(100); // 100ms 간격으로 입력 대기
    curs_set(0);  // 커서 숨기기
 
    // 메인 루프
    while (1) 
    {
        // CAN 데이터를 candump에서 가져오기 및 업데이트
        fetch_can_data(fp, canData, &data_count);

        // 화면 클리어
        clear();
        
        int line = 2;
        mvprintw(0, 0, "CAN Data Monitor - Press 'q' to quit");
        mvprintw(1, 0, "CAN-ID   Data");

        // CAN ID가 사용된 항목만 출력
        for (int i = 0; i < data_count; i++) 
        {
            if (canData[i].used) 
            {
                mvprintw(line, 0, "%03Xh     %s", canData[i].id, canData[i].data);
                line++;
            }
        }

        refresh();

        usleep(16666); // 60frame (데이터 갱신 주기)
    }

    // ncurses 종료 및 파일 포인터 닫기
    endwin();
    pclose(fp);

    return 0;
}
