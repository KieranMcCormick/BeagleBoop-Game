OUTFILE = gameTest
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread
LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all:
	$(CC_C) $(CFLAGS) main.c InputManager.c Accelerometer.c Potentiometer.c Joystick.c game_core.c client_interface.c seg_display.c textToSpeech.c audioMixer.c -o $(OUTDIR)/$(OUTFILE) $(LFLAGS) -lasound
	cp -R audio $(OUTDIR)/
