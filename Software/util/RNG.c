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
 * Random Number Generator code
 */

#include "RNG.h"

#include <stdint.h>
#include <avr/io.h>

#include "../settings.h"

void RNG_next();

uint8_t randa;
uint8_t randb;
uint8_t randc;
uint8_t randx;

void RNG_seed() {
  randa = ADCH;  //Seed with value from LDR
  randb = settings.time[SECONDS];  //Seed with current second
  randc = settings.time[MINUTES];  //Seed with current minute
  randx = settings.time[HOURS];
}

void RNG_next() {
  randx++;
  randa = (randa ^ randc ^ randx);
  randb = (randb + randa);
  randc = (randc + ((randb >> 1) ^ randa));
}

uint8_t RNG_nextByte(){
  return randc;
}

uint8_t RNG_nextChar() {
  RNG_next();
  uint8_t temp = randc & 0x0F;
  return temp > 9 ? 15 - temp : temp;
}
