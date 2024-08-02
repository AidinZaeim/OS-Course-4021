# Makefile for Q2.c

CC = gcc
CFLAGS = -pthread
TARGET = Q2

all: $(TARGET)

$(TARGET): Q2.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
