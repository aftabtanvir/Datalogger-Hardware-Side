//////RTC///////
void RTCsetup()
{ 
  I2CMux.begin(Wire);             // TwoWire isntance can be passed here as 3rd argument
  I2CMux.openChannel(3);
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC"); Serial.flush(); abort();
  }
  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));// January 21, 2014 at 3am you would call:// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
     rtc.adjust(DateTime(2021, 1, 21, 10, 06, 00));
  }
  I2CMux.closeChannel(3);
}
//////RTC///////



/////MPU6050/////
void MPUsetup()
{
  I2CMux.begin(Wire);             // TwoWire isntance can be passed here as 3rd argument
  I2CMux.openChannel(0);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }
  else
  {
    Serial.println("MPU6050 Found!");
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
 I2CMux.closeChannel(0);
}
////MPU6050///////
/////ADS/////
void ADSsetup()
{
  I2CMux.begin(Wire);             // TwoWire isntance can be passed here as 3rd argument
  I2CMux.openChannel(1);
  ADS.begin();
  I2CMux.closeChannel(1);
}
/////ADS/////

/////OLED/////

void OLEDsetup()
{
    I2CMux.begin(Wire);             
  I2CMux.openChannel(2);
  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  I2CMux.closeChannel(2);
}
/////OLED/////

////BT///////
void BTSetup()
{
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
};
/////BT/////
