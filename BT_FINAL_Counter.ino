#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_Sensor.h>
#include "ADS1X15.h"
#include <Wire.h>
#include "TCA9548A.h"
TCA9548A I2CMux;

#define   DS3231_ADDRESS   0x68;
ADS1115 ADS(0x48);
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

char msg[2000] = "Initial Message";

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//Starts UUID with "c00fa" 
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

void setup() {
  Serial.begin(115200);
  
  // Create the BLE Device
  BLEDevice::init("ESP62");

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
  I2CMux.begin(Wire);             // TwoWire isntance can be passed here as 3rd argument
  I2CMux.closeAll();              // Set a base state which we know (also the default state on power on)
  I2CMux.openChannel(1);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);
  ADS.begin();
  I2CMux.closeChannel(1);
  /////ADS/////

}

int counter=0;

void loop() {
I2CMux.openChannel(1);
  ADS.setGain(0);

  int16_t val_0 = ADS.readADC(0);
  int16_t val_1 = ADS.readADC(1);
  int16_t val_2 = ADS.readADC(2);
  int16_t val_3 = ADS.readADC(3);
  
  
  float f = ADS.toVoltage(1);   // voltage factor
 
  float val_00=(val_0 * f);
  float val_01=(val_1 * f);
  float val_02=(val_2 * f);
  float val_03=(val_3 * f);
  
  String st0 = "a0:"+String(val_0)+"x"+String(val_00);
  String st1 = "a1:"+String(val_1)+"x"+String(val_01);
  String st2 = "a2:"+String(val_2)+"x"+String(val_02);
  String st3 = "a3:"+String(val_3)+"x"+String(val_03);
String st4 = String(val_00)+"x"+String(val_01)+"x"+String(val_02);
   I2CMux.closeChannel(1);
    // notify changed value
    if (deviceConnected) {
    counter++;
    String MP =st4;
    String cmd = MP;
    
    cmd.toCharArray(msg, cmd.length()+1);
   
        pCharacteristic->setValue(msg);
        pCharacteristic->notify();
        delay(1000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
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
    
}
