# lib-p_status

lib-p_status can be used to flash a heartbeat or status codes on an LED.

## System Requirements
* Microchip PIC24 project- any PIC24 should work, tested with PIC24FJ256GA106 and PIC24FJ256GA110

## Quick Start
1. Get the newest version from the [releases](https://github.com/PriceElectronics/lib-p_status/releases) section of the repository and copy the source files to your project directory.
2. Add files to your IDE project if required.

## Usage
1. Configure the LED pin, timers, and blink codes in ```status.h```.
2. Run  ```status_routine()``` on a regular interval using a scheduler or similar.
3. Add blink codes using ```status_add_code()```. If no codes are queued, the LED will flash on a regular "heartbeat" interval. You may remove status codes by using ```status_remove_code()```.

## Known Issues / Limitations
* All blink codes must be added to the ```StatusCodes``` typedef enumeration in ```status.h```. This is required because ```NUM_STA_CODES``` is used to determine the size of the code array.
