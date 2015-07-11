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

#include <avr/io.h>
#include <avr/fuse.h>
#include <stdbool.h>
#include <util/delay.h>
#include "Constants.h"
#include "Display.h"

FUSES = { .low = 0x02, .high = 0xDF, .extended = 0x01, };

void initialize() {
  DDRB = (1 << LE) | (1 << BL) | (1 << CLK) | (1 << DIN);
  MCUCR = (1 << PUD);
  PORTB = 0x00;
  initDisplay();
}

int main() {
  initialize();
  while (1) {
    for (int i = 0; i < 12; i++) {
      tube1 = i;
      tube2 = i;
      tube3 = i;
      tube4 = i;
      tube5 = i;
      tube6 = i;
      tube7 = i;
      tube8 = i;
      updateDisplay();

      _delay_ms(10);
      _delay_ms(10);
      _delay_ms(10);
      _delay_ms(10);
      _delay_ms(10);
    }
  }
  return 0;
}
