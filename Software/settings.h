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
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>

#define SECONDS 0
#define MINUTES 1
#define HOURS 2
#define DAY_OF_WEEK 3
#define DATE 4
#define MONTH 5
#define YEAR 6

#define ALARM1_SECONDS 0
#define ALARM1_MINUTES 1
#define ALARM1_HOURS 2
#define ALARM1_DAY_DATE 3

#define ALARM2_MINUTES 4
#define ALARM2_HOURS 5
#define ALARM2_DAY_DATE 6

#define BRIGHTNESS 0
#define TIME_FORMAT_12H 1
#define DATE_FORMAT_DD_MM 2
#define BEEP_ON_PRESS 3

#define REST_ON_HOUR 4
#define REST_ON_MINUTE 5
#define WAKE_ON_HOUR 6
#define WAKE_ON_MINUTE 7

#define A1IE 0
#define A2IE 1
#define INTCN 2
#define RS1 3
#define RS2 4
#define CONV 5
#define BBSQW 6
#define EOSC 7

typedef struct {
  uint8_t time[7];
  uint8_t alarm[7];
  uint8_t main[8];
  uint8_t not_first_boot;
  uint8_t control;
  uint8_t controlStatus;
} Settings;

extern Settings settings;

void settings_init();

void settings_readDS3232();

void settings_readTimeDS3232();

void settings_readAlarm1DS3232();

void settings_readAlarm2DS3232();

void settings_readSettingsDS3232();

void settings_readControlDS3232();

void settings_readControlStatusDS3232();

void settings_writeDS3232();

void settings_writeTimeDS3232();

void settings_writeAlarm1DS3232();

void settings_writeAlarm2DS3232();

void settings_writeSettingsDS3232();

void settings_writeControlDS3232();

void settings_writeControlStatusDS3232();

void settings_clearAlarmFlagsDS3232();

#endif /* SETTINGS_H_ */
