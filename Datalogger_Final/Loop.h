
void RTCloop()
{
  I2CMux.begin(Wire);             // TwoWire isntance can be passed here as 3rd argument
  I2CMux.openChannel(3);
  Serial.println();
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
    I2CMux.closeChannel(3);

}

void MPUloop()
{
  I2CMux.begin(Wire);             
  I2CMux.openChannel(0);
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
    I2CMux.closeChannel(0);

}

void ADSloop()
{ 

  I2CMux.begin(Wire);        
  I2CMux.openChannel(1);

  int16_t val_0 = ADS.readADC_SingleEnded(0);
  int16_t val_1 = ADS.readADC_SingleEnded(1);
  int16_t val_2 = ADS.readADC_SingleEnded(2);
  int16_t val_3 = ADS.readADC_SingleEnded(3);
  
  float f = ADS.computeVolts(1);   // voltage factor
  float val_00=(val_0 * f,3);
  float val_01=(val_1 * f,3);
  float val_02=(val_2 * f,3);
  float val_03=(val_3 * f,3);
  
  Serial.println("//////////////////ADS LOOP START////////////////");
  Serial.print("AIN0: "); Serial.print(val_0); Serial.print("  "); Serial.print(val_00); Serial.println("V");
  Serial.print("AIN1: "); Serial.print(val_1); Serial.print("  "); Serial.print(val_01); Serial.println("V");
  Serial.print("AIN2: "); Serial.print(val_2); Serial.print("  "); Serial.print(val_02); Serial.println("V");
  Serial.print("AIN3: "); Serial.print(val_3); Serial.print("  "); Serial.print(val_03); Serial.println("V");
  Serial.println("//////////////////ADS LOOP STOP////////////////");
  Serial.println();
  I2CMux.closeChannel(1);
}
  
void OLEDloop()
{
  I2CMux.begin(Wire);             
  I2CMux.openChannel(2);
  // clear the display
  display.clear();
  // draw the current demo method
  demos[demoMode]();
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();
  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) 
  {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(10);
  I2CMux.closeChannel(2);
}


void BTloop()
{
  I2CMux.openChannel(1);

  int16_t val_0 = ADS.readADC_SingleEnded(0);
  int16_t val_1 = ADS.readADC_SingleEnded(1);
  int16_t val_2 = ADS.readADC_SingleEnded(2);
  int16_t val_3 = ADS.readADC_SingleEnded(3);
  
  
  float f = ADS.computeVolts(1);   // voltage factor
 
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
   
if (deviceConnected) {
       counterr++;
    String MP =st4;
    String cmd = MP;
     Serial.print("MP");
    Serial.println(MP);
    
    cmd.toCharArray(msg, cmd.length()+1);
   
            pCharacteristic->setValue(msg);
       pCharacteristic->notify();
       Serial.print(msg);
        delay(100); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        counterr = 0;
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
