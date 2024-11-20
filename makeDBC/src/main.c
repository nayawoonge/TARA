#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int main(int argc, char* argv[]) {

    int n = 0;
    FILE *trcFilePointer, *dbcFilePointer;
    char trcFileLocation[40];
    char trcFileLine[70];
    canid_t canID;
    // CANData canDatas[MAX_CAN_ID] = {0}; // 과도한 메모리 점유 방지
    CANData* canDatas = (CANData*)calloc(MAX_CAN_ID, sizeof(CANData));
    if (canDatas == NULL) {
        perror("[Err] Clear allocation Error");
        return -1;
    }


    // trc 파일 위치 입력
    if(argc == 1) {
        printf("Trc File Location : ");
        scanf("%39s", trcFileLocation);
    } else {
        strncpy(trcFileLocation, argv[1], sizeof(trcFileLocation) - 1);
    }

    // trc 파일 로드
    trcFilePointer = fopen(trcFileLocation, "r");
    if(trcFilePointer == NULL) {
        perror("[Err] Trc file open Error");
        return -1;
    }

    // dbc 파일 생성
    dbcFilePointer = fopen("/home/lisa/TARA/makeDBC/output/outputDBC.dbc", "w");
    if(dbcFilePointer == NULL) {
        perror("[Err] DBC File Open Error");
        return -1;
    }
    printf("[Initialize done]\n");

    for(int i = 0; i < 18 ; i++) fgets(trcFileLine, sizeof(trcFileLine), trcFilePointer); // trc file의 주석 건너뛰기

    while(fgets(trcFileLine, sizeof(trcFileLine), trcFilePointer)) {
        sscanf(trcFileLine, "%*d) %*lf Rx %x %*d %*hhx %*hhx %*hhx %*hhx %*hhx %*hhx %*hhx %*hhx", &canID);
        printf("0x%3X \n", canID);
        printf("%d\n", canDatas[canID].used_count);

        printf("%s", trcFileLine);
        sscanf(trcFileLine, "%*d) %*lf Rx %*x %d %2hhx %2hhx %2hhx %2hhx %2hhx %2hhx %2hhx %2hhx", canDatas[canID].canDlc, 
                &canDatas[canID].data[canDatas[canID].used_count][0], &canDatas[canID].data[canDatas[canID].used_count][1],
                &canDatas[canID].data[canDatas[canID].used_count][2], &canDatas[canID].data[canDatas[canID].used_count][3],   
                &canDatas[canID].data[canDatas[canID].used_count][4], &canDatas[canID].data[canDatas[canID].used_count][5],   
                &canDatas[canID].data[canDatas[canID].used_count][6], &canDatas[canID].data[canDatas[canID].used_count][7] );  

        canDatas[canID].used_count++;
        if(canDatas[canID].used_count > 0x39) {
            printf("overflow\n");
            return 0;
        }
    }

    free(canDatas);
    fclose(trcFilePointer);
    fclose(dbcFilePointer);
    return 0;
}