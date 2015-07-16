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

#include <stdint.h>
#include <avr/eeprom.h>

#include "constants.h"
#include "i2cmaster.h"
#include "util/display.h"

uint8_t EEMEM default_brightness = 5;
uint8_t EEMEM adaptive_brightness = 0;
uint8_t EEMEM formats = (1<<TIME_FORMAT_24H) | (1<<DATE_FORMAT_DD_MM);

Settings settings;

void settings_init(){
  settings_readDS3232();
  settings.default_brightness = eeprom_read_byte(&default_brightness);
  settings.adaptive_brightness = eeprom_read_byte(&adaptive_brightness);
  settings.formats = eeprom_read_byte(&formats);
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
  settings.alarm1_day_date = i2c_readAck();

  settings.alarm2_minutes = i2c_readAck();
  settings.alarm2_hours = i2c_readAck();
  settings.alarm2_day_date = i2c_readAck();
  i2c_stop();
}

void settings_writeTimeDS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x00);

  i2c_write(settings.seconds); //00h
  i2c_write(settings.minutes); //01h
  i2c_write(settings.hours | ((settings.formats & 0x01)<<6)); //02h
  i2c_write(settings.day); //03h
  i2c_write(settings.date); //04h
  i2c_write(settings.month); //05h
  i2c_write(settings.year); //06h
  i2c_stop();
}

void settings_writeAlarm1DS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x07);

  i2c_write(settings.alarm1_seconds); //07h
  i2c_write(settings.alarm1_minutes); //08h
  i2c_write(settings.alarm1_hours); //09h
  i2c_write(settings.alarm1_day_date); //Ah
  i2c_stop();
}

void settings_writeAlarm2DS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x0B);

  i2c_write(settings.alarm2_minutes); //Bh
  i2c_write(settings.alarm2_hours); //Ch
  i2c_write(settings.alarm2_day_date); //Dh
  i2c_stop();
}
