#include "Initialization.h"
#include "Setup.h"
#include "Loop.h"

void setup () {
  Serial.begin(115200);
  I2CMux.begin(Wire);
  unsigned long currentMillis = millis();
  BTSetup();
  OLEDsetup();
  ADSsetup();
  RTCsetup();
  MPUsetup();
  I2CMux.closeAll(); 
  }


void loop () {
   unsigned long currentMillis = millis();
   OLEDloop();
   I2CMux.begin(Wire);
   I2CMux.closeAll(); 
   BTloop();
   ADSloop();
   RTCloop();
   MPUloop();
   delay(1000);
   I2CMux.closeAll(); 
    }
