#include "ecu_diagram.h"

int export_ECU_Diagram(const char *default_dot_dir_path, struct CAN_Message *CAN_m) {
    Agraph_t *g[MAX_ID];
    for (int i = 0; i < MAX_ID; i++) {
        g[i] = NULL;
    }

    Agnode_t *sender = NULL;

    Agnode_t *n[50];
    Agedge_t *e[50];
    for (int i = 0; i < 50; i++) {
        n[i] = NULL;
        e[i] = NULL;
    }

    GVC_t *gvc;

    // 디렉토리 생성
    if (mkdir(default_dot_dir_path, 0777) == -1) {
        if (errno != EEXIST) {
            perror("[디렉토리 생성 실패]");
            return 1;
        }
    }

    for(int i = 0; i < MAX_ID; i++) {
        gvc = gvContext();
        if(CAN_m[i].id > 0 && CAN_m[i].id <= MAX_ID) {
            printf("LOG : ID %03X\n", i);
            g[i] = agopen("ECU Network", Agdirected, NULL);

            sender = agnode(g[i], CAN_m[i].sender, 1);

            for(int j = 0; j < MAX_SIG_COUNTER; j++) {
                if(strlen(CAN_m[i].CAN_s[j].name) > 0) {
                    n[j] = agnode(g[i], CAN_m[i].CAN_s[j].receiver, 1);
                    e[j] = agedge(g[i], sender, n[j], CAN_m[i].CAN_s[j].name, 1);
                    agsafeset(e[j], "label", CAN_m[i].CAN_s[j].name, "");
                }
            }

            // 초기화 후 메세지별로 파일 포인터 생성
            FILE *output_dot_file = NULL;
            FILE *output_svg_file = NULL;

            char dot_file_path[MAX_DIR_PATH_LEN] = {0,};
            char svg_file_path[MAX_DIR_PATH_LEN] = {0,};

            sprintf(dot_file_path, "%s0x%03X_%s.dot", default_dot_dir_path, i, CAN_m[i].name);
            sprintf(svg_file_path, "%s0x%03X_%s.svg", default_dot_dir_path, i, CAN_m[i].name);

            printf("%s\n", dot_file_path);
            printf("%s\n", svg_file_path);

            output_dot_file = fopen(dot_file_path, "w");
            agwrite(g[i], output_dot_file);
            fclose(output_dot_file);

            printf("dot file done\n");



            gvLayout(gvc, g[i], "dot");
            output_svg_file = fopen(svg_file_path, "w");
            gvRender(gvc, g[i], "svg", output_svg_file);
            fclose(output_svg_file);
            
            printf("svg file done\n");
            
            gvFreeLayout(gvc, g[i]);
            agclose(g[i]);
            gvFreeContext(gvc);

            // break; // test과정동안은 첫 메세지에 대한 다이어그램만 출력
        }
    }
    return 0;
}