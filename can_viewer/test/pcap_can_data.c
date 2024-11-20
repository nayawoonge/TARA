#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <pcap.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>

#define MAX_DATA 100

typedef struct {
    int id;
    char data[64];  // CAN 데이터 내용을 저장
} CANData;

// CAN 패킷을 캡처하고 출력하는 함수
void process_packet(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    struct can_frame *frame = (struct can_frame *)packet;
    int id = frame->can_id;
    char data[64];
    int data_len = frame->can_dlc;

    // CAN 데이터 파싱
    snprintf(data, sizeof(data), "%02X %02X %02X %02X %02X %02X %02X %02X",
             frame->data[0], frame->data[1], frame->data[2], frame->data[3],
             frame->data[4], frame->data[5], frame->data[6], frame->data[7]);

    // 화면 출력
    static int line = 2;  // 출력 줄 위치 (0번과 1번 줄은 타이틀로 사용)
    mvprintw(line, 0, "ID: %03X, Data: %s", id, data);
    line++;

    // 화면 하단에 도달하면 스크롤
    if (line >= LINES - 1) {
        line = 2;  // 다시 위로 돌아감
        clear();   // 화면 클리어
        mvprintw(0, 0, "CAN Data Monitor - Press 'q' to quit");
        refresh();
    }
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // CAN 인터페이스 열기
    handle = pcap_open_live("can1", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device can1: %s\n", errbuf);
        return 1;
    }

    // CAN 프로토콜의 경우 pcap 필터 사용 불가
    // 필터 설정 제거

    // ncurses 초기화
    initscr();
    noecho();
    cbreak();
    timeout(100); // 100ms 간격으로 입력 대기
    curs_set(0);  // 커서 숨기기

    // 초기 화면 구성
    mvprintw(0, 0, "CAN Data Monitor - Press 'q' to quit");

    // 메인 루프
    while (1) {
        // CAN 데이터를 캡처하여 처리
        pcap_dispatch(handle, 0, process_packet, NULL);

        refresh();

        // 'q' 키를 누르면 종료
        int ch = getch();
        if (ch == 'q') {
            break;
        }

        usleep(50000); // 50ms 지연 (데이터 갱신 주기)
    }

    // ncurses 종료 및 pcap 핸들 닫기
    endwin();
    pcap_close(handle);

    return 0;
}
