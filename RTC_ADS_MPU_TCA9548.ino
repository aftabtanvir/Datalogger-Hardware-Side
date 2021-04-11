////////////////////////////HEADER///////////////////////////////////////////////////
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "RTClib.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "TCA9548A.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "ADS1X15.h"

////////////////////////////HEADER///////////////////////////////////////////////////


////////////////////////////INT///////////////////////////////////////////////////
TCA9548A I2CMux;
RTC_DS3231 rtc;
Adafruit_MPU6050 mpu;
#define   DS3231_ADDRESS   0x68;
ADS1115 ADS(0x48);
////////////////////////////INT///////////////////////////////////////////////////


////////////////////////////RTCINT///////////////////////////////////////////////////
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long thisNowMillis;
uint32_t thisNow;



////////////////////////////RTCINT///////////////////////////////////////////////////


////////////////////////////BTINT///////////////////////////////////////////////////
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

char msg[2000] = "Initial Message";  

#define SERVICE_UUID        "c00fa1d8-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "c00fa3f4-36e1-4688-b7f5-ea07361b26a8"
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.print("Received from APP: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
      }
    }
};


//callback to receive events of connection and disconnection
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
 deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
 deviceConnected = false;
    }
};
////////////////////////////BTINT///////////////////////////////////////////////////


void setup () {
  
  unsigned long currentMillis = millis();
  Serial.begin(115200);
  /////BT/////
  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());
  
  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
  
  /////BT/////

  I2CMux.begin(Wire);             // TwoWire isntance can be passed here as 3rd argument
  I2CMux.closeAll();              // Set a base state which we know (also the default state on power on)


 
  


  
   ////ADS/////
  I2CMux.openChannel(1);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);
  ADS.begin();
  I2CMux.closeChannel(1);
  /////ADS/////
  
   /////RTC//////
  I2CMux.openChannel(0);
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC"); Serial.flush(); abort();
  }
  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));// January 21, 2014 at 3am you would call:// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
     rtc.adjust(DateTime(2021, 1, 21, 10, 06, 00));
  }
  I2CMux.closeChannel(0);
  //////RTC///////

  

    /////MPU6050/////
  I2CMux.openChannel(7);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }
  else
  {
    Serial.println("MPU6050 Found!");
    mpusetup();
  }
  I2CMux.closeChannel(7);

  ////MPU6050///////
}




  


int counter=0;

void loop () {
    // notify changed value
    ADS.setGain(0);

  int16_t val_0 = ADS.readADC(0);
  int16_t val_1 = ADS.readADC(1);
  int16_t val_2 = ADS.readADC(2);
  int16_t val_3 = ADS.readADC(3);
  
  
  float f = ADS.toVoltage(1);   // voltage factor
 
  float val_00=(val_0 * f,3);
  float val_01=(val_1 * f,3);
  float val_02=(val_2 * f,3);
  float val_03=(val_3 * f,3);
  
  String st0 = "a0:"+String(val_0)+String("x")+String(val_00);
  String st1 = "a1:"+String(val_1)+"x"+String(val_01);
  String st2 = "a2:"+String(val_2)+"x"+String(val_02);
  String st3 = "a3:"+String(val_3)+"x"+String(val_03);
 
    if (deviceConnected) {
       counter++;
    String MP =String(counter);
    String cmd = MP;
     Serial.print(MP);
    cmd.toCharArray(msg, cmd.length()+1);
   
            pCharacteristic->setValue(msg);
       pCharacteristic->notify();
       Serial.print(msg);
        delay(100); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        counter = 0;
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  unsigned long currentMillis = millis();
  I2CMux.openChannel(1);

  I2CMux.closeChannel(1);
  I2CMux.openChannel(0);
 // printtime();
  I2CMux.closeChannel(0);
  I2CMux.openChannel(7);
  //printmpu();
  I2CMux.closeChannel(7);
delay(1000);
}








void printtime()
{

  Serial.println("//////////////////RTC START////////////////");
  DateTime now = rtc.now(); uint32_t now1 = (now.unixtime()); if (now1 != thisNow) {
    thisNow = now1;
    thisNowMillis = millis();
  }
  Serial.print(now.year(), DEC); Serial.print('/'); Serial.print(now.month(), DEC); Serial.print('/'); Serial.print(now.day(), DEC); Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); Serial.print(") "); Serial.print(now.hour(), DEC); Serial.print(':'); Serial.print(now.minute(), DEC);
  Serial.print(':'); Serial.print(now.second(), DEC); Serial.print(":"); Serial.print(millis() - thisNowMillis); Serial.println(); Serial.print("Temperature: ");
  Serial.print(rtc.getTemperature()); Serial.println(" C");
  Serial.println("//////////////////RTC STOP////////////////");
  Serial.println("");
}




void mpusetup()
{
  
  Serial.println("//////////////////MPU SETUP START////////////////");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange())
  {
    case MPU6050_RANGE_2_G: Serial.println("+-2G"); break;
    case MPU6050_RANGE_4_G: Serial.println("+-4G"); break;
    case MPU6050_RANGE_8_G: Serial.println("+-8G"); break;
    case MPU6050_RANGE_16_G: Serial.println("+-16G"); break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange())
  {
    case MPU6050_RANGE_250_DEG: Serial.println("+- 250 deg/s"); break;
    case MPU6050_RANGE_500_DEG: Serial.println("+- 500 deg/s"); break;
    case MPU6050_RANGE_1000_DEG: Serial.println("+- 1000 deg/s"); break;
    case MPU6050_RANGE_2000_DEG: Serial.println("+- 2000 deg/s"); break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth())
  {
    case MPU6050_BAND_260_HZ: Serial.println("260 Hz"); break;
    case MPU6050_BAND_184_HZ: Serial.println("184 Hz"); break;
    case MPU6050_BAND_94_HZ: Serial.println("94 Hz"); break;
    case MPU6050_BAND_44_HZ: Serial.println("44 Hz"); break;
    case MPU6050_BAND_21_HZ: Serial.println("21 Hz"); break;
    case MPU6050_BAND_10_HZ: Serial.println("10 Hz"); break;
    case MPU6050_BAND_5_HZ: Serial.println("5 Hz"); break;
  }
  Serial.println("//////////////////MPU SETUP STOP////////////////");
  Serial.println("");
}

void printmpu()
{
  Serial.println("//////////////////MPU LOOP START////////////////");
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("MPU Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("//////////////////MPU LOOP STOP////////////////");
  Serial.println("");
}
