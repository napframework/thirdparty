CC = gcc
CFLAGS = -I../../common -Wall -Wextra -ansi -pedantic -std=c99 -lm -lpthread

UNAME = $(shell uname)

ifneq ($(UNAME), Darwin)
CFLAGS += -lrt
endif

etherdream.so: etherdream.c
	gcc $(CFLAGS) -shared -o libetherdream.so -fPIC etherdream.c

.PHONY: clean

clean:
	rm -rf etherdream.so etherdream.c.*
