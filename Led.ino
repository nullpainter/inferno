#include <Arduino.h>
#include "Led.h"
#include "LedControl.h"

Led::Led(int pin)
{
    _pin = pin;
    pinMode(pin, OUTPUT);

    _intensity = 255;
    _ledState = Normal;

    LedControl::add(this);
}

void Led::update()
{
    switch (_ledState)
    {
    case FadeIn:
    case FadeOut:
        unsigned long elapsed = millis() - _startMillis;

        if (elapsed > _fadeDelay)
        {
            elapsed -= _fadeDelay;

            if (elapsed <= _fadeDuration)
            {
                int index = round(elapsed / (float)_fadeDuration * 255.0);
                if (_ledState == FadeOut)
                {
                    index = 255 - index;
                }

                setIntensity(LOG_CURVE[index]);
            }
            else
            {
                // If fading has completed, reset state to normal
                _ledState = Normal;

                Serial.print("Fade completed: GPIO ");
                Serial.println(_pin);
            }
        }

        break;
    }
}

// FIXME this isn't working after a while(?)
void Led::fadeIn(int duration)
{
    _startMillis = millis();
    _fadeDuration = duration;

    _ledState = FadeIn;
}

void Led::fadeOut(int duration, int delay)
{
    _startMillis = millis();

    _fadeDuration = duration;
    _fadeDelay = delay;
    _ledState = FadeOut;
}

void Led::on()
{
    analogWrite(_pin, _intensity);
}

void Led::off()
{
    digitalWrite(_pin, 0);
}

void Led::toggle()
{
    if (_intensity > 0)
    {
        off();
    }
    else
    {
        on();
    }
}

void Led::setIntensity(int intensity)
{
    _intensity = intensity;
}