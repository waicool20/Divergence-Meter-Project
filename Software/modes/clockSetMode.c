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

#include "clockSetMode.h"

#include <stdint.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"
#include "../util/BCD.h"

/* Prototypes */

/* Variables */

static uint8_t currentTimeSetting;

/* Clock Set Mode */

void clockSetMode_run() {
  if (justEnteredMode[CLOCK_SET_MODE]) {
    currentTimeSetting = YEAR;
    display_setTube(TUBE1, 0, false, false);
    display_setTube(TUBE3, BLANK, false, false);
    display_setTube(TUBE4, BLANK, false, false);
    display_setTube(TUBE5, BLANK, false, false);
    display_setTube(TUBE6, BLANK, false, false);
    justEnteredMode[CLOCK_SET_MODE] = false;
  }
  if (buttonIsPressed[BUTTON2]) {
    uint8_t lowerLimit = 0x00;
    switch (currentTimeSetting) {
      case MONTH:
      case DATE:
      case DAY_OF_WEEK:
        lowerLimit = 0x01;
        break;
    }
    if (settings.time[currentTimeSetting] > lowerLimit) {
      BCD_dec(&settings.time[currentTimeSetting]);
    }
    DivergenceMeter_delayCS(s2cs(0.1));
  } else if (buttonIsPressed[BUTTON3]) {
    uint8_t upperLimit = 0x59;
    switch (currentTimeSetting) {
      case YEAR:
        upperLimit = 0x99;
        break;
      case MONTH:
        upperLimit = 0x12;
        break;
      case DATE:
        upperLimit = 0x31;
        break;
      case DAY_OF_WEEK:
        upperLimit = 0x07;
        break;
      case HOURS:
        upperLimit = 0x23;
        break;
    }
    if (settings.time[currentTimeSetting] < upperLimit) {
      BCD_inc(&settings.time[currentTimeSetting]);
    }
    DivergenceMeter_delayCS(s2cs(0.1));
  } else if (buttonIsPressed[BUTTON4]) {
    if (currentTimeSetting > SECONDS) {
      currentTimeSetting--;
    } else if (currentTimeSetting == SECONDS) {
      settings_writeTimeDS3232();
      DivergenceMeter_switchMode(CLOCK_MODE, false);
    }
    DivergenceMeter_delayCS(s2cs(0.2));
  } else if (buttonIsPressed[BUTTON5]) {
    if (currentTimeSetting < YEAR) {
      currentTimeSetting++;
    }
    DivergenceMeter_delayCS(s2cs(0.2));
  }

  display_setTube(TUBE2, (7 - currentTimeSetting), false, false);
  uint8_t tube7;
  uint8_t tube8;
  switch (currentTimeSetting) {
    case MONTH:
      tube7 = ((settings.time[MONTH] >> 4) & 0x01);
      tube8 = (settings.time[MONTH] & 0x0F);
      break;
    case DAY_OF_WEEK:
      tube7 = 0;
      tube8 = (settings.time[DAY_OF_WEEK]);
      break;
    case HOURS:
      tube7 = ((settings.time[HOURS] >> 4) & 0x03);
      tube8 = (settings.time[HOURS] & 0x0F);
      break;
    default:
      tube7 = (settings.time[currentTimeSetting] >> 4);
      tube8 = (settings.time[currentTimeSetting] & 0x0F);
      break;
  }
  display_setTube(TUBE7, tube7, false, false);
  display_setTube(TUBE8, tube8, false, false);
  display_update();
}
