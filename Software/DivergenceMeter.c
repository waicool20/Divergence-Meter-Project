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

#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>

#include <stdbool.h>

#include "constants.h"
#include "i2cmaster.h"
#include "settings.h"
#include "util/display.h"
#include "util/RNG.h"

FUSES = { .low = 0x82, .high = 0xDF, .extended = 0x01, };

int main() {
  DivergenceMeter_init();
  while (1) {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sleep_bod_disable();
    sei();
    power_adc_disable();
    power_usi_disable();
    sleep_disable();
  }
  return 0;
}

void tmr0_init() {
  TCCR0A = (1 << TCW0);  //16 bit width
  TCCR0B = (1 << CS01);  //x8 prescaler
  unsigned int val = 10000;  // 10ms interval calculated by (Seconds)/(1/(((System Clock))/prescaler))
  OCR0B = (val >> 8);
  OCR0A = (unsigned char) val;
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
  DDRA &= ~(1 << ALARM_INT) | ~(1 << BUTTON1) | ~(1 << BUTTON2)
      | ~(1 << BUTTON3) | ~(1 << BUTTON4) | ~(1 << BUTTON5);
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

void DivergenceMeter_rollWorldLine() {
  for (char i = ((ROLL_SECONDS * 1000) / ROLL_INTERVAL_MS); i > 0; i--) {
    display.tube1 = RNG_nextChar() == 9 && i == 1 ? BLANK : RNG_nextChar();
    display.tube2 = RDP;
    display.tube3 = RNG_nextChar();
    display.tube4 = RNG_nextChar();
    display.tube5 = RNG_nextChar();
    display.tube6 = RNG_nextChar();
    display.tube7 = RNG_nextChar();
    display.tube8 = RNG_nextChar();
    display_update();
    _delay_ms(ROLL_INTERVAL_MS);
  }
  _delay_ms(DISPLAY_SECONDS * 1000);
}

unsigned char mode = 0;
unsigned char updateCount = 0;
unsigned char buttons[5] = { 0, 0, 0, 0, 0 };

typedef struct{
  bool BUTTON1_SHORT_PRESS;
  bool BUTTON2_SHORT_PRESS;
  bool BUTTON3_SHORT_PRESS;
  bool BUTTON4_SHORT_PRESS;
  bool BUTTON5_SHORT_PRESS;

  bool BUTTON1_LONG_PRESS;
  bool BUTTON2_LONG_PRESS;
  bool BUTTON3_LONG_PRESS;
  bool BUTTON4_LONG_PRESS;
  bool BUTTON5_LONG_PRESS;
} BUTTON_FLAGS;

ISR(TIMER0_COMPA_vect) {
  for (int i = 4; i >= 0; i--) {
    buttons[i] =
        (bit_is_set(PINA, i+3) && buttons[i] < 255) ?
            (buttons[i] + 1) : 0;
    if (buttons[i] > 8 && buttons[i] <= 9) {
      //display_on();
      DivergenceMeter_rollWorldLine();
    } else if (buttons[i] >= 100) {
      switch (updateCount) {
        case 20:
        case 40:
        case 60:
        case 80:
        case 100:
          //display_off();
          DivergenceMeter_rollWorldLine();
          break;
      }
    }
  }

  switch (mode) {
    case CLOCK_MODE:
      DivergenceMeter_clockMode();
      break;
    case DIVERGENCE_MODE:
      break;
    case SETTINGS_MODE:
      break;
  }

  updateCount++;
  if (updateCount > 100)
    updateCount = 0;
  TCNT0H = 0x00;
  TCNT0L = 0x00;
}

void DivergenceMeter_clockMode() {
  settings_readDS3232();
  DivergenceMeter_displayCurrentTime();
}

void DivergenceMeter_displayCurrentTime() {
  display.tube1 = (settings.hours >> 4) & 0x01;
  display.tube2 = settings.hours & 0x0F;

  display.tube4 = (settings.minutes >> 4);
  display.tube5 = settings.minutes & 0x0F;

  display.tube7 = (settings.seconds >> 4);
  display.tube8 = settings.seconds & 0x0F;
  display_update();
}

void DivergenceMeter_displayCurrentDate() {
  display.tube1 = (settings.date >> 4) & 0x02;
  display.tube2 = settings.date & 0x0F;

  display.tube4 = (settings.month >> 4) & 0x01;
  display.tube5 = settings.month & 0x0F;

  display.tube7 = (settings.year >> 4);
  display.tube8 = settings.year & 0x0F;
  display_update();
}

