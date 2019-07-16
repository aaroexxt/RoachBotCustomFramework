//Standard C libraries:
#include <stdio.h>
#include <stdlib.h>

//Microchip's specialized library
#include <xc.h>

//A library to use the uc32 board
#include "BOARD.h"
#include "roach.h"
#include "timers.h"

#define ONE_SECOND 500000

#define TIMER_A 0
#define TIMER_B 1

void delayMs(int ms){
    
}

void driveForwardMs(int ms) {
    Roach_RightMtrSpeed(100);
    Roach_LeftMtrSpeed(100);
    delayMs(ms);
    Roach_RightMtrSpeed(0);
    Roach_LeftMtrSpeed(0);
}
void driveBackwardMs(int ms) {
    Roach_RightMtrSpeed(-100);
    Roach_LeftMtrSpeed(-100);
    delayMs(ms);
    Roach_RightMtrSpeed(0);
    Roach_LeftMtrSpeed(0);
}
void turnLeftMs(int ms) {
    Roach_RightMtrSpeed(100);
    Roach_LeftMtrSpeed(-100);
    delayMs(ms);
    Roach_RightMtrSpeed(0);
    Roach_LeftMtrSpeed(0);
}
void turnRightMs(int ms) {
    Roach_RightMtrSpeed(-100);
    Roach_LeftMtrSpeed(100);
    delayMs(ms);
    Roach_RightMtrSpeed(0);
    Roach_LeftMtrSpeed(0);
}

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int state = 0;

int main(void)
{
    //These calls configure the Pic32 so that it can interact with the Roach hardware
    BOARD_Init();
    Roach_Init();
    TIMERS_Init();
    
    TIMERS_InitTimer(0, 50);
    TIMERS_InitTimer(1, 100);
    

    printf("Welcome to Dead Recokining!  This program was compiled on %s as %s.\r\n", __DATE__, __TIME__);

    //Initialization code here:

    int ledState = 0;
    
    Roach_RightMtrSpeed(100);
    Roach_LeftMtrSpeed(100);
    while (1) {
        if(TIMERS_IsTimerExpired(0)) {
            //set clear, reset
            TIMERS_ClearTimerExpired(0);
            TIMERS_InitTimer(0, 50);
            
            switch(ledState) {
                case 0:
                    Roach_LEDSSet(0b000000000000);
                    ledState++;
                    break;
                case 2<<12:
                    ledState = 0;
                    break;
                default:
                    Roach_LEDSSet(ledState);
                    ledState++;
                    break;
            }
            
        }
        if(TIMERS_IsTimerExpired(1)) {
            //set clear, reset
            TIMERS_ClearTimerExpired(1);
            TIMERS_InitTimer(1, 100);
            
            int value = Roach_LightLevel();
            value = map(value, 0, 1023, 0, 100);
            Roach_RightMtrSpeed(value);
            Roach_LeftMtrSpeed(value);
            
        }
        
        if (Roach_ReadFrontLeftBumper() || Roach_ReadFrontRightBumper()) {
            TIMERS_StopTimer(1); //pause timer1
            Roach_RightMtrSpeed(-100);
            Roach_LeftMtrSpeed(-100);
            int i = 0;
            for(i=0; i<100000; i++){}
            TIMERS_StartTimer(1);
        }
        //printf("%d",(int)TIMERS_GetTime());
    }
    
    
    
    
//    while (1) {
//        driveForwardMs(1200);
//        turnLeftMs(250);
//        driveForwardMs(2400);
//        turnLeftMs(500);
//        driveForwardMs(3400);
//        turnLeftMs(750);
//        driveForwardMs(1500);
//        turnLeftMs(350);
//        driveForwardMs(2100);
//        
//        turnRightMs(300);
//        driveForwardMs(750);
//        turnRightMs(700);
//        driveForwardMs(2500);
//        delayMs(5000);
//        while (1) {
//            int value = Roach_LightLevel();
//            value = map(value, 0, 1023, 0, 100);
//            Roach_RightMtrSpeed(value);
//            Roach_LeftMtrSpeed(value);
//        }
//        break;
//    };

    return (EXIT_SUCCESS);
}

