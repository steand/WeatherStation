EESchema Schematic File Version 4
LIBS:WeatherStation-cache
LIBS:Module-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title "DIY Weather Station"
Date "2019-07-24"
Rev "0.2"
Comp "C2019 by Stefan Andres (develop@myandres.de)"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Notes Line
	1200 6050 3750 6050
Wire Notes Line
	1200 4550 3750 4550
NoConn ~ 3600 5400
$Comp
L Connector:Conn_01x06_Male J?
U 1 1 5D5F5F36
P 3800 5300
AR Path="/5D5F5F36" Ref="J?"  Part="1" 
AR Path="/5D5D3ED6/5D5F5F36" Ref="J22"  Part="1" 
F 0 "J22" H 3772 5274 50  0001 R CNN
F 1 "Prog" H 3772 5228 50  0000 R CNN
F 2 "Socket_Strips:Socket_Strip_Angled_1x06_Pitch2.54mm" H 3800 5300 50  0001 C CNN
F 3 "~" H 3800 5300 50  0001 C CNN
	1    3800 5300
	-1   0    0    -1  
$EndComp
Wire Notes Line
	3750 4550 3750 6050
Wire Notes Line
	1200 6050 1200 4550
Text Notes 900  5900 1    50   ~ 0
To CP2102 USB-TTL Programmer
Wire Wire Line
	3200 5300 3600 5300
Wire Wire Line
	3200 4850 3200 5300
Wire Wire Line
	1600 4850 3200 4850
Wire Wire Line
	3250 5200 3600 5200
Wire Wire Line
	3250 4750 3250 5200
Wire Wire Line
	3450 5600 3600 5600
Wire Wire Line
	3450 5950 3450 5600
Wire Wire Line
	1600 5950 3450 5950
Wire Wire Line
	3150 5500 3600 5500
Wire Wire Line
	3150 5900 3150 5500
Wire Wire Line
	2750 5900 3150 5900
Wire Wire Line
	3150 5100 3600 5100
Wire Wire Line
	3150 5400 3150 5100
Wire Wire Line
	2750 5400 3150 5400
Text Label 3350 5500 0    50   ~ 0
PROG
Text Label 3350 5300 0    50   ~ 0
RxD
Text Label 3350 5200 0    50   ~ 0
TxD
Text Label 3350 5100 0    50   ~ 0
RST
Text Notes 1400 4650 0    50   ~ 0
Programmer
Wire Wire Line
	1850 5400 1850 5700
Connection ~ 1850 5400
Wire Wire Line
	1950 5300 1950 5200
Connection ~ 1950 5300
Wire Wire Line
	1850 5700 2050 5700
Wire Wire Line
	1850 4900 1850 5400
Wire Wire Line
	2750 4900 1850 4900
Wire Wire Line
	2750 5000 2750 4900
Wire Wire Line
	2500 5500 2750 5500
Wire Wire Line
	1950 5200 2050 5200
Wire Wire Line
	1950 5400 1950 5300
Wire Wire Line
	2500 5400 1950 5400
Wire Wire Line
	2500 5500 2500 5400
Wire Wire Line
	2350 5700 2450 5700
Wire Wire Line
	2350 5200 2450 5200
$Comp
L Device:R R?
U 1 1 5D5F5F68
P 2200 5700
AR Path="/5D5F5F68" Ref="R?"  Part="1" 
AR Path="/5D5D3ED6/5D5F5F68" Ref="R5"  Part="1" 
F 0 "R5" V 1993 5700 50  0000 C CNN
F 1 "4k7" V 2084 5700 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 2130 5700 50  0001 C CNN
F 3 "~" H 2200 5700 50  0001 C CNN
	1    2200 5700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5D5F5F6E
P 2200 5200
AR Path="/5D5F5F6E" Ref="R?"  Part="1" 
AR Path="/5D5D3ED6/5D5F5F6E" Ref="R4"  Part="1" 
F 0 "R4" V 1993 5200 50  0000 C CNN
F 1 "4k7" V 2084 5200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 2130 5200 50  0001 C CNN
F 3 "~" H 2200 5200 50  0001 C CNN
	1    2200 5200
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:BC548 Q?
U 1 1 5D5F5F74
P 2650 5700
AR Path="/5D5F5F74" Ref="Q?"  Part="1" 
AR Path="/5D5D3ED6/5D5F5F74" Ref="Q3"  Part="1" 
F 0 "Q3" H 2841 5746 50  0000 L CNN
F 1 "BC548" H 2841 5655 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 2850 5625 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 2650 5700 50  0001 L CNN
	1    2650 5700
	1    0    0    1   
$EndComp
$Comp
L Transistor_BJT:BC548 Q?
U 1 1 5D5F5F7A
P 2650 5200
AR Path="/5D5F5F7A" Ref="Q?"  Part="1" 
AR Path="/5D5D3ED6/5D5F5F7A" Ref="Q2"  Part="1" 
F 0 "Q2" H 2841 5246 50  0000 L CNN
F 1 "BC548" H 2841 5155 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 2850 5125 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 2650 5200 50  0001 L CNN
	1    2650 5200
	1    0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J21
U 1 1 5D568003
P 3600 3300
F 0 "J21" H 3550 2950 50  0000 L CNN
F 1 "Conn_01x05" H 3350 3650 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 3600 3300 50  0001 C CNN
F 3 "~" H 3600 3300 50  0001 C CNN
	1    3600 3300
	1    0    0    1   
$EndComp
$Comp
L Regulator_Linear:MIC5501-3.0YM5 U11
U 1 1 5D566303
P 2600 3300
F 0 "U11" H 2750 3000 50  0000 L CNN
F 1 "MIC5501-3.0YM5" H 2300 3600 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 2600 2900 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/MIC550X.pdf" H 2350 3550 50  0001 C CNN
	1    2600 3300
	1    0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J20
U 1 1 5D5687DA
P 1550 3300
F 0 "J20" H 1500 2950 50  0000 L CNN
F 1 "Conn_01x05" H 1300 3650 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 1550 3300 50  0001 C CNN
F 3 "~" H 1550 3300 50  0001 C CNN
	1    1550 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	3000 3400 3250 3400
Wire Wire Line
	2150 3300 2150 2900
Wire Wire Line
	2150 2900 2600 2900
Wire Wire Line
	2600 2900 2600 3000
Wire Wire Line
	3250 3400 3250 3200
Wire Wire Line
	3400 3200 3250 3200
Wire Wire Line
	2150 3300 1750 3300
Wire Wire Line
	2200 3200 1850 3200
Wire Wire Line
	2200 3400 1850 3400
Wire Wire Line
	1850 3200 1850 3400
Connection ~ 1850 3200
Wire Wire Line
	1850 3200 1750 3200
Connection ~ 1850 3400
Wire Wire Line
	1850 3400 1750 3400
NoConn ~ 1750 3100
NoConn ~ 1750 3500
NoConn ~ 3400 3100
NoConn ~ 3400 3300
NoConn ~ 3400 3400
NoConn ~ 3400 3500
$Comp
L Module:Mod_MIC5501-3.0YM5-TR U10
U 1 1 5D5C43E4
P 2550 1700
F 0 "U10" H 2550 2125 50  0000 C CNN
F 1 "Mod_MIC5501-3.0YM5-TR" H 2550 2034 50  0000 C CNN
F 2 "Module:Mod_SOT23_TO_10DIL" H 2550 1300 50  0001 C CNN
F 3 "" H 2450 1700 50  0001 C CNN
	1    2550 1700
	1    0    0    -1  
$EndComp
$Comp
L ESP:BAR43SFILM D1
U 1 1 5D63877D
P 5850 3250
F 0 "D1" V 5896 3329 50  0000 L CNN
F 1 "BAR43SFILM" V 5250 3000 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5850 3250 50  0001 C CNN
F 3 "~" H 5850 3250 50  0001 C CNN
	1    5850 3250
	0    1    -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 5D63940E
P 5500 3050
F 0 "R7" H 5570 3096 50  0000 L CNN
F 1 "470K" H 5570 3005 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 5430 3050 50  0001 C CNN
F 3 "~" H 5500 3050 50  0001 C CNN
	1    5500 3050
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5D639D06
P 5500 3450
F 0 "R8" H 5570 3496 50  0000 L CNN
F 1 "1M" H 5570 3405 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 5430 3450 50  0001 C CNN
F 3 "~" H 5500 3450 50  0001 C CNN
	1    5500 3450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5650 3250 5500 3250
Wire Wire Line
	5500 3250 5500 3200
Wire Wire Line
	5500 3300 5500 3250
Connection ~ 5500 3250
$Comp
L Connector_Generic:Conn_01x05 J23
U 1 1 5D63A4A1
P 4850 3250
F 0 "J23" H 4800 3550 50  0000 L CNN
F 1 "Conn_01x05" V 4950 3050 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x05_Pitch2.54mm" H 4850 3250 50  0001 C CNN
F 3 "~" H 4850 3250 50  0001 C CNN
	1    4850 3250
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J24
U 1 1 5D63AA23
P 6400 3300
F 0 "J24" H 6400 3000 50  0000 C CNN
F 1 "Conn_01x05" V 6500 3300 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x05_Pitch2.54mm" H 6400 3300 50  0001 C CNN
F 3 "~" H 6400 3300 50  0001 C CNN
	1    6400 3300
	1    0    0    1   
$EndComp
Wire Wire Line
	5500 3250 5150 3250
Wire Wire Line
	5150 3250 5150 3150
Wire Wire Line
	5150 3050 5050 3050
Connection ~ 5150 3250
Wire Wire Line
	5150 3250 5050 3250
Wire Wire Line
	5150 3250 5150 3350
Wire Wire Line
	5150 3450 5050 3450
Wire Wire Line
	5150 3350 5050 3350
Connection ~ 5150 3350
Wire Wire Line
	5150 3350 5150 3450
Wire Wire Line
	5150 3150 5050 3150
Connection ~ 5150 3150
Wire Wire Line
	5150 3150 5150 3050
Wire Wire Line
	6200 3100 6200 2750
Wire Wire Line
	6200 2750 5500 2750
Wire Wire Line
	5500 2750 5500 2900
Wire Wire Line
	6200 3200 6100 3200
Wire Wire Line
	6100 3200 6100 2900
Wire Wire Line
	6100 2900 5850 2900
Wire Wire Line
	5850 2900 5850 2950
Wire Wire Line
	6200 3400 6100 3400
Wire Wire Line
	6100 3400 6100 3650
Wire Wire Line
	6100 3650 6000 3650
Wire Wire Line
	5850 3650 5850 3550
Wire Wire Line
	6200 3500 6200 3700
Wire Wire Line
	6200 3700 6000 3700
Wire Wire Line
	5500 3700 5500 3600
Wire Wire Line
	6000 3700 6000 3650
Wire Wire Line
	6000 3700 5500 3700
Wire Wire Line
	6000 3650 5850 3650
Connection ~ 6000 3650
Connection ~ 6000 3700
$Comp
L Module:Mod_Spannungsteiler U12
U 1 1 5D5DB760
P 5700 1700
F 0 "U12" H 5650 2125 50  0000 C CNN
F 1 "Mod_Spannungsteiler" H 5650 2034 50  0000 C CNN
F 2 "Module:Mod_SOT23_TO_10DIL" H 5600 1300 50  0001 C CNN
F 3 "" H 5500 1700 50  0001 C CNN
	1    5700 1700
	1    0    0    -1  
$EndComp
$Comp
L Module:Mod_74LVC2G14GW U13
U 1 1 5D5DCE2D
P 8750 1850
F 0 "U13" H 8750 2431 50  0000 C CNN
F 1 "Mod_74LVC2G14GW" H 8750 2340 50  0000 C CNN
F 2 "Module:Mod_TSOP6_TO_8DIL" H 9350 1400 50  0001 C CNN
F 3 "" H 8700 2050 50  0001 C CNN
	1    8750 1850
	1    0    0    -1  
$EndComp
$Comp
L 74xGxx:74LVC2G14 U14
U 1 1 5D5DDCEE
P 8800 2850
F 0 "U14" H 8775 3117 50  0000 C CNN
F 1 "74LVC2G14" H 8775 3026 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-363_SC-70-6" H 8800 2850 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/sn74lvc2g14.pdf" H 8800 2850 50  0001 C CNN
	1    8800 2850
	1    0    0    -1  
$EndComp
$Comp
L 74xGxx:74LVC2G14 U14
U 2 1 5D5DED14
P 8850 3500
F 0 "U14" H 8825 3767 50  0000 C CNN
F 1 "74LVC2G14" H 8825 3676 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-363_SC-70-6" H 8850 3500 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/sn74lvc2g14.pdf" H 8850 3500 50  0001 C CNN
	2    8850 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J26
U 1 1 5D5E188E
P 9700 3250
F 0 "J26" H 9618 2825 50  0000 C CNN
F 1 "Conn_01x04" H 9618 2916 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04_Pitch2.54mm" H 9700 3250 50  0001 C CNN
F 3 "~" H 9700 3250 50  0001 C CNN
	1    9700 3250
	1    0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J25
U 1 1 5D5E265A
P 7950 3150
F 0 "J25" H 7868 3467 50  0000 C CNN
F 1 "Conn_01x04" H 7868 3376 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04_Pitch2.54mm" H 7950 3150 50  0001 C CNN
F 3 "~" H 7950 3150 50  0001 C CNN
	1    7950 3150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8150 3050 8350 3050
Wire Wire Line
	8350 3050 8350 2850
Wire Wire Line
	8350 2850 8500 2850
Wire Wire Line
	8150 3250 8350 3250
Wire Wire Line
	8350 3250 8350 3500
Wire Wire Line
	8350 3500 8550 3500
Wire Wire Line
	9050 2850 9300 2850
Wire Wire Line
	9300 2850 9300 3050
Wire Wire Line
	9300 3050 9500 3050
Wire Wire Line
	9100 3500 9300 3500
Wire Wire Line
	9300 3500 9300 3250
Wire Wire Line
	9300 3250 9500 3250
Wire Wire Line
	8150 3150 8450 3150
Wire Wire Line
	8800 3150 8800 2950
Wire Wire Line
	8450 3150 8450 3700
Wire Wire Line
	8450 3700 8850 3700
Wire Wire Line
	8850 3700 8850 3600
Connection ~ 8450 3150
Wire Wire Line
	8450 3150 8800 3150
Wire Wire Line
	8850 3400 9200 3400
Wire Wire Line
	9200 3400 9200 3150
Wire Wire Line
	9200 3150 9500 3150
Wire Wire Line
	9200 3150 9200 2750
Wire Wire Line
	9200 2750 8800 2750
Connection ~ 9200 3150
Text Notes 9250 3150 0    50   ~ 0
VCC
Text Notes 8150 3150 0    50   ~ 0
GND
Wire Notes Line
	1200 1150 3950 1150
Wire Notes Line
	3950 1150 3950 4000
Wire Notes Line
	3950 4000 1200 4000
Wire Notes Line
	1200 4000 1200 1150
Wire Notes Line
	4550 1150 6800 1150
Wire Notes Line
	6800 1150 6800 4050
Wire Notes Line
	6800 4050 4550 4050
Wire Notes Line
	4550 4050 4550 1150
Wire Notes Line
	7600 1150 10000 1150
Wire Notes Line
	10000 1150 10000 4050
Wire Notes Line
	10000 4050 7600 4050
Wire Notes Line
	7600 4050 7600 1150
$Comp
L Connector_Generic:Conn_01x05 J27
U 1 1 5D63D3DE
P 1200 5300
F 0 "J27" H 1200 5650 50  0000 C CNN
F 1 "Conn_01x05" V 1300 5300 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x05_Pitch2.54mm" H 1200 5300 50  0001 C CNN
F 3 "~" H 1200 5300 50  0001 C CNN
	1    1200 5300
	-1   0    0    -1  
$EndComp
Text Notes 1650 5300 0    50   ~ 0
DTR
Text Notes 1650 5400 0    50   ~ 0
RTS
Text Notes 1650 4750 0    50   ~ 0
TxD
Text Notes 1650 4850 0    50   ~ 0
RxD
Text Notes 1650 5950 0    50   ~ 0
GND
Wire Wire Line
	1400 5300 1950 5300
Wire Wire Line
	1400 5400 1850 5400
Wire Wire Line
	1600 5950 1600 5500
Wire Wire Line
	1600 5500 1400 5500
Wire Wire Line
	1600 4850 1600 5200
Wire Wire Line
	1600 5200 1400 5200
Wire Wire Line
	1550 4750 1550 5100
Wire Wire Line
	1550 5100 1400 5100
Wire Wire Line
	1550 4750 3250 4750
$EndSCHEMATC
