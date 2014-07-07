// File:			status.c
// Description:		Contains functionality to operate a status LED with blink codes or a heartbeat
// Author:			Jordan Hiebert
// Version:         1.0.0

#include "status.h"
#include <p24Fxxxx.h>

typedef enum
{
    DETERMINE_NEXT_STATUS,
    LED_ON,
    LED_OFF,
    FILL_TIME
}StatusStates;

static char status_override(void);

char status[MAX_STATUSES];

/**
 * The main task for the status LED library. The scheduler should call this on a regular interval.
 * @param call_interval The scheduler period for the status LED task. Recommended time is 5ms, but in theory it doesn't matter as long as CODE_BLINK_TIME, CODE_BLINK_PAUSE, and HEARTBEAT_BLINK_TIME are divisible by it.
 */
void status_routine(unsigned long call_interval)
{
    static unsigned char status_queue_position = 0;
    static unsigned char status_state = 0;
    static unsigned char flash_count = 0;
    static unsigned int status_timer;

    status_timer += call_interval;
    
    if(status_override())
    {
        switch(status_state)
        {
            case DETERMINE_NEXT_STATUS: // moves through the array until an override is found. This will never get stuck because status_override() returned non-zero for us to arrive here.
                while(status[status_queue_position] != 1)
                {
                    if(++status_queue_position >= MAX_STATUSES)
                        status_queue_position = 0;
                }
                status_state = LED_ON;
                break;
            case LED_ON: // turns LED on and waits a specified amount of time
                STATUS_LED_LAT = 1;
                if(status_timer >= CODE_BLINK_TIME)
                {
                    STATUS_LED_LAT = 1;
                    status_timer = 0;
                    status_state = LED_OFF;
                }
                break;
            case LED_OFF: // turns LED off and waits a specified amount of time
                STATUS_LED_LAT = 0;
                if(status_timer >= CODE_BLINK_TIME)
                {
                    STATUS_LED_LAT = 0;
                    status_timer = 0;
                    if(++flash_count >= (status_queue_position + 1))
                    {
                        flash_count = 0;
                        status_state = FILL_TIME;
                    }
                    else
                    {
                        status_state = LED_ON;
                    }
                }
                break;
            case FILL_TIME: // creates a pause between blink codes
                if(status_timer >= CODE_BLINK_PAUSE)
                {
                    status_timer = 0;
                    status_queue_position++; // must be incremented for the while loop in DETERMINE_NEXT_STATUS to function
                    status_state = DETERMINE_NEXT_STATUS;
                }
                break;
        }
    }
    else // no blink codes, just heartbeat
    {
        if(status_timer >= HEARTBEAT_BLINK_TIME)
        {
            STATUS_LED_LAT ^= 1;
            status_timer = 0;
        }
    }
}

/**
 * Adds a blink code to the queue. Blink codes will cycle through from lowest to highest.
 * @param num_flashes The blink code to add.
 * @return 1 if code successfully added, 0 if request out of range.
 */
char status_add_code(unsigned char num_flashes)
{
    if(num_flashes > MAX_STATUSES) return 0;
    
    status[num_flashes-1] = 1;
    return 1;
}

/**
 * Removes a blink code from the queue.
 * @param num_flashes The blink code to remove.
 * @return 1 if code successfully removed, 0 if request out of range.
 */
char status_remove_code(unsigned char num_flashes)
{
    if(num_flashes > MAX_STATUSES) return 0;

    status[num_flashes-1] = 0;
    return 1;
}

/**
 * Used to determine if there are any status codes to blink out.
 * @return 1 if there are blink codes in the queue, 0 if no overrides.
 */
static char status_override(void)
{
    char status_present = 0;
    int i;
    
    for(i=0;i<MAX_STATUSES;i++)
    {
        if(status[i] != 0)
        {
            status_present = 1;
        }
    }
    
    return status_present;
}
