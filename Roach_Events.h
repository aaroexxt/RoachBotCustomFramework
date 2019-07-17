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
    FRONT_RIGHT_BUMPER_PRESSED,
    FRONT_LEFT_BUMPER_PRESSED,
    REAR_RIGHT_BUMPER_PRESSED,
    REAR_LEFT_BUMPER_PRESSED,
    FRONT_RIGHT_BUMPER_RELEASED,
    FRONT_LEFT_BUMPER_RELEASED,
    REAR_RIGHT_BUMPER_RELEASED,
    REAR_LEFT_BUMPER_RELEASED,
    ENTERED_DARK,
    ENTERED_LIGHT,
    //Timer Events
    TIMER0_EXPIRED,
    TIMER1_EXPIRED,
    TIMER2_EXPIRED,
    TIMER3_EXPIRED,
    TIMER4_EXPIRED,
    TIMER5_EXPIRED,
    TIMER6_EXPIRED,
    TIMER7_EXPIRED,
    TIMER8_EXPIRED,
    TIMER9_EXPIRED,
    TIMER10_EXPIRED,
    TIMER11_EXPIRED,
    TIMER12_EXPIRED,
    TIMER13_EXPIRED,
    TIMER14_EXPIRED,
    TIMER15_EXPIRED
} Event;

Event CheckForAllEvents(void);

Event CheckForBumperEvents(void);

Event CheckForLightEvents(void);

Event CheckForTimerEvents(void);

Event CheckForNavigationEvents(void);

#endif	/* ROACH_EVENTS_H */

