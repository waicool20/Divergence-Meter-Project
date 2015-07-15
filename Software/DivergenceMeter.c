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

#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "constants.h"
#include "i2cmaster.h"
#include "settings.h"
#include "util/display.h"
#include "util/RNG.h"

FUSES = { .low = 0x82, .high = 0xDF, .extended = 0x01, };

/* World line constants */
const uint8_t PROGMEM WORLD_LINES[32][8] = { { 0, RDP, 0, 0, 0, 0, 0, 0 }, { 0,
    RDP, 3, 2, 8, 4, 0, 3 }, { 0, RDP, 3, 3, 4, 5, 8, 1 }, { 0, RDP, 3, 3, 7, 1,
    8, 7 }, { 0, RDP, 3, 3, 7, 1, 9, 9 }, { 0, RDP, 3, 3, 7, 3, 3, 7 }, { 0,
    RDP, 4, 0, 9, 4, 2, 0 }, { 0, RDP, 4, 0, 9, 4, 3, 1 }, { 0, RDP, 4, 5, 6, 9,
    0, 3 }, { 0, RDP, 4, 5, 6, 9, 1, 4 }, { 0, RDP, 4, 5, 6, 9, 2, 3 }, { 0,
    RDP, 5, 2, 3, 2, 9, 9 }, { 0, RDP, 5, 2, 3, 3, 0, 7 }, { 0, RDP, 5, 7, 1, 0,
    1, 5 }, { 0, RDP, 5, 7, 1, 0, 2, 4 }, { 0, RDP, 5, 7, 1, 0, 4, 6 }, { 1,
    RDP, 0, 4, 8, 5, 9, 6 }, { 1, RDP, 0, 4, 8, 5, 9, 9 }, { 1, RDP, 0, 4, 9, 3,
    2, 6 }, { 1, RDP, 1, 3, 0, 4, 2, 6 }, { 2, RDP, 6, 1, 5, 0, 7, 4 }, { 3,
    RDP, 0, 1, 9, 4, 3, 0 }, { 3, RDP, 0, 3, 0, 4, 9, 3 }, { 3, RDP, 1, 3, 0, 2,
    3, 8 }, { 3, RDP, 1, 8, 2, 8, 7, 9 }, { 3, RDP, 3, 7, 2, 3, 2, 9 }, { 3,
    RDP, 3, 8, 6, 0, 1, 9 }, { 3, RDP, 4, 0, 6, 2, 8, 8 }, { 3, RDP, 4, 0, 6, 2,
    8, 8 }, { 3, RDP, 6, 0, 0, 1, 0, 4 }, { 3, RDP, 6, 6, 7, 2, 9, 3 }, { BLANK,
    RDP, 2, 7, 5, 3, 4, 9 }, };

/* Prototypes */


void DivergenceMeter_init();

void DivergenceMeter_clockMode();
void DivergenceMeter_displayCurrentTime();
void DivergenceMeter_displayCurrentDate();


void DivergenceMeter_divergenceMode();
void DivergenceMeter_showCurrentWorldLine();
void DivergenceMeter_showPrevWorldLine();
void DivergenceMeter_showNextWorldLine();

void DivergenceMeter_divergenceEditMode();
void DivergenceMeter_settingsMode();

void DivergenceMeter_rollWorldLine(bool rollTube2);
void DivergenceMeter_rollWorldLineWithDelay(bool rollTube2);

/* Volatile Variables (Modifiable by ISR)*/

volatile uint8_t current_mode = 0;
volatile uint8_t clockCount = 0;
volatile bool just_entered_mode[4] = { false, false, false };
volatile uint16_t button_count[5] = { 0, 0, 0, 0, 0 };

volatile bool button_is_pressed[5] = { false, false, false, false, false };
volatile bool button_short_pressed[5] = { false, false, false, false, false };
volatile bool button_long_pressed[5] = { false, false, false, false, false };
volatile uint8_t button_time_since_last_pressed[5] = { 0, 0, 0, 0, 0 };

/* Normal Variables */

bool shouldRoll = false;
uint8_t currentWorldLineIndex = 0;
uint8_t currentTube = 0;

/* Main Code Start */

int main() {
  DivergenceMeter_init();
  while (1) {
    switch (current_mode) {
      case CLOCK_MODE:
        DivergenceMeter_clockMode();
        break;
      case DIVERGENCE_MODE:
        DivergenceMeter_divergenceMode();
        break;
      case DIVERGENCE_EDIT_MODE:
        DivergenceMeter_divergenceEditMode();
        break;
      case SETTINGS_MODE:
        DivergenceMeter_settingsMode();
        break;
    }
  }
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sleep_bod_disable();
  sei();
  power_adc_disable();
  power_usi_disable();
  sleep_disable();
  power_adc_enable();
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
  ADMUX = (1 << ADLAR) | 0x07;
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
}

void DivergenceMeter_init() {
  //Initialize IO
  DDRA &= ~(1 << ALARM_INT) | ~(1 << BUTTON1_PIN) | ~(1 << BUTTON2_PIN)
      | ~(1 << BUTTON3_PIN) | ~(1 << BUTTON4_PIN) | ~(1 << BUTTON5_PIN);
  DDRB = (1 << LE) | (1 << BL) | (1 << CLK) | (1 << DIN) | (1 << SPEAKER);
  MCUCR = (1 << PUD);
  PORTB = 0x00;

  //Global interrupt enable
  sei();

  i2c_init();
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
    if (button_count[i] >= (BUTTON_LONG_PRESS_MIN_DURATION_MS / 10)) {
      button_is_pressed[i] = true;
      button_short_pressed[i] = false;
      button_long_pressed[i] = true;
    }
    if (button_count[i] > (BUTTON_SHORT_PRESS_MIN_DURATION_MS / 10)
        && button_count[i] <= (BUTTON_SHORT_PRESS_MAX_DURATION_MS / 10)
        && !button_long_pressed[i]) {
      button_is_pressed[i] = true;
      button_short_pressed[i] = true;
      button_long_pressed[i] = false;
    }
    if (button_time_since_last_pressed[i] < 255) {
      button_time_since_last_pressed[i]++;
    }
  }

  if ((button_short_pressed[BUTTON1] || button_long_pressed[BUTTON1])
      && button_time_since_last_pressed[BUTTON1] > 10) {
    if (current_mode < SETTINGS_MODE) {
      current_mode++;
    } else {
      current_mode = 0;
    }
    _delay_ms(200);
    just_entered_mode[current_mode] = true;
    button_time_since_last_pressed[BUTTON1] = 0;
  }
  clockCount++;
  if(clockCount > 9){
    settings_readDS3232();
    clockCount = 0;
  }
  TCNT0H = 0x00;
  TCNT0L = 0x00;
}

/* Clock Mode Code */

void DivergenceMeter_clockMode() {
  DivergenceMeter_displayCurrentTime();
  switch (settings.seconds){
    case 0x00:
      shouldRoll = true;
      DivergenceMeter_rollWorldLine(false);
      DivergenceMeter_displayCurrentDate();
      shouldRoll = false;
      _delay_ms((DATE_DISPLAY_SECONDS * 1000));
      break;
  }
  if (button_short_pressed[BUTTON2] || button_long_pressed[BUTTON2]) {
    DivergenceMeter_displayCurrentDate();
    _delay_ms((DATE_DISPLAY_SECONDS * 1000));
  } else if (button_short_pressed[BUTTON3] || button_long_pressed[BUTTON3]) {
    shouldRoll = true;
    DivergenceMeter_rollWorldLineWithDelay(true);
  } else if (button_short_pressed[BUTTON4]) {

  } else if (button_short_pressed[BUTTON5] || button_long_pressed[BUTTON5]) {
    display_toggleBrightness();
    display_showCurrentBrightness();
    _delay_ms(BRIGHTNESS_DISPLAY_MS);
  }
  just_entered_mode[CLOCK_MODE] = false;
}

void DivergenceMeter_displayCurrentTime() {
  display.tube[TUBE1] = (settings.hours >> 4) & 0x01;
  display.tube[TUBE2] = settings.hours & 0x0F;
  display.tube[TUBE3] = (settings.seconds & 0x01) ? LDP : RDP;
  display.tube[TUBE4] = (settings.minutes >> 4);
  display.tube[TUBE5] = settings.minutes & 0x0F;
  display.tube[TUBE6] = (settings.seconds & 0x01) ? LDP : RDP;
  display.tube[TUBE7] = (settings.seconds >> 4);
  display.tube[TUBE8] = settings.seconds & 0x0F;
  display_update();
}

void DivergenceMeter_displayCurrentDate() {
  display.tube[TUBE1] = (settings.date >> 4) & 0x03;
  display.tube[TUBE2] = settings.date & 0x0F;
  display.tube[TUBE3] = LDP;
  display.tube[TUBE4] = (settings.month >> 4) & 0x01;
  display.tube[TUBE5] = settings.month & 0x0F;
  display.tube[TUBE3] = RDP;
  display.tube[TUBE7] = (settings.year >> 4);
  display.tube[TUBE8] = settings.year & 0x0F;
  display_update();
}

/* Divergence Mode Code */

void DivergenceMeter_divergenceMode() {
  if (just_entered_mode[DIVERGENCE_MODE]) {
    DivergenceMeter_rollWorldLineWithDelay(true);
  }
  if (button_short_pressed[BUTTON2] && button_short_pressed[BUTTON4]) {
    current_mode = DIVERGENCE_EDIT_MODE;
    just_entered_mode[DIVERGENCE_EDIT_MODE] = true;
  } else if (button_short_pressed[BUTTON2]) {
    DivergenceMeter_rollWorldLine(true);
    DivergenceMeter_showPrevWorldLine();
    _delay_ms(200);
  } else if (button_short_pressed[BUTTON3] || button_long_pressed[BUTTON3]) {
    shouldRoll = true;
    DivergenceMeter_rollWorldLine(true);
  } else if (button_short_pressed[BUTTON4]) {
    DivergenceMeter_rollWorldLine(true);
    DivergenceMeter_showNextWorldLine();
    _delay_ms(200);
  } else if (button_short_pressed[BUTTON5] || button_long_pressed[BUTTON5]) {
    display_saveState();
    display_toggleBrightness();
    display_showCurrentBrightness();
    _delay_ms(BRIGHTNESS_DISPLAY_MS);
    display_restoreState();
    display_update();
  }
  just_entered_mode[DIVERGENCE_MODE] = false;
}

void DivergenceMeter_showCurrentWorldLine() {
  display.tube[TUBE1] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][0]));
  display.tube[TUBE2] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][1]));
  display.tube[TUBE3] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][2]));
  display.tube[TUBE4] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][3]));
  display.tube[TUBE5] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][4]));
  display.tube[TUBE6] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][5]));
  display.tube[TUBE7] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][6]));
  display.tube[TUBE8] = pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][7]));
  display_update();
}

void DivergenceMeter_showNextWorldLine() {
  if (currentWorldLineIndex < 31) {
    currentWorldLineIndex++;
  } else {
    currentWorldLineIndex = 0;
  }
  DivergenceMeter_showCurrentWorldLine();
}

void DivergenceMeter_showPrevWorldLine() {
  if (currentWorldLineIndex > 0) {
    currentWorldLineIndex--;
  } else {
    currentWorldLineIndex = 31;
  }
  DivergenceMeter_showCurrentWorldLine();
}

/* Divergence Edit Mode */

void DivergenceMeter_divergenceEditMode() {
  if (just_entered_mode[DIVERGENCE_EDIT_MODE]) {
    currentTube = 0;
    just_entered_mode[DIVERGENCE_EDIT_MODE] = false;
    display.tube[TUBE1] = 0;
    display.tube[TUBE2] = BLANK;
    display.tube[TUBE3] = BLANK;
    display.tube[TUBE4] = BLANK;
    display.tube[TUBE5] = BLANK;
    display.tube[TUBE6] = BLANK;
    display.tube[TUBE7] = BLANK;
    display.tube[TUBE8] = BLANK;
    display_update();
  }

  if (button_short_pressed[BUTTON3]) {
    if (currentTube < 7) {
      if (currentTube == TUBE1) {
        display.tube[TUBE2] = RDP;
        currentTube = TUBE3;  //SKIP TUBE2
      } else {
        currentTube++;
      }
      display.tube[currentTube] = 0;
      display_update();
    } else {
      currentTube = 0;
      current_mode = DIVERGENCE_MODE;
      display_saveState();
      DivergenceMeter_rollWorldLine(false);
      display_restoreState();
      display_update();
    }
    _delay_ms(200);
  } else if (button_short_pressed[BUTTON5]){
    if (currentTube > 0) {
      display.tube[currentTube] = BLANK;
      if (currentTube == TUBE3) {
        currentTube = 1;
      } else {
        currentTube--;
      }
      display_update();
    }
    _delay_ms(200);
  }
  if (button_short_pressed[BUTTON2]) {
    if (display.tube[currentTube] == BLANK) {
      display.tube[currentTube] = 9;
    } else if (display.tube[currentTube] > 0) {
      display.tube[currentTube]--;
    }
    display_update();
    _delay_ms(200);
  } else if (button_short_pressed[BUTTON4]) {
    if (display.tube[currentTube] < 9) {
      display.tube[currentTube]++;

    } else if (display.tube[currentTube] == 9) {
      display.tube[currentTube] = BLANK;
    }
    display_update();
    _delay_ms(200);
  }
}

/* Settings Mode Code */

void DivergenceMeter_settingsMode() {
  display.tube[TUBE1] = 1;
  display.tube[TUBE2] = 2;
  display.tube[TUBE3] = 3;
  display.tube[TUBE4] = 4;
  display.tube[TUBE5] = 5;
  display.tube[TUBE6] = 6;
  display.tube[TUBE7] = 7;
  display.tube[TUBE8] = 8;
  display_update();
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
