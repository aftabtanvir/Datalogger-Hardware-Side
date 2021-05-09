# ESP32 Data Logger

Portable Analog Sensor logging device built with ESP32, TCA9548A,ADS1115,SSD1306 and Micro SD Card Module.

## Hardware Schematic & Pin Connection
 
 
| ESP32  | SD Card Module |
| :---: | --- |
| Pin 5  | CS   |
| Pin 18 | SCK  |
| Pin 23 | MOSI |
| Pin 19 | MISO |
| 5V     | VCC  |
| GND    | GND  |

| ESP32 | TCA9548A |
| :---: | --- |
| 3V3    | VCC   |
| Pin 21 | SDA   |
| Pin 22 | SCL   |
| GND    | GND   |

| TCA9548A | Pin Config |
| :---: | --- |
| Vin | 5V Adapter       |
| GND | GND Adapter      |
| SD1 | SDA ADS1115      |
| SC1 | SCL ADS1115      |  
| SD2 | SDA SSD1306      |
| SC2 | SCL SSD1306      |
| SD3 | SDA  DS3231      |
| SC3 | SCL  DS3231      |


## BLE Instruction Codes

| Hex Code | Function | Description |
| :---: | --- | --- |
| 0x00 | None                             | Code Not Assigned |
| 0x01 | Get device status                | Get current ADS1115 device statue code (see GPS Status Flags) |
| 0x02 | Toggle ADS1115 All Ch  on        | Turn ADS1115 All Channel Sensing on |
| 0x03 | Toggle ADS1115 All Ch  off       | Turn ADS1115 All Channel Sensing off |
| 0x04 | Toggle ADS1115 Ch#00 on           | Turn ADS1115 Channel # 0 Sensing on |
| 0x05 | Toggle ADS1115 Ch#00 off          | Turn ADS1115 Channel # 0 Sensing off |
| 0x06 | Toggle ADS1115 Ch#01 on           | Turn ADS1115 Channel # 1 Sensing on |
| 0x07 | Toggle ADS1115 Ch#01 off          | Turn ADS1115 Channel # 1 Sensing off |
| 0x08 | Toggle ADS1115 Ch#02 on           | Turn ADS1115 Channel # 2 Sensing on |
| 0x09 | Toggle ADS1115 Ch#02 off          | Turn ADS1115 Channel # 2 Sensing off |
| 0x0A | Toggle ADS1115 Ch#03 on           | Turn ADS1115 Channel # 3 Sensing on |
| 0x0B | Toggle ADS1115 Ch#03 off          | Turn ADS1115 Channel # 3 Sensing off |
| 0x0C | Toggle ADS1115 All Ch logging on | Turn ADS1115 All Channel sentences logging on |
| 0x0D | Toggle ADS1115 All Ch logging off| Turn ADS1115 All Channel sentences logging off |
| 0x0E | Toggle ADS1115 Ch#00 logging on   | Turn ADS1115 Channel # 0 sentences logging on |
| 0x0F | Toggle ADS1115 Ch#00 logging off  | Turn ADS1115 Channel # 0 sentences logging off |
| 0x10 | Toggle ADS1115 Ch#01 logging on   | Turn ADS1115 Channel # 1 sentences logging on |
| 0x11 | Toggle ADS1115 Ch#01 logging off  | Turn ADS1115 Channel # 1 sentences logging off |
| 0x12 | Toggle ADS1115 Ch#02 logging on   | Turn ADS1115 Channel # 2 sentences logging on |
| 0x13 | Toggle ADS1115 Ch#02 logging off  | Turn ADS1115 Channel # 2 sentences logging off |
| 0x14 | Toggle ADS1115 Ch#03 logging on   | Turn ADS1115 Channel # 3 sentences logging on |
| 0x15 | Toggle ADS1115 Ch#03 logging off  | Turn ADS1115 Channel # 3 sentences logging off |
| 0x16 | Get ADS1115 All Ch data          | Get current ADS1115 All Channel data if ADS1115 All Channel has fix |
| 0x17 | Get ADS1115 Ch#00 data            | Get current ADS1115 Channel # 0 data if ADS1115 Channel 0 has fix |
| 0x18 | Get ADS1115 Ch#01 data            | Get current ADS1115 Channel # 1 data if ADS1115 Channel 0 has fix |
| 0x19 | Get ADS1115 Ch#02 data            | Get current ADS1115 Channel # 2 data if ADS1115 Channel 0 has fix |
| 0x1A | Get ADS1115 Ch#03 data            | Get current ADS1115 Channel # 3 data if ADS1115 Channel 0 has fix |
| 0x1B | List Files                       | List all current log files on SD card |
| 0x1C | Read File                        | Read log file from SD card |
| 0x1D | Get SD Card Status               | Get SD card usage information |
| 0x1E | Reboot                           | Reboot Datalogger device |
| 0x1F | Reset                            | Reset Datalogger device configurations and all status flags |

## GPS Status Flags

#### 4 Bit GPS Status Flag System

| Bit Index | Function | Description |
| :---: | --- | --- |
| 0 | BLE Connection    | True if BLE service is connected to BLE client |
| 1 | WIFI Connection   | True if WIFI service is connected to WIFI client |
| 2 | ADS1115 Ch#00 On/Off Status | True if ADS1115 Ch#00 is enabled on device |
| 3 | ADS1115 Ch#01 On/Off Status | True if ADS1115 Ch#01 is enabled on device |
| 4 | ADS1115 Ch#02 On/Off Status | True if ADS1115 Ch#02 is enabled on device |
| 5 | ADS1115 Ch#03 On/Off Status | True if ADS1115 Ch#03 is enabled on device |
| 6 | ADS1115 Ch#00 Has Fix       | True if ADS1115 Ch#00 has analog value fix |
| 7 | ADS1115 Ch#01 Has Fix       | True if ADS1115 Ch#01 has analog value fix |
| 8 | ADS1115 Ch#02 Has Fix       | True if ADS1115 Ch#02 has analog value fix |
| 9 | ADS1115 Ch#03 Has Fix       | True if ADS1115 Ch#03 has analog value fix |
| 10 | ADS1115 Ch#00 Logging Status| True if ADS1115 Ch#00 sentences logging is enabled |
| 11 | ADS1115 Ch#01 Logging Status| True if ADS1115 Ch#01 sentences logging is enabled |
| 12 | ADS1115 Ch#02 Logging Status| True if ADS1115 Ch#02 sentences logging is enabled |
| 13 | ADS1115 Ch#03 Logging Status| True if ADS1115 Ch#03 sentences logging is enabled |
## Resource Links

#### ADS1115 Module
- [Adafruit ADS1115 Library](https://github.com/adafruit/Adafruit_ADS1X15)
- [Adafruit ADS1115 Product Page](https://www.adafruit.com/product/1085)

#### ESP32
- [ESP32 DataSheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [ESP32 Wiki](http://arduinoinfo.mywikis.net/wiki/Esp32)

#### SD Card Module
- [ESP32 Logging to MicroSD Card](https://randomnerdtutorials.com/esp32-data-logging-temperature-to-microsd-card/)
- [Interfacing Micro SD Card Module](https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/)
- [Arduino SD Library](https://www.arduino.cc/en/reference/SD)

#### Bluetooth Low Energy
- [ESP32 BLE Guide](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/)
- [Characteristic with Multiple Descriptors](https://github.com/espressif/arduino-esp32/issues/1038)
- [Indication & Notification](https://community.nxp.com/docs/DOC-328525)
- [BLE introduction: Notify or Indicate ](https://www.onethesis.com/2015/11/21/ble-introduction-notify-or-indicate/)
- [UUID Generator](https://www.uuidgenerator.net/)
- [ASCII TO HEX](https://www.asciitohex.com/)

