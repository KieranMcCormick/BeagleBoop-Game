//
// Created by jamie on 15/10/17.
//

#include "InputManager.h"
#include <time.h>
#include <stdio.h>

static struct timespec delay = {2l, 0l};

int main(int argc, char* argv[]){
    InputManager_init(5000);
    int reactionTime = 0;
    int input = 0;
    for(int i = 0; i < NUMBER_OF_INPUTS; i++){
        printf("Current instruction is: %s.\n", InputManager_getInputString(i));
        input = InputManager_waitForInput(&reactionTime);
        if(input == i){
            printf("You did the right input!\n");
        }
        else{
            printf("You did the wrong input or took too long!\n");
        }
        nanosleep(&delay, (struct timespec*) NULL);
    }
    InputManager_cleanup();
    return 0;
}

