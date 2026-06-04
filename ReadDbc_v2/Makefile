CC = gcc
INCLUDES = -Ilib
CFLAGS = -Wall -Wextra -g $(INCLUDES)
LDFLAGS = -lcgraph -lgvc
TARGET = ReadDbc
SRCS = src/main.c lib/parse_dbc_file.c lib/struct_print.c lib/stride.c lib/ecu_diagram.c lib/impact_rating.c lib/attack_feasibility.c lib/risk_det.c
OBJS = $(SRCS:.c=.o)
DEPS = lib/util.h lib/parse_dbc_file.h lib/struct_print.h lib/stride.h lib/ecu_diagram.h lib/impact_rating.h lib/attack_feasibility.h lib/risk_det.h

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean $(TARGET)

debug:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)
