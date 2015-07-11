##Software of the Divergence Meter Project

To upload to an AVR chip AVRDude is supported, just modify the makefile programming settings and type "make program_flash", please lookup your settings in the AVRDude manual. Otherwise if you have another upload method, you are welcome to use the generated binaries using "make [hex|bin|srec]"

Libraries used:

- Peter Fleury's I2C library: http://homepage.hispeed.ch/peterfleury/avr-software.html