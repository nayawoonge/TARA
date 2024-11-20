#include <stdio.h>
#include <graphviz/cgraph.h>

int main() {
    Agraph_t *g;
    Agnode_t *n, *m;
    Agedge_t *e;
    FILE *fp;

    // 새로운 그래프 생성
    g = agopen("G", Agdirected, NULL);
    n = agnode(g, "Node1", 1);
    m = agnode(g, "Node2", 1);
    e = agedge(g, n, m, NULL, 1);

    // 그래프를 파일로 출력
    fp = fopen("output/output.dot", "w");
    agwrite(g, fp);
    fclose(fp);

    agclose(g);
    return 0;
}
