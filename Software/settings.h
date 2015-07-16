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

#define TIME_FORMAT_24H 0
#define DATE_FORMAT_DD_MM 1

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t day;
  uint8_t date;
  uint8_t month;
  uint8_t year;

  uint8_t alarm1_seconds;
  uint8_t alarm1_minutes;
  uint8_t alarm1_hours;
  uint8_t alarm1_day_date;

  uint8_t alarm2_minutes;
  uint8_t alarm2_hours;
  uint8_t alarm2_day;
  uint8_t alarm2_day_date;

  uint8_t default_brightness;
	uint8_t adaptive_brightness;
	uint8_t formats;
} Settings;

extern Settings settings;

void settings_init();

void settings_readDS3232();

void settings_writeTimeDS3232();

void settings_writeAlarm1DS3232();

void settings_writeAlarm2DS3232();

#endif /* SETTINGS_H_ */
