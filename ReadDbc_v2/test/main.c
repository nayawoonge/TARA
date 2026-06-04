#include <stdio.h>
#include <sys/stat.h> // mkdir() 함수 사용을 위해 필요
#include <errno.h>    // 오류 처리를 위해 필요

int main() {
    const char *dirName  = "/home/lisa/TARA/ReadDbc_copy/data/dot/test_dir";
    const char *fileName = "/home/lisa/TARA/ReadDbc_copy/data/dot/test_dir/my_file.txt";

    // 디렉토리 생성
    if (mkdir(dirName, 0777) == -1) {
        if (errno != EEXIST) {
            perror("디렉토리 생성 실패");
            return 1;
        }
    }

    // 파일 생성 및 작성
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        perror("파일 생성 실패");
        return 1;
    }

    // 파일에 내용 작성
    fprintf(file, "안녕하세요! 이 파일은 새로 생성된 디렉토리 안에 있습니다.\n");

    // 파일 닫기
    fclose(file);

    printf("디렉토리와 파일이 성공적으로 생성되었습니다.\n");
    return 0;
}
