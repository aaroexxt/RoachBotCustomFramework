/* 
 * File:   Roach_Events.h
 * Author: AutonomousSystemsLab & Aaron Becker
 *
 * Created on July 15, 2019, 10:49 AM
 */

#ifndef ROACH_EVENTS_H
#define	ROACH_EVENTS_H

//A list of events in this system:

typedef enum {
    NO_EVENT,
    //Bumper events
    FRONT_RIGHT_BUMP_PRESSED,
    FRONT_LEFT_BUMP_PRESSED,
    REAR_RIGHT_BUMP_PRESSED,
    REAR_LEFT_BUMP_PRESSED,
    FRONT_RIGHT_BUMP_RELEASED,
    FRONT_LEFT_BUMP_RELEASED,
    REAR_RIGHT_BUMP_RELEASED,
    REAR_LEFT_BUMP_RELEASED,
    ENTERED_DARK,
    ENTERED_LIGHT,
    //Timer Events
    TIMER0_EVENT,
    TIMER1_EVENT,
    TIMER2_EVENT,
    TIMER3_EVENT,
    TIMER4_EVENT,
    TIMER5_EVENT,
    TIMER6_EVENT,
    TIMER7_EVENT,
    TIMER8_EVENT,
    TIMER9_EVENT,
    TIMER10_EVENT,
    TIMER11_EVENT,
    TIMER12_EVENT,
    TIMER13_EVENT,
    TIMER14_EVENT,
    TIMER15_EVENT
} Event;

Event CheckForAllEvents(void);

Event CheckForBumperEvents(void);

Event CheckForLightEvents(void);

Event CheckForTimerEvents(void);

Event CheckForNavigatioEvents(void);

#endif	/* ROACH_EVENTS_H */

