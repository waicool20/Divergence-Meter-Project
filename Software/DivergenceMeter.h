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
#ifndef DIVERGENCEMETER_H_
#define DIVERGENCEMETER_H_

#include <stdint.h>
#include <stdbool.h>

extern bool shouldRoll;

extern volatile uint8_t current_mode;
extern volatile bool just_entered_mode[6];

extern volatile uint16_t button_count[5];
extern volatile bool button_is_pressed[5];
extern volatile bool button_short_pressed[5];
extern volatile bool button_long_pressed[5];

void DivergenceMeter_rollWorldLine(bool rollTube2);

void DivergenceMeter_rollWorldLineWithDelay(bool rollTube2);

void DivergenceMeter_showBrightness();

#endif /* DIVERGENCEMETER_H_ */
