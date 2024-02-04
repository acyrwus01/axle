CC = gcc
CFLAGS = -Wall -fPIC -pedantic -g
LIBS = -lpthread
SRC_DIR = .
OBJ_DIR = ./build
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
TARGET = axle
SHARED_TARGET = libaxle.so
STATIC_TARGET = libaxle.a

all: $(TARGET) $(SHARED_TARGET) $(STATIC_TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET) $(LIBS)

$(SHARED_TARGET): $(OBJ_FILES)
	$(CC) -shared $(CFLAGS) $(OBJ_FILES) -o $(SHARED_TARGET) $(LIBS)

$(STATIC_TARGET): $(OBJ_FILES)
	ar rcs $(STATIC_TARGET) $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean run time

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

time: $(TARGET)
	time -f 'Time: %E' sh -c "./$(TARGET)"
