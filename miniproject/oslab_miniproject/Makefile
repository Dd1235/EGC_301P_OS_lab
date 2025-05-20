# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread
SRC_DIR = src
OBJ_DIR = build

# Source files
net_utilsON_SRCS = $(SRC_DIR)/net_utils.c $(SRC_DIR)/auth_utils.c $(SRC_DIR)/password.c
SERVER_SRCS = $(net_utilsON_SRCS) \
              $(SRC_DIR)/admin_ops.c \
              $(SRC_DIR)/faculty_ops.c \
              $(SRC_DIR)/student_ops.c \
              $(SRC_DIR)/server.c

CLIENT_SRCS = $(SRC_DIR)/client.c

# Object files
SERVER_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SERVER_SRCS))
CLIENT_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CLIENT_SRCS))

# Targets
all: server client

# Server executable
server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Client executable
client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile all .c to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up binaries and object files
clean:
	rm -rf server client $(OBJ_DIR)

.PHONY: all clean
