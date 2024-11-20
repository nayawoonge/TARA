# 컴파일러 설정
CC = gcc
CFLAGS = -Wall -Wextra -g

# 실행 파일 이름
TARGET = ReadDbc

# 소스 파일 및 오브젝트 파일 정의
SRCS = main.c parse_dbc_file.c struct_print.c input_asset.c
OBJS = $(SRCS:.c=.o)

# 헤더 파일 정의
DEPS = util.h parse_dbc_file.h struct_print.h input_asset.h

# 기본 타겟(실행 파일 생성)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# 소스 파일 컴파일
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

# 클린 타겟
clean:
	rm -f $(OBJS) $(TARGET)

# 리빌드 타겟 (클린 후 재빌드)
rebuild: clean $(TARGET)

# 디버그용
debug:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)
