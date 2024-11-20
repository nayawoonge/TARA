#ifndef UTIL_H  // 헤더 가드 시작
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 0x7FF
#define MAX_SIG_COUNTER 50

// 신호 구조체 정의
struct CAN_Signal {
    char name[32];  // 신호 이름
    int start_bit;  // 시작 비트
    int length;     // 신호 길이
    int byteorder;  // byte order (0: little endian, 1: big endian)
    char sign;      // 부호 (+: unsigned, -: signed)
    double factor;   // 스케일링 팩터
    double offset;   // 오프셋
    double min_val;  // 최소값
    double max_val;  // 최대값
    char unit[32];  // 단위
    char receiver[210]; // 수신 ECU
};

// 메시지 구조체 정의
struct CAN_Message{
    u_int8_t sig_counter;
    unsigned int id; // 메시지 ID
    char name[16];  // 메시지 이름
    unsigned int dlc;    // 데이터 길이 (DLC)
    char sender[40];    // 송신 노드
    struct CAN_Signal CAN_s[MAX_SIG_COUNTER];  // 최대 20개의 신호
};

// ECU 구조체 정의
struct ECU{
    char name[32];  // ECU 이름
};

#endif  // 헤더 가드 끝
