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
 * This is the code that handles the data shifting into the HV5622 Shift Registers.
 */

#include <avr/io.h>

#include "../constants.h"

void SRShift(char i) {
  while (i) {
    PORTB |= (1 << CLK);
    PORTB &= ~(1 << CLK);
    i--;
  }
}

void SRLatch() {
  PORTB |= (1 << LE);
  PORTB &= ~(1 << LE);
}

void SRON() {
  PORTB |= (1 << BL);
}

void SROFF() {
  PORTB &= ~(1 << BL);
}

void SRSendOnes(char i) {
  PORTB |= (1 << DIN);
  SRShift(i);
  PORTB &= ~(1 << DIN);
}

void SRSendZeros(char i) {
  PORTB &= ~(1 << DIN);
  SRShift(i);
}
