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

typedef struct {
  char tube1, tube2, tube3, tube4, tube5, tube6, tube7, tube8;
} Display;

extern Display display;

void display_init();

void display_update();

void display_on();

void display_off();

void display_increaseBrightness();

void display_decreaseBrightness();

#endif /* DISPLAY_H_ */
