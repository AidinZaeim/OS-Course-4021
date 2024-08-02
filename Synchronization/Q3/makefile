# Makefile for Q3.c

CC = gcc
CFLAGS = -pthread
TARGET = Q3

all: $(TARGET)

$(TARGET): Q3.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET) ./source_file.txt ./destination_file.txt

clean:
	rm -f $(TARGET)
