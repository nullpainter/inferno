#include "LedControl.h"

unsigned char LedControl::_ledCount = 0;
Led *LedControl::_leds[MAX_LEDS];

void LedControl::add(Led *led)
{
    if (_ledCount < MAX_LEDS)
    {
        _leds[_ledCount++] = led;
    }
}

void LedControl::setIntensity(int intensity)
{
    for (Led *led : _leds)
    {
        led->setIntensity(intensity);
    }
}

void LedControl::on()
{
    for (Led *led : _leds)
    {
        led->on();
    }
}

void LedControl::off()
{
    for (Led *led : _leds)
    {
        led->off();
    }
}

void LedControl::fadeIn(int duration)
{
    for (Led *led : _leds)
    {
        led->fadeIn(duration);
    }
}

void LedControl::fadeOut(int duration, int delay)
{
    for (Led *led : _leds)
    {
        led->fadeOut(duration, delay);
    }
}

void LedControl::update()
{
    for (Led *led : _leds)
    {
        led->update();
    }
}