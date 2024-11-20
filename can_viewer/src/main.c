#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/time.h>
#include <ncurses.h>

#include "util.h"

CANData canData[MAX_CAN_ID];  // CAN ID 별로 정적 선언

int main() 
{
  int s;
  struct sigaction sa;
  struct sockaddr_can addr;
  struct can_frame frame;
  struct ifreq ifr;
  struct msghdr msg;
  struct iovec iov;
  char ctrlmsg[CMSG_SPACE(sizeof(struct timeval))];
  struct cmsghdr *cmsg;
  struct timeval *tv, first_tv;
  int first_msg = 1;
  long time_diff_sec, time_diff_usec; // 타임스탬프 계산에 사용되는 변수
  long time_dump_sec, time_dump_usec;
  const char *ifname = "can1"; // 네트워크 인터페이스 이름
  
  // 소켓 생성
  if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) 
  {
    perror("Error while opening socket");
    return 1;
  }

  // 타임스탬프 옵션 활성화
  int enable = 1;
  if (setsockopt(s, SOL_SOCKET, SO_TIMESTAMP, &enable, sizeof(enable)) < 0) 
  {
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

  // bind()
  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
  {
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

  // ncurses 초기화
  initscr();
  start_color();
  use_default_colors();
  init_pair(1, COLOR_RED, -1);
  init_pair(2, COLOR_WHITE, -1);
  noecho();
  cbreak();
  timeout(100);
  curs_set(0);

  // 메시지 수신 대기
  while (1) 
  {
    // recv()
    if (recvmsg(s, &msg, 0) < 0) 
    {
      perror("can raw socket read");
      return 1;
    }

    // 타임스탬프 loop
    for (cmsg = CMSG_FIRSTHDR(&msg); cmsg && cmsg->cmsg_level == SOL_SOCKET; cmsg = CMSG_NXTHDR(&msg,cmsg)) 
    {
      if (cmsg->cmsg_type == SCM_TIMESTAMP) 
      {
        tv = (struct timeval *)CMSG_DATA(cmsg);

        if (first_msg) 
        {
          first_tv = *tv;
          first_msg = 0;
        }

        time_diff_sec = tv->tv_sec - first_tv.tv_sec;
        time_diff_usec = tv->tv_usec - first_tv.tv_usec;

        if (time_diff_usec < 0) 
        {
          time_diff_usec += 1000000;
          time_diff_sec -= 1;
        }
        // 타임스탬프 저장

        canData[frame.can_id].offset_time_sec = time_diff_sec  - canData[frame.can_id].previous_time_sec;
        canData[frame.can_id].offset_time_usec = time_diff_usec - canData[frame.can_id].previous_time_usec;

        // if(canData[frame.can_id].offset_time_sec < 0) canData[frame.can_id].offset_time_sec = ~canData[frame.can_id].offset_time_sec;
        if(canData[frame.can_id].offset_time_usec < 0) {
          canData[frame.can_id].offset_time_usec += 1000000;
          canData[frame.can_id].offset_time_sec -= 1;
        }
        canData[frame.can_id].offset_time_usec = canData[frame.can_id].offset_time_usec / 1000;

        canData[frame.can_id].previous_time_sec  = time_diff_sec;
        canData[frame.can_id].previous_time_usec = time_diff_usec;
        
        // printf("%ld.%06ld ", canData[frame.can_id].offset_time_sec, canData[frame.can_id].offset_time_usec);
        //sec : seconds, usec : micro seconds
        ++canData[frame.can_id].used;

      }
    }

    canData[frame.can_id].can_id = frame.can_id;
    canData[frame.can_id].can_dlc = frame.can_dlc;

    for (int i = 0; i<canData[frame.can_id].can_dlc ; i++)
    {
      canData[frame.can_id].data[i] = frame.data[i];
    }

    mvprintw(0, 0, "              ######### CAN Data Monitor ########                ");
    attron(COLOR_PAIR(1));
    mvprintw(1, 0, "| Time offset | canID | L | 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | Counter |");
    attroff(COLOR_PAIR(1));

    int line = 2;
    for (int id_index = 0 ; id_index < 0x800 ; id_index++)
    {
      if(canData[id_index].used)
      {
        mvprintw(line, 0, "%6ld.%03ld      0x%03X   %d   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X     %5d", 
			    canData[id_index].offset_time_sec, canData[id_index].offset_time_usec, canData[id_index].can_id, canData[id_index].can_dlc, 
			    canData[id_index].data[0], canData[id_index].data[1], canData[id_index].data[2], canData[id_index].data[3], 
			    canData[id_index].data[4], canData[id_index].data[5], canData[id_index].data[6], canData[id_index].data[7], canData[id_index].used);

        // printf("%ld.%06ld ", canData[id_index].can_time_sec, canData[id_index].can_time_usec);
        // printf("0x%03X [%d] ", canData[id_index].can_id, canData[id_index].can_dlc);
        // for (int i = 0; i < frame.can_dlc; i++)
        //   mvprintw(line, 0, "%02X ", frame.data[i]);
        // printf("\r\n");
        line++;
      }
    }
    usleep(10);
    refresh();
  }

  // ncurses 종료
  endwin();

  // 소켓 닫기
  close(s);
  return 0; 
}
