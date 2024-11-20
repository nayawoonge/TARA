#include "util.h"
#include "parse_dbc_file.h"
#include "struct_print.h"
#include "input_asset.h"

// // 전역 변수를 선언 (정의는 util.h)
// CAN_Message CAN_m[MAX_ID];  // CAN 메시지 배열
// ECU ecu[300];               // ECU 배열
int main() 
{   
    // 전역 변수를 선언 (정의는 util.h)
    struct CAN_Message CAN_m[MAX_ID];  // CAN 메시지 배열
    struct ECU ecu[300] = {0};               // ECU 배열

    int ecu_count = 0;

    // DBC 파일 경로 설정
    const char *dbc_file_path = "/home/lisa/TARA/ReadDbc/turn_light.dbc";
    //const char *dbc_file_path = "Passenger Cars_V1.9.2.dbc";
    
    // 구조체 값 테스트
    // printf("구조체에 데이터 입력 전 구조체 값 출력 테스트\n");
    // struct_print(CAN_m);

    // DBC 파일 파싱
    printf("DBC 파일 파싱\n");
    ecu_count = parse_dbc_file(dbc_file_path, CAN_m, ecu);

    // printf("\necu_count:%d\n",ecu_count);

    // 파싱 후 구조체 데이터 출력 테스트
    printf("\n---------------------\n");
    printf("구조체 데이터 출력\n");
    printf("---------------------\n");
    struct_print(CAN_m);

    // 구조체 데이터 파일 쓰기
    // csv 형태로 저장
    const char *input_file = "asset_identify.csv";  // 자산 식별 csv파일명
    output_file(input_file, CAN_m, ecu, ecu_count);

    // 자산식별 후 자산의 중요도 입력하기
    const char *output_file = "asset_importance.csv";   // 자산 중요도 csv파일명
    input_asset(output_file, CAN_m, ecu, ecu_count);

    return 0;
}
