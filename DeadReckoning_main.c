//Standard C libraries:
#include <stdio.h>
#include <stdlib.h>

//Microchip's specialized library
#include <xc.h>

//A library to use the uc32 board
#include "BOARD.h"
#include "roach.h"

#define ONE_SECOND 500000

/*
 * 
 */

void waitOneSecond()
{
    int i = 0;
        while (i < ONE_SECOND) {
            i++;
        }
}

void delayMs(int ms)
{
    int i = 0;
    while (i < (ONE_SECOND/1000)*ms) {
        i++;
    }
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

    printf("Welcome to Dead Recokining!  This program was compiled on %s as %s.\r\n", __DATE__, __TIME__);

    //Initialization code here:

    int i;


    while (1) {
        driveForwardMs(1200);
        turnLeftMs(250);
        driveForwardMs(2400);
        turnLeftMs(500);
        driveForwardMs(3400);
        turnLeftMs(750);
        driveForwardMs(1500);
        turnLeftMs(350);
        driveForwardMs(2100);
        
        turnRightMs(300);
        driveForwardMs(750);
        turnRightMs(700);
        driveForwardMs(2500);
        delayMs(5000);
        while (1) {
            int value = Roach_LightLevel();
            value = map(value, 0, 1023, 0, 100);
            Roach_RightMtrSpeed(value);
            Roach_LeftMtrSpeed(value);
        }
        break;
    };

    return (EXIT_SUCCESS);
}

