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
#include <avr/pgmspace.h>

#include "constants.h"
#include "i2cmaster.h"
#include "util/display.h"

const uint8_t PROGMEM default_settings[3] = {
    10,  // DEFAULT_BRIGHTNESS
    0,  // TIME_FORMAT_12H
    0,  // DATE_FORMAT_DD_MM
    };

Settings settings;

void settings_init() {
  i2c_init();
  settings_readDS3232();
  if (!settings.not_first_boot) {
    settings.main[BRIGHTNESS] = pgm_read_byte(&(default_settings[BRIGHTNESS]));
    settings.main[TIME_FORMAT_12H] = pgm_read_byte(
        &(default_settings[TIME_FORMAT_12H]));
    settings.main[DATE_FORMAT_DD_MM] = pgm_read_byte(
        &(default_settings[DATE_FORMAT_DD_MM]));
    settings.main[REST_ON_HOUR] = 0;
    settings.main[REST_ON_MINUTE] = 0;
    settings.main[WAKE_ON_HOUR] = 0;
    settings.main[WAKE_ON_MINUTE] = 0;
    settings.not_first_boot = 1;
    settings_writeSettingsDS3232();
  }
}

void settings_readDS3232() {
  settings_readTimeDS3232();
  settings_readAlarm1DS3232();
  settings_readAlarm2DS3232();
  settings_readSettingsDS3232();
}

void settings_readTimeDS3232() {
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

void settings_readAlarm1DS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x07);
  i2c_rep_start(DS3232 + I2C_READ);
  settings.alarm[ALARM1_SECONDS] = i2c_readAck();
  settings.alarm[ALARM1_MINUTES] = i2c_readAck();
  settings.alarm[ALARM1_HOURS] = i2c_readAck();
  settings.alarm[ALARM1_DAY_DATE] = i2c_readNak();
  i2c_stop();
}

void settings_readAlarm2DS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x0B);
  i2c_rep_start(DS3232 + I2C_READ);
  settings.alarm[ALARM2_MINUTES] = i2c_readAck();
  settings.alarm[ALARM2_HOURS] = i2c_readAck();
  settings.alarm[ALARM2_DAY_DATE] = i2c_readNak();
  i2c_stop();
}

void settings_readSettingsDS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x14);
  i2c_rep_start(DS3232 + I2C_READ);
  settings.main[BRIGHTNESS] = i2c_readAck();
  settings.main[TIME_FORMAT_12H] = i2c_readAck();
  settings.main[DATE_FORMAT_DD_MM] = i2c_readAck();

  settings.main[REST_ON_HOUR] = i2c_readAck();
  settings.main[REST_ON_MINUTE] = i2c_readAck();
  settings.main[WAKE_ON_HOUR] = i2c_readAck();
  settings.main[WAKE_ON_MINUTE] = i2c_readAck();

  settings.not_first_boot = i2c_readNak();
  i2c_stop();
}

void settings_writeDS3232() {
  settings_writeTimeDS3232();
  settings_writeAlarm1DS3232();
  settings_writeAlarm2DS3232();
  settings_writeSettingsDS3232();
}

void settings_writeTimeDS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x00);

  i2c_write(settings.time[SECONDS]);  //00h
  i2c_write(settings.time[MINUTES]);  //01h
  i2c_write(settings.time[HOURS]);  //02h
  i2c_write(settings.time[DAY_OF_WEEK]);  //03h
  i2c_write(settings.time[DATE]);  //04h
  i2c_write(settings.time[MONTH]);  //05h
  i2c_write(settings.time[YEAR]);  //06h
  i2c_stop();
}

void settings_writeAlarm1DS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x07);

  i2c_write(settings.alarm[ALARM1_SECONDS]);  //07h
  i2c_write(settings.alarm[ALARM1_MINUTES]);  //08h
  i2c_write(settings.alarm[ALARM1_HOURS]);  //09h
  i2c_write(settings.alarm[ALARM1_DAY_DATE]);  //0Ah
  i2c_stop();
}

void settings_writeAlarm2DS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x0B);

  i2c_write(settings.alarm[ALARM2_MINUTES]);  //0Bh
  i2c_write(settings.alarm[ALARM2_HOURS]);  //0Ch
  i2c_write(settings.alarm[ALARM2_DAY_DATE]);  //0Dh
  i2c_stop();
}

void settings_writeSettingsDS3232() {
  i2c_start_wait(DS3232 + I2C_WRITE);
  i2c_write(0x14);

  i2c_write(settings.main[BRIGHTNESS]);
  i2c_write(settings.main[TIME_FORMAT_12H]);
  i2c_write(settings.main[DATE_FORMAT_DD_MM]);

  i2c_write(settings.main[REST_ON_HOUR]);
  i2c_write(settings.main[REST_ON_MINUTE]);
  i2c_write(settings.main[WAKE_ON_HOUR]);
  i2c_write(settings.main[WAKE_ON_MINUTE]);

  i2c_write(settings.not_first_boot);
  i2c_stop();
}
