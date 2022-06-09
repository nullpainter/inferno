#ifndef _FLAME_H_
#define _FLAME_H_

#include <TaskScheduler.h>

// LED pin mappings
#define D0 16
#define D1 5
#define D4 2
#define D8 15

#define RED D4

#define MIN_DUTY_ON 10
#define MAX_DUTY_ON 15

#define MIN_DUTY_OFF 0
#define MAX_DUTY_OFF 3

#define NUM_LEDS 4

#define FADE_DURATION 2000

// 15 minutes
#define UNHEALTHY_DURATION 900000

void fireCallback();

#endif