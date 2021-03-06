/*  GPLv3 License
 *
 *   Copyright (c) Divergence Meter Project by waicool20
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
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define s2cs(a) (a*100)

#define TUBE1 0
#define TUBE2 1
#define TUBE3 2
#define TUBE4 3
#define TUBE5 4
#define TUBE6 5
#define TUBE7 6
#define TUBE8 7

#define ROLL_CONST 3 //Constant for when rolling a world line a larger value makes it roll longer
#define ROLL_INTERVAL_MS 25
#define ROLL_DISPLAY_SECONDS 1

#define ALARM_INT PA1

/* LP: Long Press                                                                                                         */
/*                   |  ALL   |       CLOCK                         | DIVERGENCE                        |   SETTINGS    | */
#define BUTTON1 4 // |  MODE  |                                     |                                   |               |
#define BUTTON2 3 // |        | -/DATE              LP:Edit Time    | -/Prev        --|--EDIT MODE      | -             |
#define BUTTON3 2 // |        | +/ARM/DISARM ALARM/ LP:Edit Alarm   | +/Next        --|                 | +             |
#define BUTTON4 1 // |        | ROLL/CONFIRM                        | ROLL/CONFIRM                      | CONFIRM       |
#define BUTTON5 0 // |        | BRIGHTNESS/BACK                     | BRIGHTNESS/BACK                   | Back          |
#define BUTTON1_PIN PA3
#define BUTTON2_PIN PA4
#define BUTTON3_PIN PA5
#define BUTTON4_PIN PA6
#define BUTTON5_PIN PA7

#define BUTTON_SHORT_PRESS_MIN_DURATION_MS 40
#define BUTTON_SHORT_PRESS_MAX_DURATION_MS 60
#define BUTTON_LONG_PRESS_MIN_DURATION_MS 1200

#define DATE_DISPLAY_S 1
#define DAY_DISPLAY_S 1
#define ALARM_ARM_DISPLAY_S 0.4
#define BRIGHTNESS_DISPLAY_S 0.25

#define LE PB0
#define BL PB1
#define CLK PB2
#define DIN PB3
#define SPEAKER PB5
#define HV_DISABLE PB6

#define BLANK 10
#define DS3232 0xD0

#define ALARM1 1
#define ALARM2 2
#define ALARM_RING_M 10
#define MAX_BEEPS 10
#define BEEP_INC_INTERVAL_S 20

#define CLOCK_MODE 0
#define DIVERGENCE_MODE 1

#define SETTINGS_MODE 2
#define DIVERGENCE_EDIT_MODE 3
#define ALARM_SET_MODE 4
#define CLOCK_SET_MODE 5
#define REST_MODE 6

#endif /* CONSTANTS_H_ */
