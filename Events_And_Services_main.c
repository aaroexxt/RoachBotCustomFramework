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


//Constants to make maze navigation arrays easier to write
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define STOP -1
//Current maze step
int MazeStep = 0;

//Maze navigation array (contains commands for maze)
//Command Structure: {DIRECTION, POWER (0-100), TIME}
int maze1[11][3] = {
    {FORWARD, 50, 4000},
    {RIGHT, 50, 2000},
    {FORWARD, 100, 2000},
    {RIGHT, 50, 2000},
    {FORWARD, 100, 2000},
    {LEFT, 50, 2000},
    {FORWARD, 100, 4000},
    {LEFT, 50, 2000},
    {FORWARD, 100, 4000},
    {LEFT, 50, 2000},
    {STOP, 0, 10}
};
int mazeCommandsMax = 10; //list of actual maze elements (MINUS STOP COMMAND)





//Clear timer events by setting all of the timer event flags to zero
void clearTimerEvents() {
    int i = 0;
    for(i = 0; i<16; i++) {
        timerExpired_EventFlag[i] = 0;
    }
}

//Clear bumper events by setting all of the bumper event flags to zero
void clearBumperEvents() {
    int i = 0;
    for(i = 0; i<4; i++) {
        bumperPressed_EventFlag[i] = 0;
        bumperReleased_EventFlag[i] = 0;
    }
}


/*
MAIN CODE---
*/


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
    BOARD_Init();
    Roach_Init();
    TIMERS_Init();

    //Setup the timer(s)
    TIMERS_InitTimer(0, 1000);
    
    while (1) {
        //continuous services (event checkers):
        checkForTimerEvents();
        checkForBumperEvents();

        BumperEvent currentBumperEvent = getLastBumperEvent();
        TimerEvent currentTimerEvent = getLastTimerEvent();
        
        //front left
        if (bumperPressed_EventFlag[0]){
            printf("FL bumper pressed");
        }
        else if (bumperReleased_EventFlag[0]){
            printf("FL bumper released");
        }
        //front right
        if (bumperPressed_EventFlag[1]){
            printf("FR bumper pressed");
        }
        else if (bumperReleased_EventFlag[1]){
            printf("FR bumper released");
        }
        //back left
        if (bumperPressed_EventFlag[2]){
            printf("BL bumper pressed");
        }
        else if (bumperReleased_EventFlag[2]){
            printf("BL bumper released");
        }
        //back right
        if (bumperPressed_EventFlag[3]){
            printf("BR bumper pressed");
        }
        else if (bumperReleased_EventFlag[3]){
            printf("BR bumper released");
        }

        if (timerExpired_EventFlag[3] && (stagemaze <= mazeCommandsMax)) {
            printf("Timer 3 expired!");
            int currentDirection = maze1[stagemaze][0];
            int currentPower = maze1[stagemaze][1];
            
            printf("dir: %d pow: %d",currentDirection,currentPower);
            switch (currentDireciton) {
                case FORWARD:
                    Roach_LeftMtrSpeed(currentPower);
                    Roach_RightMtrSpeed(currentPower);
                    break;
                case BACKWARD:
                    Roach_LeftMtrSpeed(-currentPower);
                    Roach_RightMtrSpeed(-currentPower);
                    break;
                case RIGHT:
                    Roach_LeftMtrSpeed(currentPower);
                    Roach_RightMtrSpeed(-currentPower);
                    break;
                case LEFT:
                    Roach_LeftMtrSpeed(-currentPower);
                    Roach_RightMtrSpeed(currentPower);
                    break;
                case STOP: //stop or default
                default:
                    Roach_LeftMtrSpeed(0);
                    Roach_RightMtrSpeed(0);
                    break;
            }

            TIMERS_InitTimer(3, maze1[stagemaze][2]);
            MazeState++;
        
        }
        if (stagemaze > mazeCommandsMax) { 
            
        }
        //clear flags
        clearTimerEvents();
        clearBumperEvents();
    }

    return (EXIT_SUCCESS);
}
