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

#include "restMode.h"

#include <stdint.h>

#include "../DivergenceMeter.h"
#include "../settings.h"
#include "../constants.h"
#include "../util/display.h"

/* Prototypes */

static void restMode_exit();

/* Rest Mode */

void restMode_run(){
  if(justEnteredMode[REST_MODE]){
    display_off();
    justEnteredMode[REST_MODE] = false;
  }
  if((settings.time[HOURS] == settings.main[WAKE_ON_HOUR] && settings.time[MINUTES] == settings.main[WAKE_ON_MINUTE])){
    restMode_exit();
  }
  for(int8_t i = BUTTON1; i >= BUTTON5; i--){
    if(buttonShortPressed[i]){
      restMode_exit();
    }
  }
}

static void restMode_exit(){
  DivergenceMeter_switchMode(CLOCK_MODE, false);
  display_on();
}
