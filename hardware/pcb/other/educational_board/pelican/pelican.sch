EESchema Schematic File Version 2  date Thu 01 Nov 2012 14:13:27 GMT
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
LIBS:special
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
LIBS:attiny_
LIBS:pelican-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "1 nov 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 7050 2700
Connection ~ 6750 2700
Wire Wire Line
	8600 2700 2650 2700
Connection ~ 8250 2700
Wire Wire Line
	5800 1300 8600 1300
Wire Wire Line
	5800 1700 7900 1700
Wire Wire Line
	5800 2100 7400 2100
Wire Wire Line
	7400 2100 7400 2300
Wire Wire Line
	8250 2300 8250 1500
Wire Wire Line
	6750 2700 6750 2300
Wire Wire Line
	2650 2700 2650 2350
Wire Wire Line
	2100 1750 2650 1750
Wire Wire Line
	2650 1750 2650 2150
Wire Wire Line
	2650 2350 2100 2350
Wire Wire Line
	6150 2300 5800 2300
Wire Wire Line
	7900 1700 7900 2300
Wire Wire Line
	8600 1300 8600 2300
Wire Wire Line
	7050 2300 7050 1900
Wire Wire Line
	7050 1900 5800 1900
Wire Wire Line
	8250 1500 5800 1500
Connection ~ 7900 2700
Connection ~ 7400 2700
$Comp
L LED D5
U 1 1 5092816D
P 7400 2500
F 0 "D5" H 7400 2600 50  0000 C CNN
F 1 "LED" H 7400 2400 50  0000 C CNN
	1    7400 2500
	0    1    1    0   
$EndComp
$Comp
L LED D4
U 1 1 50928169
P 7050 2500
F 0 "D4" H 7050 2600 50  0000 C CNN
F 1 "LED" H 7050 2400 50  0000 C CNN
	1    7050 2500
	0    1    1    0   
$EndComp
$Comp
L LED D3
U 1 1 5092815E
P 8600 2500
F 0 "D3" H 8600 2600 50  0000 C CNN
F 1 "LED" H 8600 2400 50  0000 C CNN
	1    8600 2500
	0    1    1    0   
$EndComp
$Comp
L LED D2
U 1 1 5092814C
P 8250 2500
F 0 "D2" H 8250 2600 50  0000 C CNN
F 1 "LED" H 8250 2400 50  0000 C CNN
	1    8250 2500
	0    1    1    0   
$EndComp
$Comp
L LED D1
U 1 1 5092813D
P 7900 2500
F 0 "D1" H 7900 2600 50  0000 C CNN
F 1 "LED" H 7900 2400 50  0000 C CNN
	1    7900 2500
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 5092801A
P 6450 2300
F 0 "SW1" H 6600 2410 50  0000 C CNN
F 1 "SW_PUSH" H 6450 2220 50  0000 C CNN
	1    6450 2300
	1    0    0    -1  
$EndComp
$Comp
L BATTERY CELL1
U 1 1 50927E9E
P 2100 2050
F 0 "CELL1" H 2100 2250 50  0000 C CNN
F 1 "CR1220" H 2100 1860 50  0000 C CNN
	1    2100 2050
	0    1    1    0   
$EndComp
$Comp
L ATTINY25-45-85/DIP-SO IC1
U 1 1 509279D7
P 4200 1800
F 0 "IC1" H 4175 2425 60  0000 C CNN
F 1 "ATTINY25-45-85/DIP-SO" H 4200 2575 60  0000 C CNN
	1    4200 1800
	1    0    0    -1  
$EndComp
$EndSCHEMATC
