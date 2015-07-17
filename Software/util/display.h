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
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

typedef struct {
  uint8_t tube[8];
} Display;

extern Display display;
extern Display last_display_state;

void display_init();

void display_update();

void display_on();

void display_off();

void display_updateAdaptiveBrightness();

void display_showCurrentBrightness();

void display_setBrightness(uint8_t b);

void display_increaseBrightness();

void display_decreaseBrightness();

void display_toggleBrightness();

void display_saveState();

void display_restoreState();

#endif /* DISPLAY_H_ */
