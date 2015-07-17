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
#define ALARM2_DAY 6
#define ALARM2_DAY_DATE 7

#define BRIGHTNESS 0
#define TIME_FORMAT_12H 1
#define DATE_FORMAT_DD_MM 2

#define REST_ON_HOUR 3
#define REST_ON_MINUTE 4
#define WAKE_ON_HOUR 5
#define WAKE_ON_MINUTE 6

typedef struct {
  uint8_t time[7];
  uint8_t alarm[8];
  uint8_t main[7];
  uint8_t not_first_boot;
} Settings;

extern Settings settings;

void settings_init();

void settings_readDS3232();

void settings_readTimeDS3232();

void settings_readAlarm1DS3232();

void settings_readAlarm2DS3232();

void settings_readSettingsDS3232();

void settings_writeDS3232();

void settings_writeTimeDS3232();

void settings_writeAlarm1DS3232();

void settings_writeAlarm2DS3232();

void settings_writeSettingsDS3232();

#endif /* SETTINGS_H_ */
