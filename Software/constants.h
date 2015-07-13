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

#define ROLL_SECONDS 2
#define ROLL_INTERVAL_MS 25
#define DISPLAY_SECONDS 3

#define ALARM_INT PA1

                    // |  ALL   |       CLOCK         | DIVERGENCE  |   SETTINGS    |
#define BUTTON1 PA3 // |  MODE  |                     |             |               |
#define BUTTON2 PA4 // |        | -/DATE              | -/LEFT      | -             |
#define BUTTON3 PA5 // |        | ROLL                | ROLL        | CONFIRM       |
#define BUTTON4 PA6 // |        | +/ARM/DISARM ALARM  | +/RIGHT     | +             |
#define BUTTON5 PA7 // |        | BRIGHTNESS          | BRIGHTNESS  | LAST SETTING  |

#define LE PB0
#define BL PB1
#define CLK PB2
#define DIN PB3
#define SPEAKER PB5
#define HV_DISABLE PB6

#define RDP 10
#define LDP 11
#define BLANK 12
#define DS3232 0xD0

#define CLOCK_MODE 0
#define DIVERGENCE_MODE 1
#define SETTINGS_MODE 2

#endif /* CONSTANTS_H_ */
