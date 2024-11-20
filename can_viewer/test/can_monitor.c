#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>

// CAN 패킷을 캡처하고 출력하는 함수
void process_packet(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    struct can_frame *frame = (struct can_frame *)packet;

    // CAN 프레임 ID 및 데이터 길이 확인
    int id = frame->can_id;
    int data_len = frame->can_dlc;

    // CAN 데이터 출력
    printf("ID: %03X, DLC: %d, Data: ", id, data_len);
    for (int i = 0; i < data_len; i++) {
        printf("%02X ", frame->data[i]);
    }
    printf("\n");
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

    printf("Listening on can1... Press Ctrl+C to stop.\n");

    // 메인 루프
    while (1) {
        // CAN 데이터를 캡처하여 처리
        pcap_dispatch(handle, 0, process_packet, NULL);

        usleep(50000); // 50ms 지연 (데이터 갱신 주기)
    }

    // pcap 핸들 닫기
    pcap_close(handle);

    return 0;
}
