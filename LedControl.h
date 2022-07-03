#ifndef _LEDCONTROL_H_
#define _LEDCONTROL_H_

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include <FastLED.h>

#define LED_PIN 1
#define CHIPSET WS2812B
#define NUM_LEDS 8

#define MAX_BRIGHTNESS 255

// Default duration of fade in/out, in ms
#define FADE_DURATION 500

// HSV values of flame gradient values
#define RED 0
#define ORANGE 20
#define YELLOW 25

#define OFFLINE 0

enum class FlameMode
{
    Flame,
    Party,
    Offline
};

enum class FadeState
{
    On,
    Off,
    FadeIn,
    FadeOut
};

class LedControl
{
public:
    static void setup();
    static void update();
    static void setMode(FlameMode mode);
    static void on();
    static void off();
    static void fadeIn(int delay = 0, int duration = FADE_DURATION);
    static void fadeOut(int delay = 0, int duration = FADE_DURATION);

private:
    static void resetFadeState(uint8_t brightness);
    static uint8_t getBrightness();

    static CRGB _leds[NUM_LEDS];
    static CHSVPalette16 _palette;
    static FlameMode _flameMode;
    static FadeState _fadeState;
    static int _fadeDuration;
    static int _fadeDelay;
    static long _fadeStart;
};

#endif