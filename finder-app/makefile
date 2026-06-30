
CROSS_COMPILE ?=

SRC_DIR = .
INC_DIR = .
APP_DIR = .

SRCS = $(wildcard $(SRC_DIR)/*.c)
INCLUDES = -I$(INC_DIR)

OBJS = $(SRCS:.c=.o)

APP = $(APP_DIR)/writer

CC = $(CROSS_COMPILE)gcc

CFLAGS = -g -Wall

%.o : %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

.DEFAULT_GOAL := all

.PHONY: all
all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(APP)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(APP)

