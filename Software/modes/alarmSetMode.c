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

#include "alarmSetMode.h"

#include <stdint.h>
#include <stdbool.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"
#include "../util/BCD.h"

static bool settingAlarm;
static uint8_t currentAlarm;
static uint8_t currentSetting;

void alarmSetMode_run(){
  if(justEnteredMode[ALARM_SET_MODE]){
    currentAlarm = 1;
    settingAlarm = false;
    display_setTube(TUBE1, BLANK, false, false);
    display_setTube(TUBE2, BLANK, false, false);
    display_setTube(TUBE3, BLANK, false, false);
    display_setTube(TUBE4, 0, false, false);
    display_setTube(TUBE5, currentAlarm, false, false);
    display_setTube(TUBE6, BLANK, false, false);
    display_setTube(TUBE7, BLANK, false, false);
    display_setTube(TUBE8, BLANK, false, false);
    justEnteredMode[ALARM_SET_MODE] = false;
  }
  if(settingAlarm){
    uint8_t settingOffset = currentAlarm == ALARM1 ? 0 : 3;
    settings.alarm[DAY_OF_WEEK + settingOffset] &= 0x0F;
    if(buttonIsPressed[BUTTON2]){
      if(settings.alarm[currentSetting + settingOffset] > 0){
        BCD_dec(&settings.alarm[currentSetting + settingOffset]);
      }
      DivergenceMeter_delayCS(s2cs(0.1));
    } else if (buttonIsPressed[BUTTON3]){
      uint8_t upperLimit = 0x59;
      switch(currentSetting){
        case HOURS:
          upperLimit = 0x23;
          break;
        case DAY_OF_WEEK:
          upperLimit = 0x07;
          break;
      }
      if(settings.alarm[currentSetting + settingOffset] < upperLimit){
        BCD_inc(&settings.alarm[currentSetting + settingOffset]);
      }
      DivergenceMeter_delayCS(s2cs(0.1));
    } else if (buttonIsPressed[BUTTON4]){
      if((currentAlarm == ALARM2 && currentSetting == MINUTES) || currentSetting == SECONDS){
        settings.alarm[DAY_OF_WEEK + settingOffset] &= 0x0F;
        if(settings.alarm[DAY_OF_WEEK + settingOffset]){
          settings.alarm[DAY_OF_WEEK + settingOffset] |= 0x40;
        } else {
          settings.alarm[DAY_OF_WEEK + settingOffset] |= 0x80;
        }
        settings_writeAlarm1DS3232();
        settings_writeAlarm2DS3232();
        settings.control |= currentAlarm == ALARM1 ? 0x01 : 0x02;
        settings_writeControlDS3232();
        DivergenceMeter_switchMode(CLOCK_MODE, false);
      } else {
        currentSetting--;
      }
      DivergenceMeter_delayCS(s2cs(0.2));
    } else if (buttonIsPressed[BUTTON5]){
      if(currentSetting < DAY_OF_WEEK){
        currentSetting++;
      }
      DivergenceMeter_delayCS(s2cs(0.2));
    }

    uint8_t tube7;
    uint8_t tube8;;
    switch(currentSetting){
      case HOURS:
        tube7 = (settings.alarm[currentSetting + settingOffset] >> 4) & 0x03;
        tube8 = settings.alarm[currentSetting + settingOffset] & 0x0F;
        break;
      case DAY_OF_WEEK:
        tube7 = 0;
        tube8 = settings.alarm[currentSetting + settingOffset] & 0x0F;
        break;
      default:
        tube7 = (settings.alarm[currentSetting + settingOffset] >> 4) & 0x07;
        tube8 = settings.alarm[currentSetting + settingOffset] & 0x0F;
        break;
    }

    display_setTube(TUBE1, 0, false, false);
    display_setTube(TUBE2, 4 - currentSetting, false, false);
    display_setTube(TUBE7, tube7, false, false);
    display_setTube(TUBE8, tube8, false, false);
  } else {
    display_setTube(TUBE4, 0, false, false);
    display_setTube(TUBE5, currentAlarm, false, false);
    if(buttonShortPressed[BUTTON2] && currentAlarm != 1){
      currentAlarm = 1;
    } else if (buttonShortPressed[BUTTON3] && currentAlarm != 2){
      currentAlarm = 2;
    } else if (buttonShortPressed[BUTTON4]){
      settingAlarm = true;
      currentSetting = DAY_OF_WEEK;
      display_setTube(TUBE4, BLANK, false, false);
      display_setTube(TUBE5, BLANK, false, false);
      DivergenceMeter_delayCS(s2cs(0.2));
    }
  }
  display_update();
}
