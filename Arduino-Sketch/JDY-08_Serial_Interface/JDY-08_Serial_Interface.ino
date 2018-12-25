//  SerialIn_SerialOut_HM-10_01
//
//  Uses hardware serial to talk to the host computer and AltSoftSerial for communication with the bluetooth module
//
//  What ever is entered in the serial monitor is sent to the connected device
//  Anything received from the connected device is copied to the serial monitor
//  Does not send line endings to the HM-10
//
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D8 (SS RX) - BT TX no need voltage divider 
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
//
 
#include <AltSoftSerial.h>

AltSoftSerial BTserial; 
 
char c=' ';
boolean NL = true;
const int baud=9600;
 
void setup() 
{
    Serial.begin(baud);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    BTserial.begin(baud);  
    Serial.print("BTserial started at ");
    Serial.println(baud);
}
 
void loop()
{
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);
    }
 
 
    if (Serial.available())
    {
        c = Serial.read();
 
        if (c!=10 & c!=13 ) 
        {  
             BTserial.write(c);
        }
 
        if (NL) { Serial.print("\r\n>");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
}
