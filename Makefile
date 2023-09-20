CC := gcc

CFLAGS := -Wall

INCLUDES := -I ./inc
INCLUDES += -I ./lib

LIBS := -lcurl
LIBS += -lssl
LIBS += -lcrypto

SRC_DIR := ./src
OBJ_DIR := ./obj
LIB_DIR := ./lib
BIN_DIR := ./

EXE_NAME := steam-guard

SRCS := $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
OBJS += $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(wildcard $(LIB_DIR)/*.c))
TARGET := $(BIN_DIR)/$(EXE_NAME)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) src/main.c -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -r $(OBJ_DIR) $(EXE_NAME)

.PHONY: all clean
