/*  GPLv3 License
 *  
 *  	Copyright (c) Divergence Meter Project by waicool20
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
 * This is the main display code which handles displaying the digits onto the nixie tube.
 * Numbers 0-11 can be written directly into the variables tube(1-8) and the display will be updated
 * once the updateDisplay() function is called.
 */

#include "display.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../constants.h"
#include "../settings.h"
#include "shiftregister.h"

const uint16_t PROGMEM brightnessLevels[10] = { 25, 50, 75, 100, 125, 150, 175,
    200, 225, 250 };  //Somehow won't work with uint8_t

Display display;
Display lastDisplayState;

static void tmr1_init() {
  TCCR1A |= (1 << COM1A1) | (1 << PWM1A);
  TCCR1B |= (1 << CS13) | (1 << CS10);  // x512 prescaler
  TCCR1D &= ~(1 << WGM11) | ~(1 << WGM10);
}

void display_init() {
  //Clear shift registers
  SRSendZeros(96);
  SRLatch();
  display_on();
  tmr1_init();
  OCR1C = 0xFF;
  OCR1A = pgm_read_word(&(brightnessLevels[settings.main[BRIGHTNESS]]));

  display_update();
}

static void handleGenericTube(uint8_t tube) {
  switch (display.tube[tube]) {
    case 0:
      SRSendZeros(10);
      SRSendOne();
      display.showRDP[tube] ? SRSendOne() : SRSendZero();
      break;
    case BLANK:
      display.showLDP[tube] ? SRSendOne() : SRSendZero();
      SRSendZeros(10);
      display.showRDP[tube] ? SRSendOne() : SRSendZero();
      break;
    default:
      display.showLDP[tube] ? SRSendOne() : SRSendZero();
      SRSendZeros(display.tube[tube] - 1);
      SRSendOne();
      SRSendZeros(10 - display.tube[tube]);
      display.showRDP[tube] ? SRSendOne() : SRSendZero();
      break;
  }
}

static void handleShiftRegister1() {
  switch (display.tube[TUBE6]) {
    case 1:
    case 2:
    case 3:
    case BLANK:
      SRSendZeros(7);
      display.showRDP[TUBE6] ? SRSendOne() : SRSendZero();
      break;
    case 0:
      SRSendZeros(6);
      SRSendOne();
      display.showRDP[TUBE6] ? SRSendOne() : SRSendZero();
      break;
    default:
      SRSendZeros(display.tube[TUBE6] - 4);
      SRSendOne();
      SRSendZeros(10 - display.tube[TUBE6]);
      display.showRDP[TUBE6] ? SRSendOne() : SRSendZero();
      break;
  }

  handleGenericTube(TUBE7);
  handleGenericTube(TUBE8);
}

static void handleShiftRegister2() {
  switch (display.tube[TUBE3]) {
    case 8:
    case 9:
      SRSendZeros(display.tube[TUBE3] - 8);
      SRSendOne();
      SRSendZeros(10 - display.tube[TUBE3]);
      display.showRDP[TUBE3] ? SRSendOne() : SRSendZero();
      break;
    case 0:
      SRSendZeros(2);
      SRSendOne();
      display.showRDP[TUBE3] ? SRSendOne() : SRSendZero();
      break;
    default:
      SRSendZeros(3);
      display.showRDP[TUBE3] ? SRSendOne() : SRSendZero();
      break;
  }

  handleGenericTube(TUBE4);
  handleGenericTube(TUBE5);

  switch (display.tube[TUBE6]) {
    case 1:
    case 2:
    case 3:
      display.showLDP[TUBE6] ? SRSendOne() : SRSendZero();
      SRSendZeros(display.tube[TUBE6] - 1);
      SRSendOne();
      SRSendZeros(3 - display.tube[TUBE6]);
      break;
    default:
      display.showLDP[TUBE6] ? SRSendOne() : SRSendZero();
      SRSendZeros(3);
      break;
  }
}

static void handleShiftRegister3() {
  handleGenericTube(TUBE1);
  handleGenericTube(TUBE2);

  switch (display.tube[TUBE3]) {
    case 8:
    case 9:
    case 0:
    case BLANK:
      display.showLDP[TUBE3] ? SRSendOne() : SRSendZero();
      SRSendZeros(7);
      break;
    default:
      display.showLDP[TUBE3] ? SRSendOne() : SRSendZero();
      SRSendZeros(display.tube[TUBE3] - 1);
      SRSendOne();
      SRSendZeros(7 - display.tube[TUBE3]);
      break;
  }
}

void display_update() {
  handleShiftRegister1();
  handleShiftRegister2();
  handleShiftRegister3();
  SRLatch();
}

void display_on() {
  DDRB &= ~(1 << HV_DISABLE);
  PORTB &= ~(1 << HV_DISABLE);
}

void display_off() {
  DDRB |= (1 << HV_DISABLE);
  PORTB |= (1 << HV_DISABLE);
}

void display_setTube(uint8_t tube, uint8_t digit, bool showRDP, bool showLDP) {
  display.tube[tube] = digit;
  display.showRDP[tube] = showRDP;
  display.showLDP[tube] = showLDP;
}

void display_showCurrentBrightness() {
  display_setTube(TUBE1, BLANK, false, false);
  display_setTube(TUBE2, BLANK, false, false);
  display_setTube(TUBE3, BLANK, false, false);
  display_setTube(TUBE4, settings.main[BRIGHTNESS] == 10 ? 1 : 0, false, false);
  display_setTube(
      TUBE5, settings.main[BRIGHTNESS] == 10 ? 0 : settings.main[BRIGHTNESS],
      false,
      false);
  display_setTube(TUBE6, BLANK, false, false);
  display_setTube(TUBE7, BLANK, false, false);
  display_setTube(TUBE8, BLANK, false, false);
  display_update();
}

void display_updateAdaptiveBrightness() {
  uint8_t adc_temp = 255 - ADCH;
  OCR1A = (adc_temp >= 250 ? 250 : (adc_temp >= 25 ? adc_temp : 25));
}

void display_setBrightness(uint8_t b) {
  if (b <= 10) {
    settings.main[BRIGHTNESS] = b;
    OCR1A = pgm_read_word(&(brightnessLevels[settings.main[BRIGHTNESS]]));
  }
}

void display_increaseBrightness() {
  if (settings.main[BRIGHTNESS] < 10) {
    settings.main[BRIGHTNESS]++;
    OCR1A = pgm_read_word(&(brightnessLevels[settings.main[BRIGHTNESS]]));
  }
}

void display_decreaseBrightness() {
  if (settings.main[BRIGHTNESS] > 0) {
    settings.main[BRIGHTNESS]--;
    OCR1A = pgm_read_word(&(brightnessLevels[settings.main[BRIGHTNESS]]));
  }
}

void display_toggleBrightness() {
  settings.main[BRIGHTNESS] =
      settings.main[BRIGHTNESS] < 10 ? settings.main[BRIGHTNESS] + 1 : 0;
  OCR1A = pgm_read_word(&(brightnessLevels[settings.main[BRIGHTNESS]]));
  settings_writeSettingsDS3232();
}

void display_saveState() {
  lastDisplayState = display;
}

void display_restoreState() {
  display = lastDisplayState;
}
