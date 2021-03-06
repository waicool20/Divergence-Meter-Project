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

#include "BCD.h"

#include <stdint.h>
#include <stdbool.h>

void BCD_inc(uint8_t *bcd) {
  uint8_t HBCD = ((*bcd) >> 4) & 0x0F;
  uint8_t LBCD = (*bcd) & 0x0F;
  uint8_t LBCD_inc = LBCD + 1;
  if (LBCD_inc >= 10) {
    HBCD++;
    LBCD = 0;
  } else {
    LBCD = LBCD_inc;
  }
  *bcd = (HBCD << 4) | LBCD;
}

void BCD_dec(uint8_t *bcd) {
  uint8_t HBCD = ((*bcd) >> 4) & 0x0F;
  uint8_t LBCD = (*bcd) & 0x0F;
  uint8_t LBCD_dec = LBCD - 1;
  if (LBCD_dec < 9) {
    LBCD = LBCD_dec;
  } else {
    if (HBCD > 0) {
      HBCD--;
      LBCD = 9;
    }
  }
  *bcd = (HBCD << 4) | LBCD;
}

uint8_t BCD_add(uint8_t bcd, uint8_t bcd2) {
  uint16_t bcd_add = bcd + bcd2;
  uint8_t carry = bcd_add >> 8;

  uint8_t HBCD = (bcd_add >> 4) & 0x0F;
  uint8_t LBCD = bcd_add & 0x0F;
  if (carry || LBCD > 9 || HBCD > 9) {
    bcd_add += 6;
  }
  return bcd_add & 0xFF;
}

uint8_t BCD_sub(uint8_t bcd, uint8_t bcd2) {
  uint16_t bcd_sub = bcd - bcd2;
  uint8_t carry = bcd_sub >> 8;

  uint8_t HBCD = (bcd_sub >> 4) & 0x0F;
  uint8_t LBCD = bcd_sub & 0x0F;
  if (carry || LBCD > 9 || HBCD > 9) {
    bcd_sub -= 6;
  }
  return bcd_sub & 0xFF;
}

