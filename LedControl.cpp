#include "LedControl.h"

CHSVPalette16 LedControl::_palette = CHSVPalette16(
                                       CHSV(RED, 255, 255),
                                       CHSV(RED, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(ORANGE, 255, 255),
                                       CHSV(YELLOW, 255, 255),
                                       CHSV(YELLOW, 255, 255),
                                       CHSV(YELLOW, 255, 255));

CRGB LedControl::_leds[NUM_LEDS];

FlameMode LedControl::_flameMode;
FadeState LedControl::_fadeState;

int LedControl::_fadeDuration;
int LedControl::_fadeDelay;
long LedControl::_fadeStart;

void LedControl::setup()
{
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(_leds, NUM_LEDS).setCorrection(TypicalPixelString);
  FastLED.setBrightness(MAX_BRIGHTNESS);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = ColorFromPalette(_palette, random8());
  }
}

uint8_t offset;

void LedControl::update()
{

  switch (_flameMode)
  {
    case FlameMode::Flame:
      EVERY_N_MILLISECONDS(100)
      {
        _leds[random8(NUM_LEDS)] = ColorFromPalette(_palette, random8());
      }

      EVERY_N_MILLISECONDS(10)
      {
        // Flicker LEDs
        fadeToBlackBy(_leds, NUM_LEDS, 1);
      }

      break;
    case FlameMode::Party:
      EVERY_N_MILLISECONDS(5)
      {
        offset++;
        fill_rainbow(_leds, NUM_LEDS, offset);
      }

      break;

    case FlameMode::Offline:
      uint8_t intensity;

      EVERY_N_MILLISECONDS(5)
      {
        uint8_t intensity = cubicwave8(offset++);
        fill_solid(_leds, NUM_LEDS, CHSV(OFFLINE, 255, intensity));
      }

      break;
  }

  uint8_t brightness = getBrightness();
  resetFadeState(brightness);

  FastLED.setBrightness(brightness);
  FastLED.show();
}

void LedControl::resetFadeState(uint8_t brightness)
{
  if (brightness == 0 && _fadeState == FadeState::FadeOut)
  {
    Serial.println("[LedControl] Resetting state to off after fade out");
    off();
  }
  else if (brightness == MAX_BRIGHTNESS && _fadeState == FadeState::FadeIn)
  {
    Serial.println("[LedControl] Resetting state to on after fade in");
    on();
  }
}

uint8_t LedControl::getBrightness()
{
  long elapsedMillis = millis() - _fadeStart;

  switch (_fadeState)
  {
    case FadeState::FadeOut:
      if (elapsedMillis < _fadeDelay)
      {
        return FastLED.getBrightness();
      }

      return constrain(MAX_BRIGHTNESS - round((elapsedMillis - _fadeDelay) / (float)_fadeDuration * MAX_BRIGHTNESS), 0, MAX_BRIGHTNESS);
    case FadeState::FadeIn:
      if (elapsedMillis < _fadeDelay)
      {
        return FastLED.getBrightness();
      }

      return constrain(round((elapsedMillis - _fadeDelay) / (float)_fadeDuration * MAX_BRIGHTNESS), 0, MAX_BRIGHTNESS);
    default:
      return FastLED.getBrightness();
  }
}

void LedControl::fadeOut(int delay, int duration)
{
  Serial.printf("[LedControl] Fading out for %dms with %d delay\n", duration, delay);

  _fadeState = FadeState::FadeOut;
  _fadeDuration = duration;
  _fadeDelay = delay;

  _fadeStart = millis();
}

void LedControl::fadeIn(int delay, int duration)
{
  Serial.printf("[LedControl] Fading in for %dms with %d delay\n", duration, delay);

  _fadeState = FadeState::FadeIn;
  _fadeDuration = duration;
  _fadeDelay = delay;

  _fadeStart = millis();
}

void LedControl::on()
{
  Serial.println("[LedControl] Turning on");

  _fadeState = FadeState::On;
  FastLED.setBrightness(MAX_BRIGHTNESS);
}

void LedControl::off()
{
  Serial.println("[LedControl] Turning off");

  _fadeState = FadeState::Off;
  FastLED.setBrightness(0);
}

void LedControl::setMode(FlameMode mode)
{
  Serial.printf("[LedControl] Setting mode to %d\n", mode);

  _flameMode = mode;
}
