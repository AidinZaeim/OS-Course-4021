# Makefile for Q1.c

CC = gcc
CFLAGS = -pthread
TARGET = Q1

all: $(TARGET)

$(TARGET): Q1.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
