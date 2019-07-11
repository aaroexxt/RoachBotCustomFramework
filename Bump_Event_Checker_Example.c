/* 
 * File:   Hello_World_main.c
 * Author: AutonomousSystemsLab
 *
 * Created on July 5, 2019, 1:32 PM
 */

//Standard C libraries:
#include <stdio.h>
#include <stdlib.h>

//Microchip's specialized library
#include <xc.h>

//A library to use the uc32 board
#include "BOARD.h"
#include "roach.h"
#include "timers.h"

char FL_bump_pressed_flag = 0;
char FL_bump_released_flag = 0;

void checkForFLBump()
{
    static char prevState = 0;
    char currState = Roach_ReadFrontLeftBumper();

    if (currState != prevState) {
        if (currState != 0) {
            FL_bump_pressed_flag = 1;
        } else FL_bump_released_flag = 1;
    }
    prevState = currState;
}

int main(void)
{
    // <editor-fold defaultstate="collapsed" desc="//Initialization code (BOARD_Init(), etc)">


    //These calls configure the Pic32 so that it can interact with the Roach hardware
    BOARD_Init();
    Roach_Init();
    TIMERS_Init();

    //Initialization code here:
    printf("Welcome to Events and Services lab, compiled on %s %s\r\n", __TIME__, __DATE__);
    // </editor-fold>

    while (1) {
        checkForFLBump();

        //poll for events:
        if (FL_bump_pressed_flag) {
            printf("FL BUMPER PRESSED\r\n");
            FL_bump_pressed_flag = 0;
        }
        if (FL_bump_released_flag){
            printf("FL BUMPER RELEASED\r\n");
            FL_bump_released_flag = 0;
        }
    }

    return (EXIT_SUCCESS);
}

