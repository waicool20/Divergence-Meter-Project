|                    RefDes                    |    Value   |        Name       | Quantity |
|:--------------------------------------------:|:----------:|:-----------------:|:--------:|
|                      B1                      |            |     CR2032 Bat. Holder |     1    |
|       C1, C2, C3, C4, C6, C8, C10, C13       |    0.1uF   |      CAP_0805     |     8    |
|                  C5, C7, C9                  |  100uF 16V |    E. Cap 2 5D    |     3    |
|                      C11                     | 4.7uF 250V |   E. Cap 3.5 8D   |     1    |
|                      C12                     |            | F1772-410-2200-E3 |     1    |
| | | Any safety capacitor with 10 or 15mm pin pitch will do, capacitance should be 0.1uF | |
|                      D1                      |            |     PMLL4148L     |     1    |
|                      D2                      |            |       MUR240      |     1    |
|                      J1                      |            |      HDR-1x6      |     1    |
|                      J2                      |            |       AVRISP      |     1    |
|                      J3                      |            |      DC Plug      |     1    |
|                      J4                      |            |    Power Select   |     1    |
|                      J5                      |            |      Battery      |     1    |
|                      J6                      |            |       ON/OFF      |     1    |
|                      J7                      |            |        SW1        |     1    |
|                      J8                      |            |        SW2        |     1    |
|                      J9                      |            |        SW3        |     1    |
|                      J10                     |            |        SW4        |     1    |
|                      J11                     |            |        SW5        |     1    |
|                      J12                     |            |      SPEAKER      |     1    |
|                      J13                     |            |        LDR        |     1    |
| | | The above pin headers (J1-J13) are optional, you can solder the wires directly to the PCB instead, refer to schematic for how many pins they are,
|                      J14                     |            |   Board connector |     1    |
|                      L1                      |    2.2uH   |       D52LC       |     1    |
|                      L2                      |    100uH   |     ELC16B101L    |     1    |
|										  	   |			| As for the above 2 inductors I didn't actually use those parts, instead I had a factory custom make some for me. Iirc they were 1.5A and 4A rated respectively. |
|                      Q1                      |            |       2N4401      |     1    |
|                      Q2                      |            |   IPB17N25S3-100  |     1    |
| | | Any mosfet with rating above 250V+ should do, but efficiency may drop if switch on/off is too slow |
| R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11 |     1K     |      RES_0805     |    11    |
|                      R12                     |    390K    |      RES_0805     |     1    |
|                 R13, R21, R22                |    100K    |      RES_0805     |     3    |
|                      R14                     |     75K    |      RES_0805     |     1    |
|                 R15, R16, R17                |    200K    |      RES_0805     |     3    |
|                      R18                     |    1.5K    |      RES_0805     |     1    |
|            R19, R23, R24, R25, R26           |    4.7K    |      RES_0805     |     5    |
|                      R20                     |    0.015   |      RES_2512     |     1    |
|                      U1                      |            |     ATtiny861A-SU    |     1    |
|                      U2                      |            |     DS3232SN#     |     1    |
|                      U3                      |            |      TPS62160DGKT     |     1    |
|                      U4                      |            |       LM3481MM/NOPB      |     1    |

Resistor and capacitors specifications don't really matter, just keep the tolerance low and keep it in the same package (I just pulled those off the shelf, don't even know what brand they were XD )

Sorry if some of the above parts can be a little misleading as I mostly just pulled this off the BOM export function of diptrace. So please refer to the schematics or board files and check what that part actually is using the RefDes 
