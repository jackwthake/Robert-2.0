#ifndef __ULTRASONIC_DRIVER_H__
#define __ULTRASONIC_DRIVER_H__

#include <stdint.h>

extern void ultrasonic_init(void);
extern uint16_t ultrasonic_get(void);

#endif