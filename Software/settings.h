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

typedef struct {
  char seconds;
  char minutes;
  char hours;
  char day;
  char date;
  char month;
  char year;

  char alarm1_seconds;
  char alarm1_minutes;
  char alarm1_hours;
  char alarm1_day;
  char alarm1_date;

  char alarm2_minutes;
  char alarm2_hours;
  char alarm2_day;
  char alarm2_date;
	
	char brightness;
	char date_format;
} Settings;

extern Settings settings;

void settings_init();

void settings_readDS3232();

void settings_writeDS3232();

#endif /* SETTINGS_H_ */
