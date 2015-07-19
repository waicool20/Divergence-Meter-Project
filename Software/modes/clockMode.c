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

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"
#include "../util/BCD.h"

/* Prototypes */

void clockMode_displayCurrentTime();
void clockMode_displayDates();
void clockMode_displayCurrentDate();
void clockMode_displayCurrentDayOfWeek();

/* Clock Mode Code */

void clockMode_run() {
  if (justEnteredMode[CLOCK_MODE]) {
    justEnteredMode[CLOCK_MODE] = false;
  }
  switch (settings.time[SECONDS]) {
    case 0x00:
      shouldRoll = true;
      DivergenceMeter_rollWorldLine(false);
      shouldRoll = false;
      clockMode_displayDates();
      break;
  }
  if (buttonShortPressed[BUTTON2]) {
    clockMode_displayDates();
  } else if (buttonLongPressed[BUTTON2]){
    DivergenceMeter_switchMode(CLOCK_SET_MODE, false);
  } else if (buttonShortPressed[BUTTON3]) {


  } else if (buttonIsPressed[BUTTON4]) {
    shouldRoll = true;
    DivergenceMeter_rollWorldLineWithDelay(true);
  } else if (buttonIsPressed[BUTTON5]) {
    display_toggleBrightness();
    DivergenceMeter_showBrightness();
    return;
  }
  clockMode_displayCurrentTime();
}

void clockMode_displayCurrentTime() {
  if(settings.main[TIME_FORMAT_12H]){
    uint8_t hour = settings.time[HOURS];
    if(hour >= 0x13){
      hour = BCD_sub(hour,0x12); //PM
    } else if(hour >= 0x12) {
      //PM
    } else if(hour >= 0x1) {
      //AM
    } else {
      hour = BCD_add(hour,0x12); //AM
    }
    display_setTube(TUBE1, ((hour >> 4) & 0x03),false,false);
    display_setTube(TUBE2, (hour & 0x0F),false,false);
  } else {
    display_setTube(TUBE1, ((settings.time[HOURS] >> 4) & 0x03), false, false);
    display_setTube(TUBE2, (settings.time[HOURS] & 0x0F), false, false);
  }
  display_setTube(TUBE6, BLANK, !(settings.time[SECONDS] & 0x01), (settings.time[SECONDS] & 0x01));
  display_setTube(TUBE4, (settings.time[MINUTES] >> 4), false, false);
  display_setTube(TUBE5, (settings.time[MINUTES] & 0x0F), false, false);
  display_setTube(TUBE6, BLANK, (settings.time[SECONDS] & 0x01), !(settings.time[SECONDS] & 0x01));
  display_setTube(TUBE7, (settings.time[SECONDS] >> 4), false, false);
  display_setTube(TUBE8, (settings.time[SECONDS] & 0x0F), false, false);
  display_update();
}

void clockMode_displayDates(){
  clockMode_displayCurrentDate();
  DivergenceMeter_delayCS(s2cs(DATE_DISPLAY_S));
  clockMode_displayCurrentDayOfWeek();
  DivergenceMeter_delayCS(s2cs(DAY_DISPLAY_S));
}

void clockMode_displayCurrentDate() {
  display_setTube(TUBE1, ((settings.time[settings.main[DATE_FORMAT_DD_MM] ? DATE : MONTH] >> 4) & 0x03), false, false);
  display_setTube(TUBE2, (settings.time[settings.main[DATE_FORMAT_DD_MM] ? DATE : MONTH] & 0x0F), false, false);
  display_setTube(TUBE3, BLANK, false, true);
  display_setTube(TUBE4, ((settings.time[settings.main[DATE_FORMAT_DD_MM] ? MONTH : DATE] >> 4) & 0x01), false, false);
  display_setTube(TUBE5, (settings.time[settings.main[DATE_FORMAT_DD_MM] ? MONTH : DATE] & 0x0F), false, false);
  display_setTube(TUBE3, BLANK, true, false);
  display_setTube(TUBE7, (settings.time[YEAR] >> 4), false, false);
  display_setTube(TUBE8, (settings.time[YEAR] & 0x0F), false, false);
  display_update();
}

void clockMode_displayCurrentDayOfWeek(){
  display_setTube(TUBE1, BLANK, false, false);
  display_setTube(TUBE2, BLANK, false, false);
  display_setTube(TUBE3, BLANK, false, false);
  display_setTube(TUBE4, 0, false, false);
  display_setTube(TUBE5, (settings.time[DAY_OF_WEEK]), false, false);
  display_setTube(TUBE3, BLANK, false, false);
  display_setTube(TUBE7, BLANK, false, false);
  display_setTube(TUBE8, BLANK, false, false);
  display_update();
}
