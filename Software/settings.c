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
uint8_t EEMEM adaptive_brightness_on = 0;
uint8_t EEMEM rest_on_hour = 0;
uint8_t EEMEM rest_on_minute = 0;
uint8_t EEMEM wake_on_hour = 0;
uint8_t EEMEM wake_on_minute = 0;
uint8_t EEMEM time_format_24h = 1;
uint8_t EEMEM date_format_dd_mm = 0;

Settings settings;

void settings_init(){
  settings_readDS3232();
  settings.defaults[DEFAULT_BRIGHTNESS] = eeprom_read_byte(&default_brightness);
  settings.defaults[ADAPTIVE_BRIGHTNESS_ON] = eeprom_read_byte(&adaptive_brightness_on);
  settings.defaults[TIME_FORMAT_24H] = eeprom_read_byte(&time_format_24h);
  settings.defaults[DATE_FORMAT_DD_MM] = eeprom_read_byte(&date_format_dd_mm);
}

void settings_readDS3232() {
  settings_readTimeDS3232();
  settings_readAlarm1DS3232();
  settings_readAlarm2DS3232();
}

void settings_readTimeDS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x00);
  i2c_rep_start(DS3232 + I2C_READ);
  settings.time[SECONDS] = i2c_readAck();
  settings.time[MINUTES] = i2c_readAck();
  settings.time[HOURS] = i2c_readAck();
  settings.time[DAY_OF_WEEK] = i2c_readAck();
  settings.time[DATE] = i2c_readAck();
  settings.time[MONTH] = i2c_readAck();
  settings.time[YEAR] = i2c_readNak();
  i2c_stop();
}

void settings_readAlarm1DS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x07);
  i2c_rep_start(DS3232 + I2C_READ);
  settings.alarm[ALARM1_SECONDS] = i2c_readAck();
  settings.alarm[ALARM1_MINUTES] = i2c_readAck();
  settings.alarm[ALARM1_HOURS] = i2c_readAck();
  settings.alarm[ALARM1_DAY_DATE] = i2c_readNak();
  i2c_stop();
}

void settings_readAlarm2DS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x0B);
  i2c_rep_start(DS3232 + I2C_READ);
  settings.alarm[ALARM2_MINUTES] = i2c_readAck();
  settings.alarm[ALARM2_HOURS] = i2c_readAck();
  settings.alarm[ALARM2_DAY_DATE] = i2c_readNak();
  i2c_stop();
}

void settings_writeDS3232(){
  settings_writeTimeDS3232();
  settings_writeAlarm1DS3232();
  settings_writeAlarm2DS3232();
}

void settings_writeTimeDS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x00);

  i2c_write(settings.time[SECONDS]); //00h
  i2c_write(settings.time[MINUTES]); //01h
  i2c_write(settings.time[HOURS]); //02h
  i2c_write(settings.time[DAY_OF_WEEK]); //03h
  i2c_write(settings.time[DATE]); //04h
  i2c_write(settings.time[MONTH]); //05h
  i2c_write(settings.time[YEAR]); //06h
  i2c_stop();
}

void settings_writeAlarm1DS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x07);

  i2c_write(settings.alarm[ALARM1_SECONDS]); //07h
  i2c_write(settings.alarm[ALARM1_MINUTES]); //08h
  i2c_write(settings.alarm[ALARM1_HOURS]); //09h
  i2c_write(settings.alarm[ALARM1_DAY_DATE]); //0Ah
  i2c_stop();
}

void settings_writeAlarm2DS3232(){
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x0B);

  i2c_write(settings.alarm[ALARM2_MINUTES]); //0Bh
  i2c_write(settings.alarm[ALARM2_HOURS]); //0Ch
  i2c_write(settings.alarm[ALARM2_DAY_DATE]); //0Dh
  i2c_stop();
}
