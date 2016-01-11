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

#define point_1_cond 40000   // Write here your EC calibration value of the solution 1 in µS/cm
#define point_1_cal 40       // Write here your EC value measured in resistance with solution 1
#define point_2_cond 10500   // Write here your EC calibration value of the solution 2 in µS/cm
#define point_2_cal 120      // Write here your EC value measured in resistance with solution 2


void setup() {
  Serial.begin(9600);
  OpenGarden.initSensors();   //Initialize sensors power 
  OpenGarden.sensorPowerON(); //Turn on sensors power supply
  OpenGarden.calibrateEC(point_1_cond,point_1_cal,point_2_cond,point_2_cal);
  delay(500);
}

void loop() {

  float resistanceEC = OpenGarden.readResistanceEC(); //EC Value in resistance
  Serial.print(F("EC Value in resistance = "));
  Serial.print(resistanceEC);  

  Serial.print(F(" // EC Value = "));
  float EC = OpenGarden.ECConversion(resistanceEC); //EC Value in µS/cm
  Serial.print(EC);
  Serial.println(F(" uS/cm"));



  delay(2000);   //Wait 2 seconds
}


