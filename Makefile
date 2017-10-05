GCC = arm-linux-gnueabihf-gcc
COMPILE_FLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread
FILE_NAME = accelerometer

all:
	$(GCC) $(COMPILE_FLAGS) $(FILE_NAME).c test_accelerometer.c -o $(FILE_NAME)
	cp $(FILE_NAME) $(HOME)/cmpt433/public/myApps/
