# Tên chương trình
TARGET = build/bin/app

# Compiler và flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinc

# Thư mục
SRC_DIR = src
OBJ_DIR = build/obj
BIN_DIR = build/bin

# Nguồn: gồm main.c ở root + tất cả .c trong src/
SRCS = main.c $(wildcard $(SRC_DIR)/*.c)
# Biến đổi thành obj
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Rule mặc định
all: $(TARGET)

# Link thành file thực thi
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile từng file .c thành .o
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Dọn dẹp
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean

