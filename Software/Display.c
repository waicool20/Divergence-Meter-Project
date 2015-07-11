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
 * This is the main display code which handles displaying the digits onto the nixie tube.
 * Numbers 0-11 can be written directly into the variables tube(1-8) and the display will be updated
 * once the updateDisplay() function is called.
 */

#include "Display.h"
#include "ShiftRegister.h"
#include "Constants.h"

char tube1 = 0;
char tube2 = 0;
char tube3 = 0;
char tube4 = 0;
char tube5 = 0;
char tube6 = 0;
char tube7 = 0;
char tube8 = 0;

void initDisplay() {
  SRSendZeros(96);
  SRLatch();
  SRON();
}

void handleShiftRegister1() {
  switch (tube6) {
    case 1:
    case 2:
    case 3:
      SRSendZeros(8);
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      SRSendZeros(tube6 - 4);
      SRSendOnes(1);
      SRSendZeros(11 - tube6);
      break;
    case 0:
      SRSendZeros(6);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(7);
      SRSendOnes(1);
      break;
  }

  switch (tube7) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      SRSendZeros(tube7);
      SRSendOnes(1);
      SRSendZeros(11 - tube7);
      break;
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
  }

  switch (tube8) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      SRSendZeros(tube8);
      SRSendOnes(1);
      SRSendZeros(11 - tube8);
      break;
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
  }
}

void handleShiftRegister2() {
  switch (tube3) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      SRSendZeros(4);
      break;
    case 8:
    case 9:
      SRSendZeros(tube3 - 8);
      SRSendOnes(1);
      SRSendZeros(11 - tube3);
      break;
    case 0:
      SRSendZeros(2);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(3);
      SRSendOnes(1);
      break;
  }

  switch (tube4) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      SRSendZeros(tube4);
      SRSendOnes(1);
      SRSendZeros(11 - tube4);
      break;
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
  }

  switch (tube5) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      SRSendZeros(tube5);
      SRSendOnes(1);
      SRSendZeros(11 - tube5);
      break;
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
  }

  switch (tube6) {
    case 1:
    case 2:
    case 3:
      SRSendZeros(tube6);
      SRSendOnes(1);
      SRSendZeros(3 - tube6);
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0:
    case RDP:
      SRSendZeros(4);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(3);
  }
}

void handleShiftRegister3() {
  switch (tube1) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      SRSendZeros(tube1);
      SRSendOnes(1);
      SRSendZeros(11 - tube1);
      break;
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
  }
  switch (tube2) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      SRSendZeros(tube2);
      SRSendOnes(1);
      SRSendZeros(11 - tube2);
      break;
    case 0:
      SRSendZeros(10);
      SRSendOnes(1);
      SRSendZeros(1);
      break;
    case RDP:
      SRSendZeros(11);
      SRSendOnes(1);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(11);
      break;
  }

  switch (tube3) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      SRSendZeros(tube3);
      SRSendOnes(1);
      SRSendZeros(7 - tube3);
      break;
    case 8:
    case 9:
    case 0:
    case RDP:
      SRSendZeros(8);
      break;
    case LDP:
      SRSendOnes(1);
      SRSendZeros(7);
      break;
  }
}

void updateDisplay() {
  handleShiftRegister1();
  handleShiftRegister2();
  handleShiftRegister3();
  SRLatch();
}
