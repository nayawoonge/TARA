#ifndef UTIL_H  // 헤더 가드 시작
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 0x7FF

// 신호 구조체 정의
struct CAN_Signal {
    char name[32];  // 신호 이름
    int start_bit;  // 시작 비트
    int length;     // 신호 길이
    int byteorder;  // byte order (0: little endian, 1: big endian)
    char sign;      // 부호 (+: unsigned, -: signed)
    float factor;   // 스케일링 팩터
    float offset;   // 오프셋
    float min_val;  // 최소값
    float max_val;  // 최대값
    char unit[8];  // 단위
    char receiver[50]; // 수신 ECU
};

// 메시지 구조체 정의
struct CAN_Message{
    unsigned int id; // 메시지 ID
    char name[32];  // 메시지 이름
    unsigned int dlc;    // 데이터 길이 (DLC)
    char sender[32];    // 송신 노드
    struct CAN_Signal CAN_s[20];  // 최대 20개의 신호
};

// ECU 구조체 정의
struct ECU{
    char name[32];  // ECU 이름
};

#endif  // 헤더 가드 끝
