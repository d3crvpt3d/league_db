CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = -L./lib
LDLIBS = -lduckdb -lcjson

# For static linking, uncomment these and comment out the above LDLIBS
# LDLIBS = -l:libduckdb_static.a -lcjson -pthread -lstdc++

SRCS = src/json_parser.c
OBJS = $(SRCS:.c=.o)
TARGET = parser

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS) -Wl,-rpath,./lib

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=./lib ./$(TARGET)
