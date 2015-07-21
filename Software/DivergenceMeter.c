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

#include "constants.h"
#include "modes/clockMode.h"
#include "modes/clockSetMode.h"
#include "modes/divergenceEditMode.h"
#include "modes/divergenceMode.h"
#include "modes/settingsMode.h"
#include "modes/restMode.h"
#include "settings.h"
#include "util/display.h"
#include "util/RNG.h"

FUSES = { .low = 0xE2, .high = 0xDF, .extended = 0x01, };

/* Prototypes */

static void DivergenceMeter_init();

/* Volatile Variables (Modifiable by ISR)*/

static volatile uint8_t clockCount;
static volatile uint16_t delayCount;
static volatile uint8_t currentMode;

volatile bool justEnteredMode[7];

volatile uint16_t buttonCount[5];
volatile bool buttonIsPressed[5];
volatile bool buttonShortPressed[5];
volatile bool buttonLongPressed[5];

/* Normal Variables */

bool shouldRoll = false;

/* Main Code Start */

int main() {
  DivergenceMeter_init();
  while (1) {
    switch (currentMode) {
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
      case CLOCK_SET_MODE:
        clockSetMode_run();
        break;
      case REST_MODE:
        restMode_run();
        break;
    }
    DivergenceMeter_sleep();
  }
  return 0;
}

static void tmr0_init() {
  TCCR0A = (1 << TCW0);  //16 bit width
  TCCR0B = (1 << CS01);  //x8 prescaler
  uint16_t val = 10000;  // 10ms interval calculated by (Seconds)/(1/(((System Clock))/prescaler))
  OCR0B = (val >> 8);
  OCR0A = (uint8_t) val;
  TIMSK = (1 << OCIE0A);  //Compare Match interrupt enable
}

static void ADC_init() {
  ADMUX = (1 << ADLAR) | 0x07;  //ADC channel 7
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADATE);
  ADCSRB = (1 << ADTS1) | (1 << ADTS0);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
}

static void DivergenceMeter_init() {
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
  for (int8_t i = 4; i >= 0; i--) {
    if (bit_is_set(PINA, i+3) && buttonCount[i] < 65535) {
      buttonCount[i]++;
    } else {
      buttonCount[i] = 0;
      buttonIsPressed[i] = false;
      buttonShortPressed[i] = false;
      buttonLongPressed[i] = false;
    }

    if (buttonCount[i] > (BUTTON_LONG_PRESS_MIN_DURATION_MS / 10)) {
      buttonIsPressed[i] = true;
      buttonShortPressed[i] = false;
      buttonLongPressed[i] = true;
    } else if (buttonCount[i] > (BUTTON_SHORT_PRESS_MAX_DURATION_MS / 10)) {
      buttonIsPressed[i] = true;
      buttonShortPressed[i] = false;
      buttonLongPressed[i] = false;
    } else if (buttonCount[i] >= (BUTTON_SHORT_PRESS_MIN_DURATION_MS / 10)) {
      buttonIsPressed[i] = true;
      buttonShortPressed[i] = true;
      buttonLongPressed[i] = false;
    }
  }
  if (buttonShortPressed[BUTTON1]) {
    switch (currentMode){
      case SETTINGS_MODE:
        settings_writeSettingsDS3232();
        break;
      case REST_MODE:
        display_on();
        break;
    }
    currentMode = currentMode < DIVERGENCE_MODE ? currentMode + 1 : CLOCK_MODE;
    justEnteredMode[currentMode] = true;
  } else if (buttonLongPressed[BUTTON1] && currentMode != SETTINGS_MODE) {
    DivergenceMeter_switchMode(SETTINGS_MODE, false);
  }
  if (settings.main[BRIGHTNESS] == 10) {
    display_updateAdaptiveBrightness();
  }
  if (++clockCount > 9 && currentMode != CLOCK_SET_MODE) {
    settings_readTimeDS3232();
    clockCount = 0;
  }
  if(delayCount > 0){
    delayCount--;
  }
  TCNT0H = 0x00;
  TCNT0L = 0x00;
}

/* Misc Code */

static bool DivergenceMeter_shouldNotRoll() {
  return currentMode != DIVERGENCE_MODE && !shouldRoll;
}

void DivergenceMeter_rollWorldLine(bool rollTube2) {
  for (uint8_t i = ((ROLL_SECONDS * 1000) / ROLL_INTERVAL_MS); i > 0; i--) {
    if (DivergenceMeter_shouldNotRoll()) {
      return;
    }
    uint8_t digit = i > 1 ? RNG_nextChar() : (RNG_nextChar() == 9 ? BLANK : (RNG_nextChar() == 8 ? 1 : 0));
    display_setTube(TUBE1, digit,false,false);
    rollTube2 ? display_setTube(TUBE2,BLANK,true,false) : display_setTube(TUBE2,RNG_nextChar(),false,false);
    display_setTube(TUBE3, RNG_nextChar(),false,false);
    display_setTube(TUBE4, RNG_nextChar(),false,false);
    display_setTube(TUBE5, RNG_nextChar(),false,false);
    display_setTube(TUBE6, RNG_nextChar(),false,false);
    display_setTube(TUBE7, RNG_nextChar(),false,false);
    display_setTube(TUBE8, RNG_nextChar(),false,false);
    display_update();
    for (uint8_t i = (ROLL_INTERVAL_MS / 10); i > 0; i--) {
      if (DivergenceMeter_shouldNotRoll()) {
        return;
      }
      DivergenceMeter_delayCS(s2cs(0.01));
    }
  }
}

void DivergenceMeter_rollWorldLineWithDelay(bool rollTube2) {
  DivergenceMeter_rollWorldLine(rollTube2);
  for (uint16_t i = (ROLL_DISPLAY_SECONDS * 100); i > 0; i--) {
    if (DivergenceMeter_shouldNotRoll()) {
      return;
    }
    DivergenceMeter_delayCS(s2cs(0.01));
  }
}

void DivergenceMeter_showBrightness() {
  display_showCurrentBrightness();
  DivergenceMeter_delayCS(s2cs(BRIGHTNESS_DISPLAY_S));
}

void DivergenceMeter_delayCS(uint16_t delay_cs){
  delayCount = delay_cs;
  while(delayCount){
    DivergenceMeter_sleep();
  }
}

void DivergenceMeter_sleep(){
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sleep_bod_disable();
  sei();
  power_adc_disable();
  power_usi_disable();
  sleep_disable();
  power_adc_enable();
}

void DivergenceMeter_switchMode(uint8_t mode, bool silent){
  currentMode = mode;
  justEnteredMode[mode] = !silent;
}
