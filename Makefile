OUTFILE = gameTest
#OUTDIR = $(HOME)/cmpt433/public/myApps
CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
#CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread -I/home/octavian13/cmpt433/work/test/libs/curl-7.56.1/include

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread -I/home/octavian13/cmpt433/public/curl_lib_BBG

#LFLAGS += -L/home/octavian13/cmpt433/public/curl_lib_BBG/ -lcurl
#LFLAGS = -L/home/octavian13/cmpt433/work/test/libs/curl-7.56.1/lib/.libs/ -lcurl
LFLAGS = -L/home/octavian13/cmpt433/public/curl_lib_BBG/libs/ -lcurl

#/home/octavian13/cmpt433/work/test/libs/curl-7.56.1/lib/.libs
all:
	$(CC_C) $(CFLAGS) main.c -o $(OUTFILE) $(LFLAGS)
