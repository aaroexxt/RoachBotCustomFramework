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

    //first, setup the framework:
    TIMERS_InitTimer(0, 1);
    TIMERS_InitTimer(1, 2);
    TIMERS_InitTimer(2, 3);

    while (1) {
        //poll for events:
        if (TIMERS_IsTimerExpired(0)) {
            printf("A--\r\n");
            TIMERS_ClearTimerExpired(0);
            TIMERS_InitTimer(0,2000);
        }
        if (TIMERS_IsTimerExpired(1)) {
            printf("-B-\r\n");
            TIMERS_ClearTimerExpired(1);
            TIMERS_InitTimer(1,3000);
        }
        if (TIMERS_IsTimerExpired(2)) {
            printf("--C\r\n");
            TIMERS_ClearTimerExpired(2);
            TIMERS_InitTimer(2,5000);
        }
    }

    return (EXIT_SUCCESS);
}

