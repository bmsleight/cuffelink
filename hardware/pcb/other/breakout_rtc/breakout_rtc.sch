EESchema Schematic File Version 2  date Mon 20 Aug 2012 20:01:22 BST
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
LIBS:RV-8564-C2
LIBS:breakout_rtc-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "20 aug 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4600 1950 5500 1950
Wire Wire Line
	5500 1950 5500 1300
Wire Wire Line
	5500 1300 5000 1300
Wire Wire Line
	4600 2150 5000 2150
Wire Wire Line
	5000 2150 5000 1500
Wire Wire Line
	3900 2050 3400 2050
Wire Wire Line
	3400 2050 3400 1300
Wire Wire Line
	3400 1300 3500 1300
Wire Wire Line
	3900 2250 3150 2250
Wire Wire Line
	3150 2250 3150 1400
Wire Wire Line
	3150 1400 3500 1400
Wire Wire Line
	3500 1200 3350 1200
Wire Wire Line
	3350 1200 3350 1950
Wire Wire Line
	3350 1950 3900 1950
Wire Wire Line
	3500 1500 3500 2550
Wire Wire Line
	3500 2550 4600 2550
Wire Wire Line
	4600 2550 4600 2250
Wire Wire Line
	5000 1400 5200 1400
Wire Wire Line
	5200 1400 5200 2050
Wire Wire Line
	5200 2050 4600 2050
$Comp
L DIL8 P1
U 1 1 50328819
P 4250 2100
F 0 "P1" H 4250 2350 60  0000 C CNN
F 1 "DIL8" V 4250 2100 50  0000 C CNN
	1    4250 2100
	1    0    0    -1  
$EndComp
$Comp
L RV-8564-C2 U1
U 1 1 50328746
P 4250 1350
F 0 "U1" H 4250 1250 50  0000 C CNN
F 1 "RV-8564-C2" H 4250 1450 50  0000 C CNN
F 2 "MODULE" H 4250 1350 50  0001 C CNN
F 3 "DOCUMENTATION" H 4250 1350 50  0001 C CNN
	1    4250 1350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
