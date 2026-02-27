CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2 -g
LDFLAGS = -pthread

SRCDIR = ./src
SOURCES = $(wildcard $(SRCDIR)/*.c)
TARGET = web-server.out

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

debug: CFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)
