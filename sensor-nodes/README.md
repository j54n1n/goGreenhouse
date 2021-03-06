# Sensor Nodes
Here you can find the code related to the various sensor nodes.

## Requirements
* The node shall consume less power as possible since it could be placed on a
remote or difficult to supply place.
* The wireless radio shall transmit a data packet for each type of sensor that
is connected to the node.
* The data packets shall be transmitted repeatedly for a not yet defined time
interval with updated sensor values.

## Development Platform
The development is based upon the *Moteino* board installed with a *RFM69*
wireless radio transmitting on the 868MHz license free ISM band.
Further libraries are included from http://jeelabs.org for power saving and the
wireless radio driver.

### Notes
* The development is compatible with the Arduino IDE 1.6.7+ from
https://www.arduino.cc
* To install the board definitions of *Moteino* add following link to your comma
separated *Additional Board Manger URLs* list in the IDE settings:
https://lowpowerlab.github.io/MoteinoCore/package_LowPowerLab_index.json  
* Use this folder as your workspace in your Arduino IDE settings. The libraries
folder will be automatically recognized and included in the IDE.
* If the Arduino IDE complaints that it can not find a library then it is very
likely that you forgot to pull in the *git submodule* of the library. Execute
either `git submodule init` and `git submodule update` or do make a recursive
clone of the repository via `git clone --recursive <repo-url>`.

## Hardware Pinout
* The physical hardware is based upon the *Moteino R4* with the *RFM69HW* radio
and added *W25X40CL* 512kB Flash memory chip. See also
[Moteino R4 Schematic](https://github.com/LowPowerLab/Moteino/blob/master/R4/MoteinoR4_schematic.pdf).
![Moteino R4 Pinout](https://farm4.staticflickr.com/3818/10585364014_df2e1604bc_o.png "Moteino R4 Pinout")
* The Moteino can be modified to support *JeeLib*'s *JeePort*s 1 to 4 by using
a simple adapter board. For example:
![Proto Board](http://jeelabs.net/attachments/download/486/jlpcb-037.png "Proto Board")
* Thus support is given for up to 4 *Plug* boards that can
contain specific sensors and actuators. See also
[JeeNode v6 Schematic](http://jeelabs.net/attachments/download/331/jlpcb-128.pdf).
![JeeNode v6 Pinout](http://jeelabs.org/wp-content/uploads/2013/03/qrc-jn-v6-part.png "JeeNode v6 Pinout")

| JeePort | Pin | Name | Moteino   | Description                     |
|---------|:---:|:----:|:---------:|---------------------------------|
|         |     |      | D0/RX     | Digital IO/Serial UART Receive  |
|         |     |      | D1/TX     | Digital IO/Serial UART Transmit |
|         |     |      | D2(INT0)  | Digital IO/RFM69 Interrupt      |
|         |     |      | D8        | Digital IO/Flash Chip Select    |
|         |     |      | D9(PWM)   | Digital IO/PWM/Built In LED     |
|         |     |      | D10       | Digital IO/RFM69 Chip Select    |
|         |     |      | D11(MOSI) | Digital IO/SPI Bus Data In      |
|         |     |      | D12(MISO) | Digital IO/SPI Bus Data Out     |
|         |     |      | D13(SCK)  | Digital IO/SPI Bus Serial Clock |
|         |     |      | A4(SDA)   | Digital/Analog IO/I2C Data      |
|         |     |      | A5(SCL)   | Digital/Analog IO/I2C Clock     |
|         |     |      | A6        | Analog Input                    |
|         |     |      | A7        | Analog Input                    |
|         |     |      | AR        | Analog Reference Voltage        |
| Port 1  | 1   | PWR  | PWR(VIN)  | External Power Supply           |
|         | 2   | DIO1 | D4        | Digital IO                      |
|         | 3   | GND  | GND       | Power Supply Ground             |
|         | 4   | 3V3  | 3V3       | Regulated 3.3V Power Supply     |
|         | 5   | AIO1 | A0(D14)   | Digital/Analog IO               |
|         | 6   | IRQ  | D3(INT1)  | Digital IO/JeePlug Interrupt    |
| Port 2  | 1   | PWR  | PWR(VIN)  | External Power Supply           |
|         | 2   | DIO2 | D5(PWM)   | Digital IO/PWM Timer 0          |
|         | 3   | GND  | GND       | Power Supply Ground             |
|         | 4   | 3V3  | 3V3       | Regulated 3.3V Power Supply     |
|         | 5   | AIO2 | A1(D15)   | Digital/Analog IO               |
|         | 6   | IRQ  | D3(INT1)  | Digital IO/JeePlug Interrupt    |
| Port 3  | 1   | PWR  | PWR(VIN)  | External Power Supply           |
|         | 2   | DIO3 | D6(PWM)   | Digital IO/PWM Timer 0          |
|         | 3   | GND  | GND       | Power Supply Ground             |
|         | 4   | 3V3  | 3V3       | Regulated 3.3V Power Supply     |
|         | 5   | AIO3 | A2(D16)   | Digital/Analog IO               |
|         | 6   | IRQ  | D3(INT1)  | Digital IO/JeePlug Interrupt    |
| Port 4  | 1   | PWR  | PWR(VIN)  | External Power Supply           |
|         | 2   | DIO4 | D7        | Digital IO                      |
|         | 3   | GND  | GND       | Power Supply Ground             |
|         | 4   | 3V3  | 3V3       | Regulated 3.3V Power Supply     |
|         | 5   | AIO4 | A3(D17)   | Digital/Analog IO               |
|         | 6   | IRQ  | D3(INT1)  | Digital IO/JeePlug Interrupt    |

* The *RFM69HW* radio can be also connected to the Raspberry PI GPIO header by
using the SPI pins *3V3*, *MOSI*, *MISO*, *SCLK*, *CE0*, and *GND*. Beware that
in transmit mode the module can overload the 3.3V power supply pin, ie.
**do not** use the *high power* settings above *+13dBm* without any external
voltage regulator supplied by the 5V pin. See also
[RPi Low-level peripherals](http://elinux.org/RPi_Low-level_peripherals#Power_pins)
and [RFM69HW Datasheet](http://www.hoperf.com/upload/rf/RFM69HW-V1.3.pdf).
![RFM69HW Pinout](https://lowpowerlab.com/forum/index.php?action=dlattach;topic=239.0;attach=102;image "RFM69 Pinout")
![Raspberry Pi Pinout](http://www.raspberrypi-spy.co.uk/wp-content/uploads/2012/06/Raspberry-Pi-GPIO-Layout-Model-B-Plus-rotated-2700x900-1024x341.png)

#### Further reading
* http://lowpowerlab.com/moteino
* http://jeelabs.org/2011/06/27/time-controlled-transmissions
* http://jeelabs.org/2011/12/13/developing-a-low-power-sketch
* [http://jeelabs.org/2010/12/03/what-is-“power”](http://jeelabs.org/2010/12/03/what-is-%E2%80%9Cpower%E2%80%9D)
* [http://jeelabs.org/2010/12/04/what-is-“power”-–-part-2](http://jeelabs.org/2010/12/04/what-is-%E2%80%9Cpower%E2%80%9D-%E2%80%93-part-2)
* http://jeelabs.org/2012/11/26/watts-amps-coulombs
* http://jeelabs.org/2013/09/05/decoding-bit-fields
* http://jeelabs.org/2013/09/06/decoding-bit-fields-part-2
* http://jeelabs.org/2013/10/05/flashback-anatomy-of-a-transmission
* http://jeelabs.org/book/1522c
* http://jeelabs.net/projects/hardware/wiki