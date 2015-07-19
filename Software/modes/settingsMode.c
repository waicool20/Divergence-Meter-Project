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

#include "settingsMode.h"

#include <stdint.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"
#include "../util/BCD.h"

/* Prototypes */

void DivergenceMeter_updateSettingsDisplay();

/* Variables */
uint8_t currentSetting = 0;

/* Settings Mode Code */

void settingsMode_run() {
  if (justEnteredMode[SETTINGS_MODE]) {
    currentSetting = TIME_FORMAT_12H;
    display_setTube(TUBE1, (0), false, false);
    display_setTube(TUBE3, (BLANK), false, false);
    display_setTube(TUBE4, (BLANK), false, false);
    display_setTube(TUBE5, (BLANK), false, false);
    display_setTube(TUBE6, (BLANK), false, false);
    justEnteredMode[SETTINGS_MODE] = false;
  }


  if (buttonIsPressed[BUTTON2]) {
    if (settings.main[currentSetting] > 0x00){
      BCD_dec(&settings.main[currentSetting]);
    }
    DivergenceMeter_delayCS(s2cs(0.1));
  } else if (buttonIsPressed[BUTTON3]){
    uint8_t upperLimit = 0x01;
    switch (currentSetting) {
      case REST_ON_HOUR:
      case WAKE_ON_HOUR:
        upperLimit = 0x23;
        break;
      case REST_ON_MINUTE:
      case WAKE_ON_MINUTE:
        upperLimit = 0x59;
        break;
    }
    if (settings.main[currentSetting] < upperLimit){
      BCD_inc(&settings.main[currentSetting]);
    }
    DivergenceMeter_delayCS(s2cs(0.1));
  } else if (buttonShortPressed[BUTTON4]) {
    if (currentSetting < WAKE_ON_MINUTE) {
      currentSetting++;
    } else {
      settings_writeSettingsDS3232();
      DivergenceMeter_switchMode(CLOCK_MODE, false);
    }
    DivergenceMeter_delayCS(s2cs(0.1));
  } else if (buttonShortPressed[BUTTON5]) {
    if (currentSetting > TIME_FORMAT_12H) {
      currentSetting--;
      DivergenceMeter_delayCS(s2cs(0.1));
    }
  }
  display_setTube(TUBE2, (currentSetting), false, false);
  display_setTube(TUBE7, (settings.main[currentSetting] >> 4), false, false);
  display_setTube(TUBE8, (settings.main[currentSetting] & 0x0F), false, false);
  display_update();
}
