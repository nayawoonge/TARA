#include <unistd.h>
#include <linux/can.h>

#define MAX_CAN_ID 0x800

typedef struct {
    u_int8_t    canDlc;
    u_int8_t    data[0x40][8];
    // __u8    data[8][0x40] __attribute__((aligned(8)));
    int used_count;
} CANData;