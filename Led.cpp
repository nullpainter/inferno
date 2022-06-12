#include <Arduino.h>
#include "Led.h"
#include "LedControl.h"

Led::Led(int pin)
{
    Serial.printf("[LED %d] Initialising");

    _pin = pin;
    pinMode(pin, OUTPUT);

    _intensity = MAX_INTENSITY;
    _maxIntensity = MAX_INTENSITY;

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
                int index = round(elapsed / (float)_fadeDuration * MAX_INTENSITY);
                if (_ledState == FadeOut)
                {
                    index = MAX_INTENSITY - index;
                }

                _maxIntensity = LOG_CURVE[index];
            }
            else
            {
                Serial.printf("[LED %d] Completed fade\n", _pin);

                // If fading has completed, reset state to normal
                _maxIntensity = _ledState == FadeIn ? MAX_INTENSITY : 0;
                _ledState = Normal;
            }
        }

        break;
    }

    int scaledIntensity = round(_intensity / (float)MAX_INTENSITY * _maxIntensity);
    analogWrite(_pin, scaledIntensity);
}

void Led::fadeIn(int duration)
{
    Serial.printf("[LED %d] Setting fade in for duration %d\n", _pin, duration);

    _startMillis = millis();
    _fadeDuration = duration;
    _fadeDelay = 0;

    _ledState = FadeIn;
}

void Led::fadeOut(int duration, int delay)
{
    Serial.printf("[LED %d] Setting fade out for duration %d and delay %d\n", _pin, duration, delay);

    _startMillis = millis();

    _fadeDuration = duration;
    _fadeDelay = delay;

    _ledState = FadeOut;
}

void Led::on()
{
    Serial.printf("[LED %d] Setting max intensity to %d\n", _pin, _maxIntensity);
    _maxIntensity = MAX_INTENSITY;
}

void Led::off()
{
    Serial.printf("[LED %d] Setting max intensity to %d\n", _pin, _maxIntensity);
    _maxIntensity = 0;
}

void Led::toggle()
{
    Serial.printf("[LED %d] toggling\n", _pin);
    if (_intensity > 0)
    {
        setIntensity(0);
    }
    else
    {
        setIntensity(MAX_INTENSITY);
    }
}

void Led::setIntensity(int intensity)
{
    _intensity = intensity;
}