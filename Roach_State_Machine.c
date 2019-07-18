#include <stdio.h>

#include "Roach_Events.h"
#include "roach.h"
#include "Roach_State_Machine.h"

//a list of states that this SM uses:

enum {
    Moving_Forward,
    Reverse,
    WaitS,
    Hiding
};

enum {
    FORWARD,
    BACK,
    LEFT,
    RIGHT
};

int current_state;
int nextDirection;

/* This function initializes the roach state machine.
 * At a minimum, this requires setting the first state.
 * Also, execute any actions associated with initializing the SM 
 * (that is, the actions on the arrow from the black dot in the SM diagram)*/
void Initialize_RoachStateMachine(void)
{
    //seed rand:
    srand(Roach_LightLevel());

    //Set state
    current_state = Moving_Forward;
    Roach_LeftMtrSpeed(100);
    Roach_RightMtrSpeed(100);
};

/* 
 * @briefThis function feeds newly detected events to the roach state machine.
 * @param event:  The most recently detected event*/
void Run_RoachStateMachine(Event event)
{
    switch (current_state) {
        case Moving_Forward:
            printf("Current state:  Moving_Forward\r\n");
            if (event == ENTERED_DARK) {
                current_state = Hiding;
                //stop motors:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            if (event == FRONT_RIGHT_BUMPER_PRESSED) {
                //Reverse
                current_state = Reverse;
                nextDirection = LEFT;
                //Set motor power:
                Roach_LeftMtrSpeed(-100);
                Roach_RightMtrSpeed(-100);

                TIMERS_InitTimer(0, 750); //set timer for time
            }
            if (event == FRONT_LEFT_BUMPER_PRESSED) {
                //Reverse
                current_state = Reverse;
                nextDirection = RIGHT;
                Roach_LeftMtrSpeed(-100);
                Roach_RightMtrSpeed(-100);

                TIMERS_InitTimer(0, 750); //set timer for time
            }
            break;
            
        case Hiding:
            printf("Current state:  Hiding\r\n");
            
            if (event == ENTERED_LIGHT) { //go back to moving forward
                current_state = Moving_Forward;
                Roach_LeftMtrSpeed(100);
                Roach_RightMtrSpeed(100);
            }
            break;

        case Reverse:
            printf("Current state: Reversing\r\n");
            //If bumper gets pressed immediately break
            if (event == REAR_LEFT_BUMPER_PRESSED || event == REAR_RIGHT_BUMPER_PRESSED) {
                current_state = Moving_Forward;
            }
            if (event == TIMER0_EXPIRED) { //Otherwise wait for timer and then change it
                if (nextDirection == LEFT) {
                    Roach_LeftMtrSpeed(-100);
                    Roach_RightMtrSpeed(100);
                } else {
                    Roach_LeftMtrSpeed(100);
                    Roach_RightMtrSpeed(-100);
                }
                TIMERS_InitTimer(0, 500);
                current_state = WaitS;
            }
            break;
        case WaitS:
            printf("Current state: WaitS");
            if (event == TIMER0_EXPIRED) {
                current_state == Moving_Forward;
            }
            break;
    }
};
