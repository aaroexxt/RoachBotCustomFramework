#include <stdio.h>

#include "Roach_Events.h"
#include "roach.h"
#include "Roach_State_Machine.h"

//a list of states that this SM uses:

enum {
    Moving_Forward,
    Reversing,
    Turning_Randomly,
    Hiding,
};

int current_state;

/* This function initializes the roach state machine.
 * At a minimum, this requires setting the first state.
 * Also, execute any actions associated with initializing the SM 
 * (that is, the actions on the arrow from the black dot in the SM diagram)*/
void Initialize_RoachStateMachine(void)
{
    current_state = Moving_Forward;
    Roach_LeftMtrSpeed(100);
    Roach_RightMtrSpeed(100);

    //seed rand:
    srand(Roach_LightLevel());
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
        break;
        
    case Hiding:
        printf("Current state:  Hiding\r\n");
        //no transitions out of hiding (for now)
        break;
    
    }
};
