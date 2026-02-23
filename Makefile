CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lncurses -lsqlite3

SRCS = main.c db_manager.c ui_manager.c
OBJS = $(SRCS:.c=.o)
TARGET = phone_contract_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) phone_contracts.db


.PHONY: all clean
