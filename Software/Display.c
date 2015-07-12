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

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Display.h"
#include "ShiftRegister.h"
#include "Constants.h"

char tube1 = 0;
char tube2 = 0;
char tube3 = 0;
char tube4 = 0;
char tube5 = 0;
char tube6 = 0;
char tube7 = 0;
char tube8 = 0;

unsigned char brightness = 10;
const unsigned char brightnessLevels[11] PROGMEM = {0,25,50,75,100,125,150,175,200,225,250};

void initDisplay() {
  //Clear shift registers
  SRSendZeros(96);
  SRLatch();

  //Setup Timer1 with Fast PWM mode with x1 prescaler
  TCCR1A |= (1<<COM1A1) | (1<<PWM1A);
  TCCR1B |= (1<<CS10);
  OCR1A = pgm_read_byte(&(brightnessLevels[brightness]));
}

void handleShiftRegister1() {
  switch (tube6) {
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
    default:
      SRSendZeros(tube6 - 4);
      SRSendOnes(1);
      SRSendZeros(11 - tube6);
      break;
  }

  switch (tube7) {
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
    default:
      SRSendZeros(tube7);
      SRSendOnes(1);
      SRSendZeros(11 - tube7);
      break;
  }

  switch (tube8) {
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
    default:
      SRSendZeros(tube8);
      SRSendOnes(1);
      SRSendZeros(11 - tube8);
      break;
  }
}

void handleShiftRegister2() {
  switch (tube3) {
    case 8:
    case 9:
      SRSendZeros(tube3 - 8);
      SRSendOnes(1);
      SRSendZeros(11 - tube3);
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
    default:
      SRSendZeros(4);
      break;
  }

  switch (tube4) {
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
    default:
      SRSendZeros(tube4);
      SRSendOnes(1);
      SRSendZeros(11 - tube4);
      break;
  }

  switch (tube5) {
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
    default:
      SRSendZeros(tube5);
      SRSendOnes(1);
      SRSendZeros(11 - tube5);
      break;
  }

  switch (tube6) {
    case 1:
    case 2:
    case 3:
      SRSendZeros(tube6);
      SRSendOnes(1);
      SRSendZeros(3 - tube6);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(3);
      break;
    default:
      SRSendZeros(4);
      break;
  }
}

void handleShiftRegister3() {
  switch (tube1) {
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
    default:
      SRSendZeros(tube1);
      SRSendOnes(1);
      SRSendZeros(11 - tube1);
      break;
  }
  switch (tube2) {
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
    default:
      SRSendZeros(tube2);
      SRSendOnes(1);
      SRSendZeros(11 - tube2);
  }

  switch (tube3) {
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
    default:
      SRSendZeros(tube3);
      SRSendOnes(1);
      SRSendZeros(7 - tube3);
      break;
  }
}

void updateDisplay() {
  handleShiftRegister1();
  handleShiftRegister2();
  handleShiftRegister3();
  SRLatch();
}

void increaseDisplayBrightness(){
  if(brightness < 10){
    brightness++;
    OCR1A = pgm_read_byte(&(brightnessLevels[brightness]));
  }
}

void decreaseDisplayBrightness(){
  if(brightness > 0){
    brightness--;
    OCR1A = pgm_read_byte(&(brightnessLevels[brightness]));
  }
}
