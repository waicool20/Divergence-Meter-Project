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

#include "clockMode.h"

#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"

/* Prototypes */

void DivergenceMeter_displayCurrentTime();
void DivergenceMeter_displayCurrentDate();

/* Clock Mode Code */

void clockMode_run() {
  if (just_entered_mode[CLOCK_MODE]) {
    just_entered_mode[CLOCK_MODE] = false;
  }
  switch (settings.time[SECONDS]) {
    case 0x00:
      shouldRoll = true;
      DivergenceMeter_rollWorldLine(false);
      DivergenceMeter_displayCurrentDate();
      shouldRoll = false;
      _delay_ms((DATE_DISPLAY_SECONDS * 1000));
      break;
  }
  if (button_is_pressed[BUTTON2]) {
    DivergenceMeter_displayCurrentDate();
    _delay_ms((DATE_DISPLAY_SECONDS * 1000));
  } else if (button_short_pressed[BUTTON3]) {

  } else if (button_is_pressed[BUTTON4]) {
    shouldRoll = true;
    DivergenceMeter_rollWorldLineWithDelay(true);
  } else if (button_is_pressed[BUTTON5]) {
    display_toggleBrightness();
    DivergenceMeter_showBrightness();
    return;
  }
  DivergenceMeter_displayCurrentTime();
}

void DivergenceMeter_displayCurrentTime() {
  display.tube[TUBE1] = (settings.time[HOURS] >> 4) & 0x01;
  display.tube[TUBE2] = settings.time[HOURS] & 0x0F;
  display.tube[TUBE3] = (settings.time[SECONDS] & 0x01) ? LDP : RDP;
  display.tube[TUBE4] = (settings.time[MINUTES] >> 4);
  display.tube[TUBE5] = settings.time[MINUTES] & 0x0F;
  display.tube[TUBE6] = (settings.time[SECONDS] & 0x01) ? LDP : RDP;
  display.tube[TUBE7] = (settings.time[SECONDS] >> 4);
  display.tube[TUBE8] = settings.time[SECONDS] & 0x0F;
  display_update();
}

void DivergenceMeter_displayCurrentDate() {
  display.tube[TUBE1] = (settings.time[DATE] >> 4) & 0x03;
  display.tube[TUBE2] = settings.time[DATE] & 0x0F;
  display.tube[TUBE3] = LDP;
  display.tube[TUBE4] = (settings.time[MONTH] >> 4) & 0x01;
  display.tube[TUBE5] = settings.time[MONTH] & 0x0F;
  display.tube[TUBE3] = RDP;
  display.tube[TUBE7] = (settings.time[YEAR] >> 4);
  display.tube[TUBE8] = settings.time[YEAR] & 0x0F;
  display_update();
}
