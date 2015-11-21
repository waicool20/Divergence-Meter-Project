# Divergence Meter User's Manual

Learn how to use the divergence meter with this manual!

Buttons will be referred to as B1, B2, B3, B4, B5 going from left to right with the digits facing towards you.

## All Modes

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| B1 | Switch mode | Go to device settings | N\A |


## Clock Mode

This is the first mode that appears when the divergence meter boots up. The format is HH-MM-SS. Every minute the 
tubes will be rolled to prevent cathode poisoning and the date and day of week will show up afterwards. 
The date format will depend on your settings, which can either be MM-DD-YY or DD-MM-YY. On the other hand day of week 
is user defined so if your week starts on sunday then you can set that as 01 otherwise if your week starts on monday 
then you can set monday will become 01 and sunday will become 07

Wehn you press B3 to arm an alarm, the most middle tubes will indicate which alarm is active, the left tube will indicate alarm 1 and the right tube will indicate alarm 2.
1 means armed and 0 means disarmed.

Button Functions:

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| B2 | Show date and day of week | Go to time settings mode |  |
| B3 | Toggle alarms | Go to alarm settings mode |  |
| B4 |  |  |  Roll a random world line |
| B5 |  |  | Toggle Brightness |

## Divergence Mode

This is the mode where you can play with world lines as much as you want, you can cycle through preset world lines from the anime/VN, roll a random world line or manually set your own world line by going to the Divergence Edit Mode.

Button Functions:

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| B2 | Prev preset | Combo see below |  |
| B3 | Next preset | Combo see below |  |
| B4 |  |  |  Roll a random world line |
| B5 |  |  | Toggle Brightness |
| B2 + B3 |  | Go to Divergence Edit Mode |  |

## Divergence Edit Mode

This is the mode where you can enter any world line manually. It will then return to Divergence Mode after you set all the digits. Digits can be set from 0-9 and blank character. It will skip tube2 as it is always a decimal point.

Button Functions:

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| B2 |  |  | Decrement |
| B3 |  |  | Increment |
| B4 | Next Tube |  |  |
| B5 | Prev Tube |  |  |

## Settings Mode

This is the mode where you can set different kind of parameters to change the meter's behaviour, refer to **All Modes** to see how to access this mode.  On the leftmost side there will be a number showing what setting you are currently configuring, and on the rightmost side it will show you its current value.
The resting and waking times are also set here. More on those below.

Settings:

| Number | Default | Min Value | Max Value | Description |
|:------:|:-------:|:---------:|:---------:|:-----------:|
| 01 | 0 | 0 | 1 | 0 for 24H time, 1 for  12H time |
| 02 | 0 | 0 | 1 | 0 for MM-DD-YY dates, 1 for DD-MM-YY dates |
| 03 | 0 | 0 | 1 | 0 for silent buttons, 1 for beep on press |
| 04 | 0 | 0 | 23 | Combined with settings 05 to set the start time of resting time |
| 05 | 0 | 0 | 59 | Combined with settings 04 to set the start time of resting time |
| 06 | 0 | 0 | 23 | Combined with settings 07 to set the start time of waking time |
| 07 | 0 | 0 | 59 | Combined with settings 06 to set the start time of waking time |

Button Functions:

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| B2 |  |  | Decrement |
| B3 |  |  | Increment |
| B4 | Next Setting |  |  |
| B5 | Prev Setting |  |  |

## Clock Set Mode

This is the mode where you can set your time, refer to **Clock Mode** to see how to access this mode. On the leftmost side there will be a number showing what setting you are currently configuring, and on the rightmost side it will show you its current value.

Settings:

| Number | Default | Min Value | Max Value | Description |
|:------:|:-------:|:---------:|:---------:|:-----------:|
| 01 | 0 | 0 | 99 | Year |
| 02 | 0 | 1 | 12 | Month |
| 03 | 0 | 1 | 31 | Date |
| 04 | 0 | 1 | 7 | Day of week |
| 05 | 0 | 0 | 23 | Hours |
| 06 | 0 | 0 | 23 | Minutes |
| 07 | 0 | 0 | 59 | Seconds |

Button Functions:

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| B2 |  |  | Decrement |
| B3 |  |  | Increment |
| B4 | Next Setting |  |  |
| B5 | Prev Setting |  |  |

## Alarm Set Mode

This is the mode where you can set the alarms, there are a total of 2 alarms, the main difference being that you can set the exact second in alarm 1 but not in alarm 2.

When you first enter this mode you will give the choice to choose which alarm to set.
after which you can set the settings as below.

Settings:

| Number | Default | Min Value | Max Value | Description |
|:------:|:-------:|:---------:|:---------:|:-----------:|
| 01 | 0 | 0 | 7 | Day of week, 0 means any day|
| 02 | 0 | 0 | 23 | Hours |
| 03 | 0 | 0 | 23 | Minutes |
| 04 | 0 | 0 | 59 | Seconds |

Button Functions:

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| B2 |  |  | Decrement |
| B3 |  |  | Increment |
| B4 | Next Setting |  |  |
| B5 | Prev Setting |  |  |

## Rest Mode

This is the mode where the device will turn of the display to conserve energy and extend battery life. The time to turn off and turn on the display can be set in **Settings Mode** This mode cannot be accessed manually. When waking up, it will always go to clock mode.

| Button | Short Press | Long Press | Pressed |
|:------:|:-----------:|:----------:|:-------:|
| All |  |  | Wake up |

## Other

### Brightness levels:

The brightness of the device can be toggled through 11 different light intensity, please refer to the above to see how. Manual brightness levels range from 0-9 while brightness level 10 turns on the adaptive brightness functionality, the device will then adapt to the current light level of the environment and will set the brightness accordingly, basically it turns darker in dark environments to save power and it will turns brighter in brighter environments so that the display can be seen properly. Please note that the device detects brightness through the LDR (Light Dependent Resistor), if its covered by something or just that section is in a shaded region then it the device will respond as if it were in that environment.

### Power Supply
The device provides 2 power supply pcb pads, one for the batteries and one for the DC jack. Please choose a DC jack according to your size preference when  making your own device. Though please choose a power supply that can at least provide 12V, otherwise the nixie tubes will appear as if they had cathode poisoning. The maximum power supply voltage at any time should not exceed 18V. The battery terminals can be left floating in the case without any worries that it will short out, as the terminals are disconnected by the power select switch.

## FAQ

//Ask me some questions first
