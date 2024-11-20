#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/time.h>
#include <ncurses.h>

int main() {
    int s;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;
    struct msghdr msg;
    struct iovec iov;
    char ctrlmsg[CMSG_SPACE(sizeof(struct timeval))];
    struct cmsghdr *cmsg;
    struct timeval *tv, first_tv;
    int first_msg = 1;
    long time_diff_sec, time_diff_usec;
    const char *ifname = "can1"; // Set network interface
    
    // 소켓 생성
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket");
        return 1;
    }

    // 타임스탬프 옵션 활성화
    int enable = 1;
    if (setsockopt(s, SOL_SOCKET, SO_TIMESTAMP, &enable, sizeof(enable)) < 0) {
        perror("setsockopt SO_TIMESTAMP");
        return 1;
    }

    // 인터페이스 이름 설정
    strncpy(ifr.ifr_name, ifname, 5);
    ioctl(s, SIOCGIFINDEX, &ifr);

    // 소켓에 주소 할당
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return 1;
    }

    // 메시지 수신 준비
    iov.iov_base = &frame;
    iov.iov_len = sizeof(frame);
    msg.msg_name = &addr;
    msg.msg_namelen = sizeof(addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &ctrlmsg;
    msg.msg_controllen = sizeof(ctrlmsg);
    msg.msg_flags = 0;

    // 메시지 수신 대기
    while (1) {
        if (recvmsg(s, &msg, 0) < 0) {
            perror("can raw socket read");
            return 1;
        }

        for (cmsg = CMSG_FIRSTHDR(&msg); cmsg && cmsg->cmsg_level == SOL_SOCKET; cmsg = CMSG_NXTHDR(&msg,cmsg)) {
            if (cmsg->cmsg_type == SCM_TIMESTAMP) {
                tv = (struct timeval *)CMSG_DATA(cmsg);
                if (first_msg) {
                    first_tv = *tv;
                    first_msg = 0;
                }
                time_diff_sec = tv->tv_sec - first_tv.tv_sec;
                time_diff_usec = tv->tv_usec - first_tv.tv_usec;
                if (time_diff_usec < 0) {
                    time_diff_usec += 1000000;
                    time_diff_sec -= 1;
                }
                printf("%ld.%06ld ", time_diff_sec, time_diff_usec);
                //sec : seconds, usec : micro seconds
            }
        }

        printf("0x%03X [%d] ", frame.can_id, frame.can_dlc);
        for (int i = 0; i < frame.can_dlc; i++)
            printf("%02X ", frame.data[i]);
        printf("\r\n");
    }

    // 소켓 닫기
    close(s);
    return 0;
}