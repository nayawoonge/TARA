#include "util.h"
#include "parse_dbc_file.h"
#include "struct_print.h"
#include "stride.h"
#include "ecu_diagram.h"
#include "impact_rating.h"
#include "risk_det.h"

int main() {   
    printf("[process start]\n");
    // 전역 변수를 선언 (정의는 util.h)
    struct CAN_Message CAN_m[MAX_ID] = {0};  // CAN 메시지 배열
    struct ECU ecu[300] = {0};               // ECU 배열

    int ecu_count = 0;

    // DBC 파일 경로 설정
    // const char *dbc_file_path = "/home/lisa/TARA/ReadDbc_copy/data/dbc/turn_light.dbc";
    const char *dbc_dir_path = "/home/lisa/TARA/ReadDbc_copy/data/dbc/";
    const char *dbc_file_name = "turn_light.dbc";
    // const char *dbc_file_name = "Passenger_Cars_V1.9.2.dbc";
    char dbc_file_path[60] = {0,};

    sprintf(dbc_file_path, "%s%s", dbc_dir_path, dbc_file_name);
    printf("%s\n", dbc_file_path);

    // DBC 파일 파싱
    printf("[DBC 파일 파싱]\n");
    // ecu의 개수를 반환
    ecu_count = parse_dbc_file(dbc_file_path, CAN_m, ecu);

    // DBC 다이어그램 출력
    printf("[DBC 다이어그램 출력]\n");
    const char *dot_dir_path = "/home/lisa/TARA/ReadDbc_copy/data/dot/";
    char dot_file_dir[60] = {0,};  
    
    sprintf(dot_file_dir, "%s%s", dot_dir_path, dbc_file_name);

    dot_file_dir[strlen(dot_file_dir)-4] = '/';
    dot_file_dir[strlen(dot_file_dir)-3] = '\0';
    printf("%s\n", dot_file_dir);

    export_ECU_Diagram(dot_file_dir, CAN_m);

    // 파싱 후 구조체 데이터 출력 테스트
    printf("[구조체 데이터 출력]");
    struct_print(CAN_m);

    const char *csv_dir_path = "/home/lisa/TARA/ReadDbc_copy/data/csv/";
    char csv_file_dir[60] = {0,};

    sprintf(csv_file_dir, "%s%s", csv_dir_path, dbc_file_name);
    csv_file_dir[strlen(csv_file_dir)-4] = '/';
    csv_file_dir[strlen(csv_file_dir)-3] = '\0';

    // 디렉토리 생성
    if (mkdir(csv_file_dir, 0777) == -1) {
        if (errno != EEXIST) {
            perror("[디렉토리 생성 실패]");
            return 1;
        }
    }

    // 1 자산 식별
    const char *default_identify_file_name = "asset_identify.csv";  // 자산 식별 csv파일명
    identify_asset(csv_file_dir, default_identify_file_name, CAN_m, ecu, ecu_count);

    // 버스 모니터링
    // code

    // 2 위협 시나리오 식별
    const char *default_stride_file_name = "asset_stride.csv";   // 위협 시나리오 식별 csv파일명
    stride_asset(csv_file_dir, default_stride_file_name, CAN_m, ecu, ecu_count);

    // 3 영향 평가
    const char *default_rating_file_name = "asset_rating.csv";  // 영향 평가 csv파일명
    rating_asset(csv_file_dir, default_rating_file_name, default_stride_file_name);

    // 4 공격 경로 분석
    // tree code

    // 5 공격 실현 가능성 평가
    const char *default_feasibility_file_name = "attack_feasibility.csv";  // 공격 실현 가능성 평가 csv파일명
    attack_vector(csv_file_dir, default_feasibility_file_name, default_rating_file_name);

    // 6 위험도 평가
    const char *default_risk_file_name = "risk_determination.csv";  // 위험도 평가 csv파일명
    risk_determination(csv_file_dir, default_risk_file_name, default_feasibility_file_name);

    // 7 위험 처리 결정
    // 4가지 위험 처리 옵션 중 선택

    printf("[process finish]\n");
    return 0;
}
