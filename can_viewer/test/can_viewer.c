#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#define MAX_DATA 50

typedef struct {
    int id;
    char data[64];  // CAN 데이터 내용을 저장
} CANData;

// CAN 데이터를 ID로 정렬하는 비교 함수
int compare(const void *a, const void *b) 
{
    CANData *data1 = (CANData *)a;
    CANData *data2 = (CANData *)b;
    return data1->id - data2->id;
}

// candump 명령어로부터 데이터를 수집
void fetch_can_data(FILE *fp, CANData *canData, int *count) 
{
    char buffer[128];
    int index = 0;

    // 파일 포인터로부터 데이터를 읽어서 파싱
    while ( (fgets(buffer, sizeof(buffer), fp) != NULL) && (index < MAX_DATA) ) 
    {
        int id;
        int data_length;
        char data[64];
        
        // 출력 예시 :  can1  <id>   [<length>]  <data>
        //  can1  3C1   [8]  65 00 00 00 00 00 00 00
        // [^\n] : \n이 올 때까지 모든 문자열
        if (sscanf(buffer, "  can1  %x   [%d]  %[^\n]", &id, &data_length, data) == 3) 
        {
            canData[index].id = id;
            strncpy(canData[index].data, data, sizeof(canData[index].data) - 1);
            canData[index].data[sizeof(canData[index].data) - 1] = '\0';
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
	    // CAN 데이터를 candump에서 가져오기
        fetch_can_data(fp, canData, &data_count);

        // 화면 클리어
        clear();
        mvprintw(0, 0, "CAN Data Monitor - Press 'q' to quit");

        int line = 2;
        // 정렬된 CAN 데이터를 화면에 출력
        for (int i = 0; i < data_count; i++) 
        {
            mvprintw(i + 2, 0, " CAN_ID: %03X : Data: %s", canData[i].id, canData[i].data);
        }

        refresh();
        
        usleep(16666); // 60frame (데이터 갱신 주기)
    }

    // ncurses 종료 및 파일 포인터 닫기
    endwin();
    pclose(fp);

    return 0;
}
