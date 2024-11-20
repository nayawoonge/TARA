#include <unistd.h>
#include <linux/can.h>

#define MAX_CAN_ID 0x800  // 0x000~0x7FF

typedef struct 
{
    canid_t can_id;  // 32bit CAN ID
    __u8    can_dlc;  // 데이터 길이
	__u8    data[CAN_MAX_DLEN] __attribute__((aligned(8))); // 데이터

    long previous_time_sec; // 타임스탬프 seconds
    long previous_time_usec; // 타임스탬프 micro seconds

    long offset_time_sec;
    long offset_time_usec;

    int used; // CAN ID 수신 여부
}CANData;
 