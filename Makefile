OUTFILE = gameTest
CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread -I${CURDIR}/curl_bbg -I${CURDIR}/jsonc_bbg

LFLAGS = -L${CURDIR}/curl_bbg/libs/ -L${CURDIR}/jsonc_bbg/libs/  -lcurl  -ljson-c

all:
	$(CC_C) $(CFLAGS) main.c -o $(OUTFILE) $(LFLAGS)
