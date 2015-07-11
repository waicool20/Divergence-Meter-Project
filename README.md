#Divergence Meter Project

##Introduction
Hi~ I'm waicool20, this is the Divergence Meter Project! Just in case you're wondering what project this is about, it's actually a project to make the Divergence Meter from the anime Steins;Gate a reality (If you still don't know what that is, then I'm gonna start questioning why you're here lol, go google it). Basically it displays the current world line you're in. 

Well anyways originally this project is based on this guy's project: http://www.mindspring.com/~tomtitor/ and it has helped greatly so I guess give him some credit? I tried to improve his design by improving electronics design with power efficiency and mind-friendliness in mind (hence the AVR so we can code in C :D) I designed the thing in Diptrace, but I have also included Gerber/Drill files in a convenient zip so you can just hand it to some manufacturer to make some boards (remember to remind them that it's 2 separate boards in a single zip and not one!) included also are the schematic PDFs which are not quite convenient... orz anyways have a look around.

I just pulled this off my blog ( http://waicool20.ga ) and it's not all the differences I think...feel free to help me expand this list :p

> Additional Features:
> - 5 switches possible! Could be less if you tweak the software
> - Buzzer/Speaker output 😀 hurray for alarms in the morning
> - Light Sensor to detect night time to conserve power and extend Nixie tube life (could probably act as a basic motion sensor with some software magic)
> - Improvements/Slight changes:
> - Should be able to fit 2x9V batteries instead of one so more portable cosplay time :D
> - Should be able to fit 2xAA RTC backup battery so it keeps track of time accurately for theoretically over a year.
> - Interrupt based instead of Polling to conserve even more battery
> - Uses the larger but cheaper and more available version of the HV5622
> - Integrated switching power supply instead of a bought module
> - Uses an AVR instead of a PIC (I chose it because it can be reprogrammed by AVR tools and a PICKIT2 if you happen to have any of those)
> - Mostly SMD parts to make look all good when your showing off the innards XD
> - Miku silhouette included
>
> Limitations:
> - Can only use the DS3232 RTC and not the DS1307

##Project Structure
Well the Hardware and Software git branches are separate so that they can be developed independently easily.

Basically if you want the latest Software changes please switch to the software branch. Same thing for hardware. Though most often it will be the software branch that will be updated.

##About the licensing...
Well to keep it simple the hardware and the software are licensed with the TAPR OHL and GPLv3 respectively, if you are gonna use my work feel free to contact me at waicool20@gmail.com or not if that's how you swing. 

##Safety Disclaimer
**THIS PROJECT CONTAINS HIGH VOLTAGES (~170V) IT CAN KILL YOU IN THE WORST CASE SO BE CAREFUL.**  

The project has already minimized High Voltage (assuming you have a solder mask) so the only places you need to be careful are the parts around the LM3481, the HV pin on the board connector, and most of the pins on the Nixie Board. When constructing the device, please make sure no high voltage areas touch the case or anything that's not supposed to be "live" like seriously it will make your life easier.

When handling this device be aware that high voltage can still exist even when the device is completely powered off because of residual power in the capacitors, wait at least 3-5 seconds before touching anything on the pcb after unplugging the device.

Well you are at your own risk when you attempt this project, you can't hold me liable for anything that happens to you, your property or anybody/thing affected by your actions. 

El Psy Congroo,  
waicool20

