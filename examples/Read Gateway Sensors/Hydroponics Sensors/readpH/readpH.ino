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

#define calibration_point_4 2246  //Write here your measured value in mV of pH 4
#define calibration_point_7 2080  //Write here your measured value in mV of pH 7
#define calibration_point_10 1894 //Write here your measured value in mV of pH 10

void setup() {
  Serial.begin(9600);
  OpenGarden.initSensors();   //Initialize sensors power 
  OpenGarden.sensorPowerON(); //Turn on sensors power supply
  OpenGarden.calibratepH(calibration_point_4,calibration_point_7,calibration_point_10);
  delay(1000);
}

void loop() {

  int mvpH = OpenGarden.readpH(); //Value in mV of pH
  Serial.print(F("pH Value in mV = "));
  Serial.print(mvpH);

  Serial.print(F(" // pH = "));
  float pH = OpenGarden.pHConversion(mvpH); //Calculate pH value
  Serial.println(pH);

  delay(2000);  //Wait two seconds
}



