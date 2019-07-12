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



char timer_Expired_EventFlag[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char bumperPressed_EventFlag[4] = {0, 0, 0, 0};
char bumperReleased_EventFlag[4] = {0, 0, 0, 0};

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
int mazeCommandsMax = 10;
#define STOP -1


int maze1[11][3] = {{FORWARD,50,4000},{RIGHT,50,2000}, {FORWARD, 100, 2000}, {RIGHT, 50, 2000}, {FORWARD, 100, 2000}, {LEFT, 50, 2000}, {FORWARD, 100, 4000}
, {LEFT, 50, 2000}, {FORWARD, 100, 4000}, {LEFT, 50, 2000},{STOP, 0, 10}};


int checkForTimerEvents(void)
{
    static char previous_timer_state[16] = {TIMER_NOT_ACTIVE, TIMER_NOT_ACTIVE, TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE,TIMER_NOT_ACTIVE};
    char current_timer_state[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int i = 0;
    for (i = 0; i<16; i++) {
        current_timer_state[i] = TIMERS_IsTimerActive(i);

        if (previous_timer_state[i] == TIMER_ACTIVE && 
            current_timer_state[i] == TIMER_NOT_ACTIVE) {
        //then an event occurred!
            timer_Expired_EventFlag[i] = 1;
        }
        previous_timer_state[i] = current_timer_state[i];
    }
}
int checkForBumperEvents(void)
{
    static char previous_bumper_state[4]={0,0,0,0}; 
    char current_bumper_state[4] = {Roach_ReadFrontLeftBumper(), Roach_ReadFrontRightBumper(), Roach_ReadRearLeftBumper(), Roach_ReadRearRightBumper()};
    static int bumperCountdown[4] = {0, 0, 0, 0}; //debounce array
    int i = 0;
    for (i=0; i<4; i++){
        if (bumperCountdown[i] <= 0) {
            if (previous_bumper_state[i] && 
                !current_bumper_state[i]) {
            //then an event occurred!
                bumperReleased_EventFlag[i] = 1;
                bumperCountdown[i] = 200; //set  debounce variable
            } else if (!previous_bumper_state[i] && 
                current_bumper_state[i]) {
            //then an event occurred!
                bumperPressed_EventFlag[i] = 1;
                bumperCountdown[i] = 200; //set debounce variable
            }
            previous_bumper_state[i] = current_bumper_state[i];
        } else {
            bumperCountdown[i]--;
        }
    }
}


void clearTimerEvents() {
    int i = 0;
    for(i = 0; i<16; i++) {
        timer_Expired_EventFlag[i] = 0;
    }
}

void clearBumperEvents() {
    int i = 0;
    for(i = 0; i<4; i++) {
        bumperPressed_EventFlag[i] = 0;
        bumperReleased_EventFlag[i] = 0;
    }
}

void ToggleLED_Service0()
{
    static char LED_triggered = 1;
    //what the strategy is here: first get the current LED and then mask to occlude the last 4 bits; then add the final 4 bits of data depending on whether the LED is triggered
    Roach_LEDSSet((Roach_LEDSGet()&0b111111110000) + ((LED_triggered)?0b1111:0b0)); 
    LED_triggered = !LED_triggered;

    TIMERS_InitTimer(0, 2000); //reinit timer
}
void ToggleLED_Service1()
{
    static char LED_triggered = 1;
    Roach_LEDSSet((Roach_LEDSGet()&0b111100001111) + ((LED_triggered)?0b000011110000:0b0)); 
    LED_triggered = !LED_triggered;

    TIMERS_InitTimer(1, 3000); //reinit timer
}
void ToggleLED_Service2()
{
    static char LED_triggered = 1;

    Roach_LEDSSet((Roach_LEDSGet()&0b000011111111) + ((LED_triggered)?0b111100000000:0b0)); 
    LED_triggered = !LED_triggered;
    
    TIMERS_InitTimer(2, 5000); //reinit timer
}
int stagemaze = 0;


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

    //first, setup the framework:
    TIMERS_InitTimer(0, 2000);
    TIMERS_InitTimer(1, 3000);
    TIMERS_InitTimer(2, 5000);
    TIMERS_InitTimer(3, 1000);
    
    while (1) {
        //continuous services (event checkers):
        checkForTimerEvents();
        checkForBumperEvents();
        
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

        //poll for events:
        if (timer_Expired_EventFlag[0]) {
            //printf("A--\r\n");
            //run service        
            ToggleLED_Service0();
        }
        if (timer_Expired_EventFlag[1]) {
           // printf("-B-\r\n");
            //run service        
            ToggleLED_Service1();
        }
        if (timer_Expired_EventFlag[2]) {
           // printf("--C\r\n");
            //run service        
            ToggleLED_Service2();
        }
        if (timer_Expired_EventFlag[3] && (stagemaze <= mazeCommandsMax)) {
            printf("Timer 3 expired!");
            int currentDirection = maze1[stagemaze][0];
            int currentPower = maze1[stagemaze][1];
            printf("dir: %d pow: %d",currentDirection,currentPower);
            if (currentDirection == FORWARD) {
                Roach_LeftMtrSpeed(currentPower);
                Roach_RightMtrSpeed(currentPower);
            }
            else if (currentDirection == BACKWARD) {
                Roach_LeftMtrSpeed(-currentPower);
                Roach_RightMtrSpeed(-currentPower);
            }
            else if (currentDirection == LEFT){
                Roach_LeftMtrSpeed(-currentPower);
                Roach_RightMtrSpeed(currentPower);
            }
            else if (currentDirection == RIGHT){
                Roach_LeftMtrSpeed(currentPower);
                Roach_RightMtrSpeed(-currentPower);
            }
            TIMERS_InitTimer(3, maze1[stagemaze][2]);
            stagemaze++;
        
        }
        if (stagemaze > mazeCommandsMax) { 
            Roach_LeftMtrSpeed(0);
            Roach_RightMtrSpeed(0);
        }
        //clear flags
        clearTimerEvents();
        clearBumperEvents();
    }

    return (EXIT_SUCCESS);
}
