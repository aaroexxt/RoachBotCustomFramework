#include "Roach_Events.h"
#include "roach.h"
#include "timers.h"


#define DEBOUNCE_TIMER 15 //select a timer to be the debounce timer
#define DEBOUNCE_PERIOD 30 //in milliseconds, the duration of the debounce period

#define LIGHT_THRESHOLD 500 //The threshold between light and darkness - you may need to change this!

enum {
    DARK, LIGHT
};

Event CheckForAllEvents(void)
{
    Event returnEvent = NO_EVENT;

    returnEvent = CheckForTimerEvents();
    if (returnEvent != NO_EVENT) return returnEvent;

    returnEvent = CheckForBumperEvents();
    if (returnEvent != NO_EVENT) return returnEvent;

    returnEvent = CheckForLightEvents();
    if (returnEvent != NO_EVENT) return returnEvent;

    return NO_EVENT;
}

//Checks for timer events that have occured and sets flags
Event CheckForTimerEvents(void) {
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
            switch(i) {
                case 0:
                    return TIMER0_EXPIRED;
                    break;
                case 1:
                    return TIMER1_EXPIRED;
                    break;
                case 2:
                    return TIMER2_EXPIRED;
                    break;
                case 3:
                    return TIMER3_EXPIRED;
                    break;
                case 4:
                    return TIMER4_EXPIRED;
                    break;
                case 5:
                    return TIMER5_EXPIRED;
                    break;
                case 6:
                    return TIMER6_EXPIRED;
                    break;
                case 7:
                    return TIMER7_EXPIRED;
                    break;
                case 8:
                    return TIMER8_EXPIRED;
                    break;
                case 9:
                    return TIMER9_EXPIRED;
                    break;
                case 10:
                    return TIMER10_EXPIRED;
                    break;
                case 11:
                    return TIMER11_EXPIRED;
                    break;
                case 12:
                    return TIMER12_EXPIRED;
                    break;
                case 13:
                    return TIMER13_EXPIRED;
                    break;
                case 14:
                    return TIMER14_EXPIRED;
                    break;
                case 15:
                    return TIMER15_EXPIRED;
                    break;
            }
        }
        previous_timer_state[i] = current_timer_state[i]; //set previous state
    }

    return NO_EVENT; //if we're here, return no event
}

//Checks for bumper events that have occurred and sets flags
Event CheckForBumperEvents(void)
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
                switch(i) {
                    case 0:
                        return FRONT_LEFT_BUMPER_RELEASED;
                        break;
                    case 1:
                        return FRONT_RIGHT_BUMPER_RELEASED;
                        break;
                    case 2:
                        return REAR_LEFT_BUMPER_RELEASED;
                        break;
                    case 3:
                        return REAR_RIGHT_BUMPER_RELEASED;
                        break;
                }
                bumperCountdown[i] = 200; //set  debounce variable
            } else if (!previous_bumper_state[i] && current_bumper_state[i]) { //previous state LOW, current state HIGH (RISING)
            //then an event occurred!
                switch(i) {
                    case 0:
                        return FRONT_LEFT_BUMPER_PRESSED;
                        break;
                    case 1:
                        return FRONT_RIGHT_BUMPER_PRESSED;
                        break;
                    case 2:
                        return REAR_LEFT_BUMPER_PRESSED;
                        break;
                    case 3:
                        return REAR_RIGHT_BUMPER_PRESSED;
                        break;
                }
                bumperCountdown[i] = 200; //set debounce variable
            }
            previous_bumper_state[i] = current_bumper_state[i]; //set prev to current
        } else {
            bumperCountdown[i]--; //if bumper countdown is not zero yet (i.e. minimum cycles have not elapsed), then decrement the countdown
        }
    }

    return NO_EVENT; //if we got here no event happened
}

Event CheckForLightEvents(void)
{
    static char previousLightState = LIGHT;

    char currentLightState;
    int current_light_reading = Roach_LightLevel();
    if (current_light_reading >= LIGHT_THRESHOLD) {
        currentLightState = DARK;
    } else currentLightState = LIGHT;
    if (currentLightState != previousLightState) {
        previousLightState = currentLightState;
        if (currentLightState == DARK) {
            return ENTERED_DARK;
        } else return ENTERED_LIGHT;
    }

    //if we got this far, there was no event
    return NO_EVENT;
}