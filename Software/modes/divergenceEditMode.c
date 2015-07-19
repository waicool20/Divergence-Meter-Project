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

#include "divergenceEditMode.h"

#include <stdint.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"

/* Variables */

uint8_t currentTube = 0;

/* Divergence Edit Mode */

void divergenceEditMode_run() {
  if (justEnteredMode[DIVERGENCE_EDIT_MODE]) {
    currentTube = TUBE1;
    display_setTube(TUBE1, 0, false, false);
    display_setTube(TUBE2, BLANK, false, false);
    display_setTube(TUBE3, BLANK, false, false);
    display_setTube(TUBE4, BLANK, false, false);
    display_setTube(TUBE5, BLANK, false, false);
    display_setTube(TUBE6, BLANK, false, false);
    display_setTube(TUBE7, BLANK, false, false);
    display_setTube(TUBE8, BLANK, false, false);
    display_update();
    justEnteredMode[DIVERGENCE_EDIT_MODE] = false;
  }

  if (buttonIsPressed[BUTTON2]) {
    if (display.tube[currentTube] > 0) {
      display.tube[currentTube]--;
    }
    display_update();
    DivergenceMeter_delayCS(s2cs(0.2));
  } else if (buttonIsPressed[BUTTON3]) {
    if (display.tube[currentTube] < BLANK) {
      display.tube[currentTube]++;
    }
    display_update();
    DivergenceMeter_delayCS(s2cs(0.2));
  }

  if (buttonShortPressed[BUTTON4]) {
    if (currentTube < TUBE8) {
      if (currentTube == TUBE1) {
        display_setTube(TUBE2, BLANK, true, false);
        currentTube = TUBE3;  //SKIP TUBE2
      } else {
        currentTube++;
      }
      display.tube[currentTube] = 0;
      display_update();
    } else {
      currentTube = TUBE1;
      DivergenceMeter_switchMode(DIVERGENCE_MODE, true);
      display_saveState();
      DivergenceMeter_rollWorldLine(false);
      display_restoreState();
      display_update();
    }
    DivergenceMeter_delayCS(s2cs(0.2));
  } else if (buttonShortPressed[BUTTON5]) {
    if (currentTube > TUBE1) {
      display.tube[currentTube] = BLANK;
      if (currentTube == TUBE3) {
        display_setTube(TUBE2, BLANK, false, false);
        currentTube = TUBE1;
      } else {
        currentTube--;
      }
      display_update();
    }
    DivergenceMeter_delayCS(s2cs(0.2));
  }
}
