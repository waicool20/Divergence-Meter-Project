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

extern volatile bool justEnteredMode[7];

extern volatile uint16_t buttonCount[5];
extern volatile bool buttonIsPressed[5];
extern volatile bool buttonShortPressed[5];
extern volatile bool buttonLongPressed[5];

extern volatile uint16_t ringDuration;

void DivergenceMeter_rollWorldLine(bool rollTube2, uint8_t worldline[8]);

void DivergenceMeter_rollRandomWorldLine(bool rollTube2);

void DivergenceMeter_rollRandomWorldLineWithDelay(bool rollTube2);

void DivergenceMeter_showBrightness();

void DivergenceMeter_delayCS(uint16_t delay_cs);

void DivergenceMeter_sleep();

void DivergenceMeter_switchMode(uint8_t mode, bool silent);

void DivergenceMeter_buzz(uint8_t duration_cs, uint8_t interval, uint8_t times);

#endif /* DIVERGENCEMETER_H_ */
