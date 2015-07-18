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

void clockSetMode_run(){
  if (justEnteredMode[CLOCK_SET_MODE]) {
    currentTimeSetting = YEAR;
    display.tube[TUBE1] = 0;
    display.tube[TUBE3] = BLANK;
    display.tube[TUBE4] = BLANK;
    display.tube[TUBE5] = BLANK;
    display.tube[TUBE6] = BLANK;
    justEnteredMode[CLOCK_SET_MODE] = false;
  }
  if(buttonIsPressed[BUTTON2]){
    uint8_t lowerLimit = 0x00;
    switch(currentTimeSetting){
        case MONTH:
        case DATE:
        case DAY_OF_WEEK:
          lowerLimit = 0x01;
          break;
      }
    if (settings.time[currentTimeSetting] > lowerLimit){
      BCD_dec(&settings.time[currentTimeSetting]);
    }
    DivergenceMeter_delayCS(s2cs(0.1));
  } else if (buttonIsPressed[BUTTON3]){
    uint8_t upperLimit = 0x60;
    switch(currentTimeSetting){
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
    if (settings.time[currentTimeSetting] < upperLimit){
      BCD_inc(&settings.time[currentTimeSetting]);
    }
    DivergenceMeter_delayCS(s2cs(0.1));
  } else if (buttonIsPressed[BUTTON4]){
    if(currentTimeSetting > SECONDS){
      currentTimeSetting--;
    } else if (currentTimeSetting == SECONDS){
      settings_writeTimeDS3232();
      DivergenceMeter_switchMode(CLOCK_MODE);
    }
    DivergenceMeter_delayCS(s2cs(0.2));
  } else if (buttonIsPressed[BUTTON5]){
    if(currentTimeSetting < YEAR){
      currentTimeSetting++;
    }
    DivergenceMeter_delayCS(s2cs(0.2));
  }


  display.tube[TUBE2] = 7 - currentTimeSetting;
  switch(currentTimeSetting){
    case YEAR:
      display.tube[TUBE7] = settings.time[YEAR] >> 4;
      display.tube[TUBE8] = settings.time[YEAR] & 0x0F;
      break;
    case MONTH:
      display.tube[TUBE7] = (settings.time[MONTH] >> 4) & 0x01;
      display.tube[TUBE8] = settings.time[MONTH] & 0x0F;
      break;
    case DATE:
      display.tube[TUBE7] = settings.time[DATE] >> 4;
      display.tube[TUBE8] = settings.time[DATE] & 0x0F;
      break;
    case DAY_OF_WEEK:
      display.tube[TUBE7] = 0;
      display.tube[TUBE8] = settings.time[DAY_OF_WEEK];
      break;
    case HOURS:
      display.tube[TUBE7] = (settings.time[HOURS] >> 4) & 0x03;
      display.tube[TUBE8] = settings.time[HOURS] & 0x0F;
      break;
    case MINUTES:
      display.tube[TUBE7] = settings.time[MINUTES] >> 4;
      display.tube[TUBE8] = settings.time[MINUTES] & 0x0F;
      break;
    case SECONDS:
      display.tube[TUBE7] = settings.time[SECONDS] >> 4;
      display.tube[TUBE8] = settings.time[SECONDS] & 0x0F;
      break;
  }
  display_update();
}
