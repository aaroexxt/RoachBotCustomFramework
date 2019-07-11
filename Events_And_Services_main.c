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



char timer_Expired_EventFlag[7] = {0, 0, 0, 0, 0, 0, 0};
char bumperPressed_EventFlag[4] = {0, 0, 0, 0};
char bumperReleased_EventFlag[4] = {0, 0, 0, 0};


int checkForTimerEvent(int timer)
{
    static char previous_timer_state = TIMER_NOT_ACTIVE;
    char current_timer_state = TIMERS_IsTimerActive(timer);
    
    if (previous_timer_state == TIMER_ACTIVE && 
            current_timer_state == TIMER_NOT_ACTIVE) {
        //then an event occurred!
            timer_Expired_EventFlag[timer] = 1;
    }
    previous_timer_state = current_timer_state;
}
int checkForBumperEvent(void)
{
    static char previous_bumper_state[4]={0,0,0,0}; 
    int bumper_reading = Roach_ReadBumpers();
    char current_bumper_state[4] = {bumper_reading&0b1000>>3, bumper_reading&0b0100>>2, bumper_reading&0b0010>>1, bumper_reading&0b0001};
    int i = 0;
    for (i=0; i<3; i++){
        if (previous_bumper_state[i] && 
            !current_bumper_state[i]) {
        //then an event occurred!
            bumperReleased_EventFlag[i] = 1;
        }
        if (!previous_bumper_state[i] && 
            current_bumper_state[i]) {
        //then an event occurred!
            bumperPressed_EventFlag[i] = 1;
        }
    }
    for (i=0; i<3; i++){
       previous_bumper_state[i] = current_bumper_state[i]; 
    }
    
    
}

void clearTimerEvents() {
    int i = 0;
    for(i = 0; i<7; i++) {
        timer_Expired_EventFlag[i] = 0;
    }
}

void clearBumperEvents() {
    int i = 0;
    for(i = 0; i<3; i++) {
        bumperPressed_EventFlag[i] = 0;
        bumperReleased_EventFlag[i] = 0;
    }
}

void ToggleLED_Service0()
{
    static char LED_triggered = 1;
    if (LED_triggered == 1){
       Roach_LEDSSet((Roach_LEDSGet()&0b111111110000) + 0b1111); 
       LED_triggered = 0;
    }
    else {
        Roach_LEDSSet(Roach_LEDSGet()&0b111111110000);
        LED_triggered = 1;
    }
    TIMERS_InitTimer(0, 2000);
}
void ToggleLED_Service1()
{
    static char LED_triggered = 1;
    if (LED_triggered == 1){
       Roach_LEDSSet((Roach_LEDSGet()&0b111100001111) + 0b000011110000); 
       LED_triggered = 0;
    }
    else {
        Roach_LEDSSet(Roach_LEDSGet()&0b111100001111);
        LED_triggered = 1;
    }
    TIMERS_InitTimer(1, 3000);
}
void ToggleLED_Service2()
{
    static char LED_triggered = 1;
    if (LED_triggered == 1){
       Roach_LEDSSet((Roach_LEDSGet()&0b000011111111) + 0b111100000000); 
       LED_triggered = 0;
    }
    else {
        Roach_LEDSSet(Roach_LEDSGet()&0b000011111111);
        LED_triggered = 1;
    }
    TIMERS_InitTimer(2, 5000);
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
    BOARD_Init();
    Roach_Init();
    TIMERS_Init();

    //first, setup the framework:
    TIMERS_InitTimer(0, 2000);
    TIMERS_InitTimer(1, 3000);
    TIMERS_InitTimer(2, 5000);
    
    while (1) {
        //continuous services (event checkers):
        checkForTimerEvent(0);
        checkForTimerEvent(1);
        checkForTimerEvent(2);
        checkForBumperEvent();
        
        if (bumperPressed_EventFlag[0]){
            printf("FL bumper pressed");
        }
        else if (bumperReleased_EventFlag[0]){
            printf("FL bumper released");
        }
        if (bumperPressed_EventFlag[1]){
            printf("FR bumper pressed");
        }
        else if (bumperReleased_EventFlag[1]){
            printf("FR bumper released");
        }
        if (bumperPressed_EventFlag[2]){
            printf("BL bumper pressed");
        }
        else if (bumperReleased_EventFlag[2]){
            printf("BL bumper released");
        }
        if (bumperPressed_EventFlag[3]){
            printf("BR bumper pressed");
        }
        else if (bumperReleased_EventFlag[3]){
            printf("BR bumper released");
        }

        //poll for events:
        if (timer_Expired_EventFlag[0]) {
            printf("A--\r\n");
            //run service        
            ToggleLED_Service0();
        }
        if (timer_Expired_EventFlag[1]) {
            printf("-B-\r\n");
            //run service        
            ToggleLED_Service1();
        }
        if (timer_Expired_EventFlag[2]) {
            printf("--C\r\n");
            //run service        
            ToggleLED_Service2();
        }
        
        //clear flags
        clearTimerEvents();
        clearBumperEvents();
    }

    return (EXIT_SUCCESS);
}

