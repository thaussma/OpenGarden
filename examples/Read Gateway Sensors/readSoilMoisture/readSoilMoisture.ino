/*  
 *  OpenGarden sensor platform for Arduino from Cooking-hacks.
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.2
 *  Design:            David Gascón 
 *  Implementation:    Victor Boria, Luis Martin & Jorge Casanova
 */

#include <OpenGarden.h>
#include <Wire.h> 


void setup() {
  Serial.begin(9600);
  OpenGarden.initSensors(); //Initialize sensors power 
}


void loop() {
  OpenGarden.sensorPowerON(); //Turns on the sensor power supply
  delay(500); // Time for initializing the sensor
  int soilMoisture = OpenGarden.readSoilMoisture(); //Read the sensor
  OpenGarden.sensorPowerOFF(); //Turns off the sensor power supply

  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);
  delay(2000);  //Wait 2 seconds

}

