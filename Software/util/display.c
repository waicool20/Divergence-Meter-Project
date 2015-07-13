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

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../constants.h"
#include "../settings.h"
#include "shiftregister.h"

unsigned char brightness;
const unsigned char brightnessLevels[11] PROGMEM = {0,25,50,75,100,125,150,175,200,225,250};

Display display;

void display_init() {
  //Clear shift registers
  SRSendZeros(96);
  SRLatch();

  //Setup Timer1 with Fast PWM mode with x1 prescaler
  TCCR1A |= (1<<COM1A1) | (1<<PWM1A);
  TCCR1B |= (1<<CS10);
  brightness = settings.brightness;
  OCR1A = pgm_read_byte(&(brightnessLevels[brightness]));

  display_on();
  display_update();
}

void handleShiftRegister1() {
  switch (display.tube6) {
    case 1:
    case 2:
    case 3:
      SRSendZeros(8);
      break;
    case 0:
      SRSendZeros(6);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(7);
      SRSendOnes(1);
      break;
    case BLANK:
      SRSendZeros(8);
      break;
    default:
      SRSendZeros(display.tube6 - 4);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube6);
      break;
  }

  switch (display.tube7) {
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
    case BLANK:
      SRSendZeros(12);
      break;
    default:
      SRSendZeros(display.tube7);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube7);
      break;
  }

  switch (display.tube8) {
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
    case BLANK:
      SRSendZeros(12);
      break;
    default:
      SRSendZeros(display.tube8);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube8);
      break;
  }
}

void handleShiftRegister2() {
  switch (display.tube3) {
    case 8:
    case 9:
      SRSendZeros(display.tube3 - 8);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube3);
      break;
    case 0:
      SRSendZeros(2);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(3);
      SRSendOnes(1);
      break;
    case BLANK:
      SRSendZeros(4);
      break;
    default:
      SRSendZeros(4);
      break;
  }

  switch (display.tube4) {
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
    case BLANK:
      SRSendZeros(12);
      break;
    default:
      SRSendZeros(display.tube4);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube4);
      break;
  }

  switch (display.tube5) {
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
    case BLANK:
      SRSendZeros(12);
      break;
    default:
      SRSendZeros(display.tube5);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube5);
      break;
  }

  switch (display.tube6) {
    case 1:
    case 2:
    case 3:
      SRSendZeros(display.tube6);
      SRSendOnes(1);
      SRSendZeros(3 - display.tube6);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(3);
      break;
    case BLANK:
      SRSendZeros(4);
      break;
    default:
      SRSendZeros(4);
      break;
  }
}

void handleShiftRegister3() {
  switch (display.tube1) {
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
    case BLANK:
      SRSendZeros(12);
      break;
    default:
      SRSendZeros(display.tube1);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube1);
      break;
  }
  switch (display.tube2) {
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
    case BLANK:
      SRSendZeros(12);
      break;
    default:
      SRSendZeros(display.tube2);
      SRSendOnes(1);
      SRSendZeros(11 - display.tube2);
  }

  switch (display.tube3) {
    case 8:
    case 9:
    case 0:
    case RDP:
      SRSendZeros(8);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(7);
      break;
    case BLANK:
      SRSendZeros(8);
      break;
    default:
      SRSendZeros(display.tube3);
      SRSendOnes(1);
      SRSendZeros(7 - display.tube3);
      break;
  }
}

void display_update() {
  handleShiftRegister1();
  handleShiftRegister2();
  handleShiftRegister3();
  SRLatch();
}

void display_on(){
  DDRB &= ~(1<<HV_DISABLE);
  PORTB &= ~(1<<HV_DISABLE);
}

void display_off(){
  DDRB |= (1<<HV_DISABLE);
  PORTB |= (1<<HV_DISABLE);
}

void display_increaseBrightness(){
  if(brightness < 10){
    brightness++;
    OCR1A = pgm_read_byte(&(brightnessLevels[brightness]));
  }
}

void display_decreaseBrightness(){
  if(brightness > 0){
    brightness--;
    OCR1A = pgm_read_byte(&(brightnessLevels[brightness]));
  }
}
