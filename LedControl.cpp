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
    for (int i = 0; i < _ledCount; i++)
    {
        _leds[i]->setIntensity(intensity);
    }
}

void LedControl::on()
{
    for (int i = 0; i < _ledCount; i++)
    {
        _leds[i]->on();
    }
}

void LedControl::off()
{
    for (int i = 0; i < _ledCount; i++)
    {
        _leds[i]->off();
    }
}

void LedControl::fadeIn(int duration)
{
    for (int i = 0; i < _ledCount; i++)
    {
        _leds[i]->fadeIn(duration);
    }
}

void LedControl::fadeOut(int duration, int delay)
{
    for (int i = 0; i < _ledCount; i++)
    {
        _leds[i]->fadeOut(duration, delay);
    }
}

void LedControl::update()
{
    for (int i = 0; i < _ledCount; i++)
    {
        _leds[i]->update();
    }
}