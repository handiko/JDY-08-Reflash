# JDY-08-Reflash
Replacing JDY-08 Bluetooth LE's firmware with the HM-10 v540 one. Firmware programming is using Arduino UNO as the programmer/downloader.

![alt text](https://github.com/handiko/JDY-08-Reflash/blob/master/Pics/JDY-08_photo.png)

## Requirements
* **JDY-08 BT Module** (TI CC2541)
* **Arduino UNO**
* **Arduino Sketch** for programming the CC2541 (**CCLoader.ino** In the Arduino-Sketch Folder, or from https://github.com/RedBearLab/CCLoader)
* **CCLoader.exe** Windows program as the programming software (Windows only). 

## Reflash the chip
* Program the Arduino UNO with the **CCLoader.ino** sketch.
* Connect the JDY-08 module into the Arduino. **BE CAUTIOUS, JDY-08 is a 3.3V device, Connect the VCC JDY-08 pin1 to the 3.3V pin on the Arduino**. I manage to be able to program the CC2541 at 5V level, but it is NOT RECOMMENDED. 
![alt text](https://github.com/handiko/JDY-08-Reflash/blob/master/Pics/JDY-08_firmware_programming.png)

