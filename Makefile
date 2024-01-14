CC = gcc
CFLAGS = -Wall
LIBS =  -lpthread -fPIC
OBJ_FILES = axle_itc.o main.o
TARGET = itc_example

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET) $(LIBS)

axle_itc.o: axle_itc.c axle_itc.h
	$(CC) $(CFLAGS) -c axle_itc.c -o axle_itc.o

main.o: test.c axle_itc.h
	$(CC) $(CFLAGS) -c test.c -o main.o

.PHONY: clean time run

clean:
	rm -f $(OBJ_FILES) $(TARGET)

run: $(TARGET)
	./$(TARGET)

time: $(TARGET)
	/usr/bin/time -f "Time taken: %E\nCPU Usage: %P\nMemory: %MKB" ./$(TARGET)
