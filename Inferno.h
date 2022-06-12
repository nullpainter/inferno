#ifndef _INFERNO_H_
#define _INFERNO_H_

// LED pin mappings

#define D1 5
#define D3 0
#define D5 14
#define D7 13

#define RED D3

// Duration that LED should stay on before randomly changing intensity
#define MIN_LED_ON 15
#define MAX_LED_ON 25

#define MIN_INTENSITY 100
#define MAX_INTENSITY 255

#define NUM_LEDS 4

#define FADE_DURATION 1000

#define BLINK_DURATION 500

// 15 minutes
#define UNHEALTHY_DURATION 900000

#endif