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
#include <util/delay.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"

/* Prototypes */

void DivergenceMeter_updateSettingsDisplay();

/* Variables */
uint8_t currentSetting = 0;

/* Settings Mode Code */

void settingsMode_run() {
  if (just_entered_mode[SETTINGS_MODE]) {
    currentSetting = TIME_FORMAT_24H;
    display.tube[TUBE1] = 0;
    display.tube[TUBE2] = currentSetting;
    display.tube[TUBE3] = BLANK;
    display.tube[TUBE4] = BLANK;
    display.tube[TUBE5] = BLANK;
    display.tube[TUBE6] = BLANK;
    display.tube[TUBE7] = 0;
    display.tube[TUBE8] = settings.main[currentSetting];
    display_update();
    just_entered_mode[SETTINGS_MODE] = false;
  }
  if (button_is_pressed[BUTTON2] || button_is_pressed[BUTTON3]) {
    display.tube[TUBE1] = 0;
    display.tube[TUBE2] = currentSetting;
    switch (currentSetting) {
      case TIME_FORMAT_24H:
      case DATE_FORMAT_DD_MM:
        settings.main[currentSetting] = settings.main[currentSetting] ? 0 : 1;
        break;
      case REST_ON_HOUR:
      case WAKE_ON_HOUR:
        if (button_is_pressed[BUTTON3]) {
          if (settings.main[currentSetting] < 24) {
            settings.main[currentSetting]++;
          }
        } else if (button_is_pressed[BUTTON2]) {
          if (settings.main[currentSetting] > 0) {
            settings.main[currentSetting]--;
          }
        }
        break;
      case REST_ON_MINUTE:
      case WAKE_ON_MINUTE:
        if (button_is_pressed[BUTTON3]) {
          if (settings.main[currentSetting] < 60) {
            settings.main[currentSetting]++;
          }
        } else if (button_is_pressed[BUTTON2]) {
          if (settings.main[currentSetting] > 0) {
            settings.main[currentSetting]--;
          }
        }
        break;
    }
    DivergenceMeter_updateSettingsDisplay();
    _delay_ms(100);
  } else if (button_short_pressed[BUTTON4]) {
    if (currentSetting < WAKE_ON_MINUTE) {
      currentSetting++;
      DivergenceMeter_updateSettingsDisplay();
    } else {
      settings_writeSettingsDS3232();
      current_mode = CLOCK_MODE;
      just_entered_mode[CLOCK_MODE] = true;
    }
    _delay_ms(200);
  } else if (button_short_pressed[BUTTON5]) {
    if (currentSetting > TIME_FORMAT_24H) {
      currentSetting--;
      DivergenceMeter_updateSettingsDisplay();
      _delay_ms(200);
    }
  }
}

void DivergenceMeter_updateSettingsDisplay() {
  display.tube[TUBE2] = currentSetting;
  display.tube[TUBE7] = (settings.main[currentSetting] / 10) % 10;
  display.tube[TUBE8] = settings.main[currentSetting] % 10;
  display_update();
}
