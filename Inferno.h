#ifndef _INFERNO_H_
#define _INFERNO_H_

// LED pin mappings
#define D1 5
#define D3 0
#define D5 14
#define D7 13

#define RED D3

#define NUM_LEDS 4

// Duration that LED should stay on before randomly changing intensity
#define MIN_LED_ON 15
#define MAX_LED_ON 25

// Min and max LED intensity for flame effect
#define MIN_INTENSITY 100
#define MAX_INTENSITY 255

// Duration in ms for LED fade in/out
#define FADE_DURATION 1000

// Duration in ms for the the WiFi connection blinking animation 
#define BLINK_DURATION 500

// 15 minutes
#define UNHEALTHY_DURATION 900000

#endif