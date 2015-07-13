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

/*
 * Code for displaying and synchronizing time from the DS3232 chip
 */

#include "settings.h"

#include <avr/eeprom.h>

#include "constants.h"
#include "i2cmaster.h"
#include "util/display.h"

#define DEFAULT_BRIGHTNESS 9
unsigned char brightness EEMEM = DEFAULT_BRIGHTNESS;
unsigned char date_format EEMEM = 0;

Settings settings;

void settings_init(){
  settings_readDS3232();
  settings.brightness = eeprom_read_byte(&brightness);
  settings.date_format = eeprom_read_byte(&date_format);
}

void settings_readDS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x00);
  i2c_rep_start(DS3232 + I2C_READ);
  settings.seconds = i2c_readAck();
  settings.minutes = i2c_readAck();
  settings.hours = i2c_readAck();
  settings.day = i2c_readAck();
  settings.date = i2c_readAck();
  settings.month = i2c_readAck();
  settings.year = i2c_readAck();

  settings.alarm1_seconds = i2c_readAck();
  settings.alarm1_minutes = i2c_readAck();
  settings.alarm1_hours = i2c_readAck();
  settings.alarm1_date = i2c_readAck();

  settings.alarm2_minutes = i2c_readAck();
  settings.alarm2_hours = i2c_readAck();
  settings.alarm2_date = i2c_readAck();
  i2c_stop();
}

void settings_writeDS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x00);

  i2c_write(settings.seconds);
  i2c_write(settings.minutes);
  i2c_write(settings.hours);
  i2c_write(settings.day);
  i2c_write(settings.date);
  i2c_write(settings.month);
  i2c_write(settings.year);

  i2c_write(settings.alarm1_seconds);
  i2c_write(settings.alarm1_minutes);
  i2c_write(settings.alarm1_hours);
  i2c_write(settings.alarm1_day);
  i2c_write(settings.alarm1_date);

  i2c_write(settings.alarm2_minutes);
  i2c_write(settings.alarm2_hours);
  i2c_write(settings.alarm2_day);
  i2c_write(settings.alarm2_date);
  i2c_stop();
}
