/*  GPLv3 License
 *
 *   Copyright (c) Divergence Meter Project by waicool20
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Main Program Entry of the Divergence Meter Project.
 * General logic will be implemented here.
 */

#include "DivergenceMeter.h"

#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>

#include "constants.h"
#include "modes/clockMode.h"
#include "modes/divergenceEditMode.h"
#include "modes/divergenceMode.h"
#include "modes/settingsMode.h"
#include "settings.h"
#include "util/display.h"
#include "util/RNG.h"


FUSES = { .low = 0x82, .high = 0xDF, .extended = 0x01, };

/* Prototypes */

void DivergenceMeter_init();

/* Volatile Variables (Modifiable by ISR)*/

volatile uint8_t clockCount = 0;

volatile uint8_t current_mode = 0;
volatile bool just_entered_mode[6] =
    { false, false, false, false, false, false };

volatile uint16_t button_count[5] = { 0, 0, 0, 0, 0 };
volatile bool button_is_pressed[5] = { false, false, false, false, false };
volatile bool button_short_pressed[5] = { false, false, false, false, false };
volatile bool button_long_pressed[5] = { false, false, false, false, false };

/* Normal Variables */

bool shouldRoll = false;

/* Main Code Start */

int main() {
  DivergenceMeter_init();
  while (1) {
    switch (current_mode) {
      case CLOCK_MODE:
        clockMode_run();
        break;
      case DIVERGENCE_MODE:
        divergenceMode_run();
        break;
      case DIVERGENCE_EDIT_MODE:
        divergenceEditMode_run();
        break;
      case SETTINGS_MODE:
        settingsMode_run();
        break;
    }
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable()
    ;
    sleep_bod_disable();
    sei();
    power_adc_disable();
    power_usi_disable();
    sleep_disable()
    ;
    power_adc_enable();
  }
  return 0;
}

void tmr0_init() {
  TCCR0A = (1 << TCW0);  //16 bit width
  TCCR0B = (1 << CS01);  //x8 prescaler
  unsigned int val = 10000;  // 10ms interval calculated by (Seconds)/(1/(((System Clock))/prescaler))
  OCR0B = (val >> 8);
  OCR0A = (uint8_t) val;
  TIMSK = (1 << OCIE0A);  //Compare Match interrupt enable
}

void ADC_init() {
  ADMUX = (1 << ADLAR) | 0x07;  //ADC channel 7
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADATE);
  ADCSRB = (1 << ADTS1) | (1 << ADTS0);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
}

void DivergenceMeter_init() {
  //Initialize IO
  DDRA &= ~((1 << ALARM_INT) | (1 << BUTTON1_PIN) | (1 << BUTTON2_PIN)
      | (1 << BUTTON3_PIN) | (1 << BUTTON4_PIN) | (1 << BUTTON5_PIN));
  DDRB = (1 << LE) | (1 << BL) | (1 << CLK) | (1 << DIN) | (1 << SPEAKER);
  MCUCR = (1 << PUD);

  //Global interrupt enable
  sei();

  settings_init();
  ADC_init();
  RNG_init();
  tmr0_init();
  display_init();
}

/* Timer0 Interrupt Code, ran every 10ms as configured*/

ISR(TIMER0_COMPA_vect) {
  for (int i = 4; i >= 0; i--) {
    if (bit_is_set(PINA, i+3) && button_count[i] < 65535) {
      button_count[i]++;
    } else {
      button_count[i] = 0;
      button_is_pressed[i] = false;
      button_short_pressed[i] = false;
      button_long_pressed[i] = false;
    }

    if (button_count[i] > (BUTTON_LONG_PRESS_MIN_DURATION_MS / 10)) {
      button_is_pressed[i] = true;
      button_short_pressed[i] = false;
      button_long_pressed[i] = true;
    } else if (button_count[i] > (BUTTON_SHORT_PRESS_MAX_DURATION_MS / 10)) {
      button_is_pressed[i] = true;
      button_short_pressed[i] = false;
      button_long_pressed[i] = false;
    } else if (button_count[i] >= (BUTTON_SHORT_PRESS_MIN_DURATION_MS / 10)) {
      button_is_pressed[i] = true;
      button_short_pressed[i] = true;
      button_long_pressed[i] = false;
    }
  }

  if (button_short_pressed[BUTTON1]) {
    if (current_mode < DIVERGENCE_MODE) {
      current_mode++;
    } else {
      current_mode = CLOCK_MODE;
    }
    _delay_ms(200);
    just_entered_mode[current_mode] = true;
  } else if (button_long_pressed[BUTTON1] && current_mode != SETTINGS_MODE) {
    current_mode = SETTINGS_MODE;
    just_entered_mode[current_mode] = true;
  }
  if (settings.main[BRIGHTNESS] == 10) {
    display_updateAdaptiveBrightness();
  }
  clockCount++;
  if (clockCount > 9) {
    settings_readTimeDS3232();
    clockCount = 0;
  }
  TCNT0H = 0x00;
  TCNT0L = 0x00;
}

/* Misc Code */

bool DivergenceMeter_shouldNotRoll() {
  return current_mode != DIVERGENCE_MODE && !shouldRoll;
}

void DivergenceMeter_rollWorldLine(bool rollTube2) {
  for (uint8_t i = ((ROLL_SECONDS * 1000) / ROLL_INTERVAL_MS); i > 0; i--) {
    if (DivergenceMeter_shouldNotRoll()) {
      return;
    }
    if (i > 1) {
      display.tube[TUBE1] = RNG_nextChar();
    } else {
      display.tube[TUBE1] =
          RNG_nextChar() == 9 ? BLANK : (RNG_nextChar() == 8 ? 1 : 0);
    }
    display.tube[TUBE2] = rollTube2 ? RDP : RNG_nextChar();
    display.tube[TUBE3] = RNG_nextChar();
    display.tube[TUBE4] = RNG_nextChar();
    display.tube[TUBE5] = RNG_nextChar();
    display.tube[TUBE6] = RNG_nextChar();
    display.tube[TUBE7] = RNG_nextChar();
    display.tube[TUBE8] = RNG_nextChar();
    display_update();
    for (int i = (ROLL_INTERVAL_MS / 10); i > 0; i--) {
      if (DivergenceMeter_shouldNotRoll()) {
        return;
      }
      _delay_ms(10);
    }
  }
}

void DivergenceMeter_rollWorldLineWithDelay(bool rollTube2) {
  DivergenceMeter_rollWorldLine(rollTube2);
  for (int i = (ROLL_DISPLAY_SECONDS * 100); i > 0; i--) {
    if (DivergenceMeter_shouldNotRoll()) {
      return;
    }
    _delay_ms(10);
  }
}

void DivergenceMeter_showBrightness() {
  display_showCurrentBrightness();
  _delay_ms(BRIGHTNESS_DISPLAY_MS);
}
