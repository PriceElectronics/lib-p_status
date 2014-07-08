// File:			status.h
// Description:		Contains functionality to operate a status LED with blink codes or a heartbeat
// Author:			Jordan Hiebert
// Version:         1.0.0

#ifndef STATUS_H
#define	STATUS_H

#define STATUS_LED_LAT LATGbits.LATG6   // The status LED output latch

#define CODE_BLINK_TIME 150             // The blink length for codes (ms)
#define CODE_BLINK_PAUSE 1000           // amount of time between codes (ms)
#define HEARTBEAT_BLINK_TIME 1000       // length of heartbeat blinks (ms)

// Status codes.
// Enum value + 1 = number of blinks
typedef enum
{
    STA_RESERVED,
    STA_PRODUCTION_TEST,
    STA_EXT_FLASH_FAIL,
    STA_EE_FAIL,

    NUM_STA_CODES
}StatusCodes;

/**
 * The main task for the status LED library. The scheduler should call this on a regular interval.
 * @param call_interval The scheduler period for the status LED task. Recommended time is 5ms, but in theory it doesn't matter as long as CODE_BLINK_TIME, CODE_BLINK_PAUSE, and HEARTBEAT_BLINK_TIME are divisible by it.
 */
void status_routine(unsigned long call_interval);

/**
 * Adds a blink code to the queue. Blink codes will cycle through from lowest to highest.
 * @param status_code The blink code to add.
 * @return 1 if code successfully added, 0 if request out of range.
 */
char status_add_code(StatusCodes status_code);

/**
 * Removes a blink code from the queue.
 * @param status_code The blink code to remove.
 * @return 1 if code successfully removed, 0 if request out of range.
 */
char status_remove_code(StatusCodes status_code);

#endif	/* STATUS_H */
