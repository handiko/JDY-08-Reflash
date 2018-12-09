
                   HM-10/11 CC2540/1 
V550
=======================================================================
Modify AT+FFE2 command add parameters '2'
AT+FFE2<P1> to query or change if use two UUID.
P1: ?   --Query
P1: 0   --Only use one Characteristic.
P1: 1   --Use two Characteristic, Second Characteristic value = first Characteristic value + 1
P1: 2   --Use two Characteristic, Second Characteristic value = first Characteristic value - 1
First Characteristic could use AT+CHAR command to setup.

Modify AT+SHOW command, add parameter '2' and parameter '3'
AT+SHOW<P1> used to query or set if show device name or device RSSI value when execute a discovery proceudre
P1: ?  -- Query
P1: 0  -- Dont show name or RSSI value
P1: 1  -- Show name
P1: 2  -- Show RSSI value
P1: 3  -- Show name and RSSI value

RSSI value total length is 14 Bytes.
RSSI value format "OK+RSSI:<P1>\r\n"
                  <P1>s: 4 Bytes RSSI value

                                                HMSoft
                                                2017.7

V547
=======================================================================
Add AT+DISA? command used to search devices and return full information
Possible return string format:
OK+DISS  --> Search start
<OK+DISA:><Device Address><Device Type><Device RSSI><Rest data length><Rest data>
OK+DISE  --> Search end
Note: <Rest data length> and <Rest data> is Hex format

                                                HMSoft
                                                2017.3
V546
=======================================================================
Add AT+128B<P1> command used to connect slave device who is using 128 bits UUID
P1: ?  ----- Query
P1: 0  ----- Doesnt use 128 Bits UUID, Default value.
P1: 1  ----- Use 128 Bit UUID
Please use HMConfigAssistant.exe to create AT Commands.

                                                HMSoft
                                                2016.12
V545
=======================================================================
Add AT+FFE2 command used to add a new Characteristic

Add AT+FFE2<P1> command used to add a new Characteristic.
P1: ?  ----- Query
P1: 0  ----- Close FFE2 Characteristic, Default value.
P1: 1  ----- Open FFE2 Characteristic

if AT+FFE20 is setup
Service and Characteristic list as follow:
  Service FFE0
  Characteristic FFE1 (if AT+RESP0 was setup, Properties is Read, WriteWithoutResponse, Notify)
  Characteristic FFE1 (if AT+RESP1 was setup, Properties is Read, Write, Notify)
if AT+FFE21 is setup
Service and Characteristic list as follow:
  Service FFE0
  Characteristic FFE1 (Properties is Read, Notify)
  Characteristic FFE2 (if AT+RESP0 was setup, Properties is WriteWithoutResponse)
  Characteristic FFE2 (if AT+RESP1 was setup, Properties is Write)

                                                HMSoft
                                                2016.12
V544
=======================================================================
Modify pari procedure
Perfect work with UWP
Device name length support 13

Add AT+TEMP[P1] command to modify Temp flag byte in advertising data.
P1: 0x00~0xFF
Add AT+HUMI[P1] command to modify Humi flag byte in advertising data.
P1: 0x00~0xFF
Change AT+BATT[P1] command to query battery or modify Batt flag byte in advertising data.
P1: ? Query battery information
P1: 0x00~0xFF(not include '?') used to modify Batt flag byte in advertising data.

                                                HMSoft
                                                2016.10

V543
=======================================================================
Add AT+SCAN[P1] command to adjust scan time in master role.
P1: ? for Query
P1: 1~9 uint seconds.
Default value : P1 = 3 Seconds
                                                HMSoft
                                                2016.10

V542
=======================================================================
Add self study function used to connect to other different BLE device

V540
=======================================================================
1. Join the compatibility code, better support different Android models


                                                HMSoft
                                                2015.05
                   HM-10/11 CC2540/1 V539
=======================================================================
1. Add AT+DISI? command, use to scan iBeacon device;

                                                HMSoft
                                                2015.03

                   HM-10/11 CC2540/1 V538
=======================================================================
1. Add AT+COMI command, config Minimum Link Layer connection interval
   para1 value: 0 ~ 9; Default: 3(20ms);
   0: 7.5ms; 1: 10ms; 2: 15ms; 3: 20ms; 4: 25ms; 5: 30ms; 6: 35ms; 7: 40ms; 8: 45ms; 9: 4000ms
2. Add AT+COMA command, config Maximum Link Layer connection interval
   para1 value: 0 ~ 9; Default: 7(40ms);
   0: 7.5ms; 1: 10ms; 2: 15ms; 3: 20ms; 4: 25ms; 5: 30ms; 6: 35ms; 7: 40ms; 8: 45ms; 9: 4000ms
3. Add AT+COLA command, config Link Layer connection slave latency
   para1 value: 0 ~ 4; Default: 0;
4. Add AT+COSU command, config Link Layer connection supervision timeout
   para1 value: 0 ~ 6; Default: 6(6000ms);
   0: 100ms; 1: 1000ms; 2: 2000ms; 3: 3000ms; 4: 4000ms; 5: 5000ms; 6: 6000ms;
5. Add AT+COUP command, switch slave role update connection parameter
   para1 value 0, 1; Default: 1(Update on);
   0: Update off; 1: Update on;




                                                HMSoft
                                                2015.02


                   HM-10/11 CC2540/1 V535
=======================================================================
1. Add AT+GAIN command, setup RX gain
   para1 value: 0~1
2. Remove result limit for AT+DISC? command.



                                                HMSoft
                                                2014.12


                   HM-10/11 CC2540/1 V534
=======================================================================
1. Add AT+NOTP command.
2. Fix some bugs.



                                                HMSoft
                                                2014.11

                   HM-10 CC2540/1 V532
=======================================================================
1. Add dualmode device support.
2. Fix some bugs.



                                                HMSoft
                                                2014.09

                   HM-10 CC2540/1 V531
=======================================================================
1. Add AT+FLAG[Para1] command
   para1 value: 00~FF
   Setup advertising data flag
2. Add AT+RELI[para1] command, set reliable advertising mode.
   para1: ?, 0, 1
3. Remove AT+FILT command
4. Modify AT+CON command to fit HM-12 dual mode
5. Modify AT+BEFC and AT+AFTC commands



                                                HMSoft
                                                2014.09

                   HM-10/11 CC2540/1 V528
=======================================================================
1. Fix some bugs
2. Add AT+BEFC command to setup PIO states on power on
3. Add AT+AFTC command to setup PIO states after connection establishment
4. Add MAC address into advertising pakcet



                                                HMSoft
                                                2014.05

                   HM-10 CC2540/1 V527
=======================================================================
1. Add AT+PCTL[para1] command
   para1 value: 0,1,?
   0: PIO pins output normal power
   1: PIO pins output high power



                                                HMSoft
                                                2014.04

                   HM-10 CC2540/1 V526
=======================================================================
1. Add AT+TEMP? command get ic temperature
2. Add ADC support, AT+ADC[para1]? para1 value: 3~B map to PIN3~PIN11


                                                HMSoft
                                                2014.04

                   HM-10 CC2540/1 V525
=======================================================================
1. Fix TX Power display bug.
2. Add PIO2 PWM function, AT+PIO2[para1], para1 value is: 0~9


                                                HMSoft
                                                2014.03

                   HM-10 CC2540/1 V524
=======================================================================
1. Add AT+ANCS command. (Query and set ANCS switch, Only HMSoft version has this function)
2. Modity AT+TYPE command. (Add Auth and BOND support)
3. Add AT+ERASE command. (Remove bond information)


                                                HMSoft
                                                2014.03
                   HM-10 CC2540/1 V523
=======================================================================
1. Add AT+TEMP command. (HMSensor version can use this command query DS18B20 temperature)
2. Add AT+ALLO command. (Query and set whitelist)
3. Add AT+ID[x][MAC] command. (Query and set whitelist MAC address)


                                                HMSensor
                                                2014.03
                   HM-10 CC2540/1 V522
=======================================================================
1. Modify Temperature, Humidity, Battery. Easy access to iOS.
2. Fixed change uuid bugs.


                                                HMSensor
                                                2014.02


                   HM-10 CC2540/1 V521
=======================================================================
1. Add AT+DISC command (Discovery device).
2. Add AT+SHOW command (Query/Set if show device name when use AT+DISC? command).
3. Add AT+CONN command (Use array index to connect device after AT+DISC? command).
4. Remove AT+BUSHU command.
5. Add AT+DELO command(Set iBeacon into work mode 1 or 2 until next power on).



                                                HMSensor
                                                2014.01



                   HM-10 CC2540/1 V520
=======================================================================
1. Add AT+IBE0 command (Query/Set iBeacon UUID).
2. Add AT+IBE1 command (Query/Set iBeacon UUID).
3. Add AT+IBE2 command (Query/Set iBeacon UUID).
4. Add AT+IBE3 command (Query/Set iBeacon UUID).
5. Add AT+BUSHU command (Set iBeacon into work mode until next power on).
6. Add AT+BEAC command (Query/Set Battery monitor).
7. Remove AT+IBE1 command (Query/Set iBeacon UUID).
8. Remove AT+IB2 command (Query/Set iBeacon UUID).
9. Open AT+FLOW command (Query/Set FLOW CONTROL).




                                                HMSensor
                                                2014.01

                   HM-10 CC2540/1 V519
=======================================================================
1. Add AT+ADTY command (Query/Set advertising type)
2. Add AT+IB1 command (Query/Set iBeacon UUID).
3. Add AT+IB2 command (Query/Set iBeacon UUID).
4. Add AT+MEAS command (Query/Set Measrued power).




                                                HMSensor
                                                2013.12

                   HM-10 CC2540/1 V518
=======================================================================
1. Add AT+ADVI command (Query/Set advertising interval)
2. Add AT+UUID command (Query/Set Service UUID).
3. Add AT+CHAR command (Query/Set characteristic UUID).




                                                HMSensor
                                                2013.12
                   HM-10 CC2540/1 V517
=======================================================================
1. Add AT+IBEA command (Open close iBeacon)
2. Add AT+MARJ command (Query/Set iBeacon marjor)
3. Add AT+MINO command (Query/Set iBeacon minor)



                                                HMSensor
                                                2013.12


                   HM-10 CC2540/1 V516
=======================================================================
1. Add AT+BIT7 command
2. Add iBeacon support
3. Add Bettery information






                                                HMSensor
                                                2013.11