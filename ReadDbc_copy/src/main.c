#include "util.h"
#include "parse_dbc_file.h"
#include "struct_print.h"
#include "input_asset.h"
#include "ecu_diagram.h"

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

    // 구조체 데이터 파일 쓰기
    const char *default_identify_file_name = "asset_identify.csv";  // 자산 식별 csv파일명
    output_file(csv_file_dir, default_identify_file_name, CAN_m, ecu, ecu_count);

    // 자산식별 후 자산의 중요도 입력하기
    const char *default_importance_file_name = "asset_importance.csv";   // 자산 중요도 csv파일명
    input_asset(csv_file_dir, default_importance_file_name, CAN_m, ecu, ecu_count);

    printf("[process finish]\n");
    return 0;
}
