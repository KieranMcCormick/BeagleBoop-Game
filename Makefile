OUTFILE = inputTest
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread

all:
	$(CC_C) $(CFLAGS) InputTest.c InputManager.c Accelerometer.c Potentiometer.c Joystick.c -o $(OUTDIR)/$(OUTFILE)
