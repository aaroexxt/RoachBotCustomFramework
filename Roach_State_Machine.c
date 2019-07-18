#include <stdio.h>

#include "Roach_Events.h"
#include "roach.h"
#include "Roach_State_Machine.h"

//a list of states that this SM uses:

enum {
    Moving_Forward,
    Reverse,
    WaitInitialTurn,
    Hiding,
    WaitAfterReverse
};

enum {
    FORWARD,
    BACK,
    LEFT,
    RIGHT
};

int current_state;
int nextDirection;
int stallCount = 0;

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
            stallCount = 0; //reset stallCount

            if (event == ENTERED_DARK) {
                current_state = Hiding;
                //stop motors:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            if (event == FRONT_RIGHT_BUMPER_PRESSED) {
                //Reverse
                current_state = WaitInitialTurn;
                nextDirection = LEFT;
                //Set motor power:
                Roach_LeftMtrSpeed(-100);
                Roach_RightMtrSpeed(100);

                TIMERS_InitTimer(0, 250); //set timer for time
            }
            if (event == FRONT_LEFT_BUMPER_PRESSED) {
                //Reverse
                current_state = WaitInitialTurn;
                nextDirection = RIGHT;
                Roach_LeftMtrSpeed(100);
                Roach_RightMtrSpeed(-100);

                TIMERS_InitTimer(0, 250); //set timer for time
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
            if (event == TIMER0_EXPIRED || event == REAR_LEFT_BUMPER_PRESSED || event == REAR_RIGHT_BUMPER_PRESSED) { //Otherwise wait for timer and then change it
                if (nextDirection == LEFT) {
                    Roach_LeftMtrSpeed(-100);
                    Roach_RightMtrSpeed(100);
                } else {
                    Roach_LeftMtrSpeed(100);
                    Roach_RightMtrSpeed(-100);
                }
                current_state = WaitAfterReverse;
                TIMERS_InitTimer(0, 400);
            }
            break;
        case WaitInitialTurn:
            printf("Current state: WaitInitialTurn");
            if (event == TIMER0_EXPIRED) { //check if timer expired
                current_state == Moving_Forward;
            }
            if (event == ENTERED_DARK) {
                current_state = Hiding;
                //stop motors:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            if (Roach_ReadFrontRightBumper() && Roach_ReadFrontLeftBumper()) {
                stallCount++;
                if (stallCount > 100000) {
                    Roach_LeftMtrSpeed(-100);
                    Roach_RightMtrSpeed(-100);
                    TIMERS_InitTimer(0, 500);
                    current_state = Reverse;
                }
            }
            break;
        case WaitAfterReverse:
            if (event == TIMER0_EXPIRED) { //check if timer expired
                current_state == Moving_Forward;
            }
            if (event == ENTERED_DARK) {
                current_state = Hiding;
                //stop motors:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            break;
    }
};
