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

#include "divergenceMode.h"

#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"

/* Prototypes */

/* World line constants */

const uint8_t PROGMEM WORLD_LINES[32][7] = {
    { 0, 0, 0, 0, 0, 0, 0 }, { 0, 3, 2, 8, 4, 0, 3 },
    { 0, 3, 3, 4, 5, 8, 1 }, { 0, 3, 3, 7, 1, 8, 7 },
    { 0, 3, 3, 7, 1, 9, 9 }, { 0, 3, 3, 7, 3, 3, 7 },
    { 0, 4, 0, 9, 4, 2, 0 }, { 0, 4, 0, 9, 4, 3, 1 },
    { 0, 4, 5, 6, 9, 0, 3 }, { 0, 4, 5, 6, 9, 1, 4 },
    { 0, 4, 5, 6, 9, 2, 3 }, { 0, 5, 2, 3, 2, 9, 9 },
    { 0, 5, 2, 3, 3, 0, 7 }, { 0, 5, 7, 1, 0, 1, 5 },
    { 0, 5, 7, 1, 0, 2, 4 }, { 0, 5, 7, 1, 0, 4, 6 },
    { 1, 0, 4, 8, 5, 9, 6 }, { 1, 0, 4, 8, 5, 9, 9 },
    { 1, 0, 4, 9, 3, 2, 6 }, { 1, 1, 3, 0, 4, 2, 6 },
    { 2, 6, 1, 5, 0, 7, 4 }, { 3, 0, 1, 9, 4, 3, 0 },
    { 3, 0, 3, 0, 4, 9, 3 }, { 3, 1, 3, 0, 2, 3, 8 },
    { 3, 1, 8, 2, 8, 7, 9 }, { 3, 3, 7, 2, 3, 2, 9 },
    { 3, 3, 8, 6, 0, 1, 9 }, { 3, 4, 0, 6, 2, 8, 8 },
    { 3, 4, 0, 6, 2, 8, 8 }, { 3, 6, 0, 0, 1, 0, 4 },
    { 3, 6, 6, 7, 2, 9, 3 }, { BLANK, 2, 7, 5, 3, 4, 9 }
};

/* Variables */

uint8_t currentWorldLineIndex;

/* Divergence Mode Code */

void divergenceMode_run() {
  if (justEnteredMode[DIVERGENCE_MODE]) {
    DivergenceMeter_rollRandomWorldLine(false);
    justEnteredMode[DIVERGENCE_MODE] = false;
  }
  if (buttonLongPressed[BUTTON2] && buttonLongPressed[BUTTON3]) {
    DivergenceMeter_switchMode(DIVERGENCE_EDIT_MODE, false);
  } else if (buttonIsPressed[BUTTON2]) {
    currentWorldLineIndex =
        currentWorldLineIndex > 0 ? currentWorldLineIndex - 1 : 31;
    uint8_t result[8] = { pgm_read_byte(
        &(WORLD_LINES[currentWorldLineIndex][TUBE1])),
    BLANK, pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE3-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE4-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE5-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE6-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE7-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE8-1])), };

    DivergenceMeter_rollWorldLine(false, result);
    DivergenceMeter_delayCS(s2cs(0.2));
  } else if (buttonIsPressed[BUTTON3]) {
    currentWorldLineIndex =
        currentWorldLineIndex < 31 ? currentWorldLineIndex + 1 : 0;
    uint8_t result[8] = { pgm_read_byte(
        &(WORLD_LINES[currentWorldLineIndex][TUBE1])),
    BLANK, pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE3-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE4-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE5-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE6-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE7-1])),
        pgm_read_byte(&(WORLD_LINES[currentWorldLineIndex][TUBE8-1])), };
    DivergenceMeter_rollWorldLine(false, result);
    DivergenceMeter_delayCS(s2cs(0.2));
  } else if (buttonIsPressed[BUTTON4]) {
    DivergenceMeter_rollRandomWorldLine(false);
  } else if (buttonIsPressed[BUTTON5]) {
    display_saveState();
    display_toggleBrightness();
    DivergenceMeter_showBrightness();
    display_restoreState();
    display_update();
    return;
  }
}

