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


//EventFlags - for various roach events
char timerExpired_EventFlag[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char bumperPressed_EventFlag[4] = {0, 0, 0, 0};
char bumperReleased_EventFlag[4] = {0, 0, 0, 0};

struct BumperEvent {
    char FL;
    char FR;
    char BL;
    char BR;
};

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

//Checks for timer events that have occured and sets flags
int checkForTimerEvents(void) {
    //previous_timer_state represents you guessed it previous timer state
    static char previous_timer_state[16] = {TIMER_NOT_ACTIVE, TIMER_NOT_ACTIVE, TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE};
    //currnent_timer_state represents current timer states
    char current_timer_state[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    int i = 0;
    for (i = 0; i<16; i++) { //loop through 16 builtin timers to check if thay are active
        current_timer_state[i] = TIMERS_IsTimerActive(i);

        if (previous_timer_state[i] == TIMER_ACTIVE && 
            current_timer_state[i] == TIMER_NOT_ACTIVE) { //is the previous state active and current not active (i.e. timer expired?)
        //then an event occurred!
            timerExpired_EventFlag[i] = 1; //set flag
        }
        previous_timer_state[i] = current_timer_state[i]; //set previous state
    }
}

//Checks for bumper events that have occurred and sets flags
BumperEvent checkForBumperEvents(void)
{
    static char previous_bumper_state[4]= {0, 0, 0, 0}; //Represents previous state (FL, FR, BL, BR)
    char current_bumper_state[4] = { //current bumper state is stored here. Calls functions from the roach
        Roach_ReadFrontLeftBumper(),
        Roach_ReadFrontRightBumper(),
        Roach_ReadRearLeftBumper(),
        Roach_ReadRearRightBumper()
    };
    static int bumperCountdown[4] = {0, 0, 0, 0}; //Array of counters to count a minimum amount of loops between bumper events
    
    int i = 0;
    for (i=0; i<4; i++) { //Loop through potential bumper states and check
        if (bumperCountdown[i] <= 0) { //Make sure bumper countdown element is zero. This ensures that a minimum amount of loop cycles occur before the bumper event can be changed.
            if (previous_bumper_state[i] && !current_bumper_state[i]) { //previous state HIGH, current state LOW (FALLING)
            //then an event occurred!
                bumperReleased_EventFlag[i] = 1;
                bumperCountdown[i] = 200; //set  debounce variable
            } else if (!previous_bumper_state[i] && current_bumper_state[i]) { //previous state LOW, current state HIGH (RISING)
            //then an event occurred!
                bumperPressed_EventFlag[i] = 1;
                bumperCountdown[i] = 200; //set debounce variable
            }
            previous_bumper_state[i] = current_bumper_state[i]; //set prev to current
        } else {
            bumperCountdown[i]--; //if bumper countdown is not zero yet (i.e. minimum cycles have not elapsed), then decrement the countdown
        }
    }
    struct BumperEvent *currentBumperEvent;
    currentBumperEvent = malloc(sizeof *BumperEvent);
    currentBumperEvent->FL = 
}

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
