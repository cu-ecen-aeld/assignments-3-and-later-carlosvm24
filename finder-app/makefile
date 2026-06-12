
SRC_DIR = .
INC_DIR = .
APP_DIR = .

SRCS = $(wildcard $(SRC_DIR)/*.c)
INCLUDES = -I$(INC_DIR)

OBJS = $(SRCS:.c=.o)

APP = $(APP_DIR)/writer

CC = gcc

CFLAGS = -g -Wall

ifdef CROSS_COMPILE
	CC = $(CROSS_COMPILE)
endif

%.o : %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

.PHONY: make
make: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(APP)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(APP)

