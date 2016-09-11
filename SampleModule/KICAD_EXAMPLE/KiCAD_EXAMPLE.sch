EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:KiCAD_EXAMPLE_Lib
LIBS:KiCAD_EXAMPLE-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 4150 2900 0    60   ~ 0
VCC_SYSTEM
Text Label 4150 3000 0    60   ~ 0
VIN
$Comp
L GND #PWR01
U 1 1 57CDFECD
P 4850 3300
F 0 "#PWR01" H 4850 3050 50  0001 C CNN
F 1 "GND" H 4850 3150 50  0000 C CNN
F 2 "" H 4850 3300 50  0000 C CNN
F 3 "" H 4850 3300 50  0000 C CNN
	1    4850 3300
	1    0    0    -1  
$EndComp
Text Label 4150 3200 0    60   ~ 0
SDA
Text Label 3200 2900 0    60   ~ 0
GPIO6
Text Label 3200 3000 0    60   ~ 0
GPIO7
Text Label 3200 3100 0    60   ~ 0
GPIO8
Text Label 3200 3200 0    60   ~ 0
GPIO9
Text Label 3200 3300 0    60   ~ 0
INT
$Comp
L CONN_01X06 P1
U 1 1 57CEE3F5
P 6450 4400
F 0 "P1" H 6450 4750 50  0000 C CNN
F 1 "CONN_01X06" V 6550 4400 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x06" H 6450 4400 50  0001 C CNN
F 3 "" H 6450 4400 50  0000 C CNN
	1    6450 4400
	1    0    0    1   
$EndComp
Text Label 5700 4150 0    60   ~ 0
SCL
Text Label 5700 4250 0    60   ~ 0
SDA
Text Label 5700 4350 0    60   ~ 0
VCC_SYSTEM
$Comp
L GND #PWR02
U 1 1 57CEE4EB
P 5200 4500
F 0 "#PWR02" H 5200 4250 50  0001 C CNN
F 1 "GND" H 5200 4350 50  0000 C CNN
F 2 "" H 5200 4500 50  0000 C CNN
F 3 "" H 5200 4500 50  0000 C CNN
	1    5200 4500
	1    0    0    -1  
$EndComp
Text Label 5700 4550 0    60   ~ 0
INT
Text GLabel 3100 2900 0    60   Input ~ 0
GPIO6
Text GLabel 3100 3000 0    60   Input ~ 0
GPIO7
Text GLabel 3100 3100 0    60   Input ~ 0
GPIO8
Text GLabel 3100 3200 0    60   Input ~ 0
GPIO9
Text GLabel 3100 3300 0    60   Input ~ 0
INT
Wire Wire Line
	4850 3100 4850 3300
Wire Wire Line
	3500 2900 3100 2900
Wire Wire Line
	3100 3000 3500 3000
Wire Wire Line
	3100 3100 3500 3100
Wire Wire Line
	3100 3200 3500 3200
Wire Wire Line
	3100 3300 3500 3300
Wire Wire Line
	4500 2900 4050 2900
Wire Wire Line
	4500 3000 4050 3000
Wire Wire Line
	5200 4450 6250 4450
Wire Wire Line
	5700 4150 6250 4150
Wire Wire Line
	5700 4250 6250 4250
Wire Wire Line
	5700 4350 6250 4350
Wire Wire Line
	5700 4550 6250 4550
$Comp
L connector J1
U 1 1 57CEFF02
P 3400 3300
F 0 "J1" H 3750 3900 60  0000 C CNN
F 1 "connector" H 3700 3800 60  0000 C CNN
F 2 "KiCAD_EXAMPLE:BoardConnectorFront" H 3550 3300 60  0001 C CNN
F 3 "" H 3550 3300 60  0001 C CNN
	1    3400 3300
	1    0    0    -1  
$EndComp
Text GLabel 4500 2900 2    60   Input ~ 0
VCC_SYSTEM
Text GLabel 4500 3000 2    60   Input ~ 0
VIN
Text GLabel 4500 3200 2    60   Input ~ 0
SDA
Text GLabel 4500 3300 2    60   Input ~ 0
SCL
Wire Wire Line
	4500 3300 4050 3300
Text Label 6750 2850 0    60   ~ 0
VCC_SYSTEM
Text Label 6750 2950 0    60   ~ 0
VIN
$Comp
L GND #PWR03
U 1 1 57CF00CF
P 7700 3100
F 0 "#PWR03" H 7700 2850 50  0001 C CNN
F 1 "GND" H 7700 2950 50  0000 C CNN
F 2 "" H 7700 3100 50  0000 C CNN
F 3 "" H 7700 3100 50  0000 C CNN
	1    7700 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 2850 6650 2850
Wire Wire Line
	7100 2950 6650 2950
Text GLabel 7100 2850 2    60   Input ~ 0
VCC_SYSTEM
Text GLabel 7100 2950 2    60   Input ~ 0
VIN
Text GLabel 7100 3150 2    60   Input ~ 0
SDA
Text GLabel 7100 3250 2    60   Input ~ 0
SCL
Wire Wire Line
	7100 3250 6650 3250
Text Label 5800 2850 0    60   ~ 0
GPIO6
Text Label 5800 2950 0    60   ~ 0
GPIO7
Text Label 5800 3050 0    60   ~ 0
GPIO8
Text Label 5800 3150 0    60   ~ 0
GPIO9
Text Label 5800 3250 0    60   ~ 0
INT
Text GLabel 5700 2850 0    60   Input ~ 0
GPIO6
Text GLabel 5700 2950 0    60   Input ~ 0
GPIO7
Text GLabel 5700 3050 0    60   Input ~ 0
GPIO8
Text GLabel 5700 3150 0    60   Input ~ 0
GPIO9
Text GLabel 5700 3250 0    60   Input ~ 0
INT
Wire Wire Line
	6100 2850 5700 2850
Wire Wire Line
	5700 2950 6100 2950
Wire Wire Line
	5700 3050 6100 3050
Wire Wire Line
	5700 3150 6100 3150
Wire Wire Line
	5700 3250 6100 3250
Text GLabel 5700 4350 0    60   Input ~ 0
VCC_SYSTEM
Text GLabel 5700 4250 0    60   Input ~ 0
SDA
Text GLabel 5700 4150 0    60   Input ~ 0
SCL
Text GLabel 5700 4550 0    60   Input ~ 0
IINT
Wire Wire Line
	5200 4500 5200 4450
$Comp
L connector J2
U 1 1 57CF0680
P 6000 3250
F 0 "J2" H 6350 3850 60  0000 C CNN
F 1 "connector" H 6300 3750 60  0000 C CNN
F 2 "KiCAD_EXAMPLE:boardConnector_back" H 6150 3250 60  0001 C CNN
F 3 "" H 6150 3250 60  0001 C CNN
	1    6000 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 3050 7700 3050
Connection ~ 7700 3050
Wire Wire Line
	7700 3050 7700 3100
Wire Wire Line
	4850 3100 4050 3100
Wire Wire Line
	4050 3200 4500 3200
Wire Wire Line
	6650 3150 7100 3150
$EndSCHEMATC
