OUTFILE = gameTest
OUTDIR = $(HOME)/cmpt433/public/myApps
CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread
LFLAGS += -L/home/octavian13/cmpt433/public/curl_lib_BBG/ -lcurl

all:
	$(CC_C) $(CFLAGS) main.c -o $(OUTDIR)/$(OUTFILE) $(LFLAGS)
