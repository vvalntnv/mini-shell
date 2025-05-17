DEBUG := 0

CC := gcc
CFLAGS := -Wall -Wextra -Werror -Iinclude -g
LDFLAGS := -lreadline

SRC_DIR := src
OBJ_DIR := build
BIN := mini-shell


SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

ifeq ($(DEBUG),1)
  CFLAGS += -g -O0
endif


all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

debug:
	$(MAKE) DEBUG=1

.PHONY: all clean debug

