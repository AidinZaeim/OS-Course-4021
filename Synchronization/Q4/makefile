# Makefile for Q4.c

CC = gcc
CFLAGS = -pthread
TARGET = Q4

all: $(TARGET)

$(TARGET): Q4.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
