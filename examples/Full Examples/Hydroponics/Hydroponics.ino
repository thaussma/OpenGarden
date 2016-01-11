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

DateTime myTime;
int flag = 0; // auxiliar variable

#define calibration_point_4 2246  //Write here your measured value in mV of pH 4
#define calibration_point_7 2080  //Write here your measured value in mV of pH 7
#define calibration_point_10 1894 //Write here your measured value in mV of pH 10

#define point_1_cond 40000   // Write here your EC calibration value of the solution 1 in µS/cm
#define point_1_cal 40       // Write here your EC value measured in resistance with solution 1
#define point_2_cond 10500   // Write here your EC calibration value of the solution 2 in µS/cm
#define point_2_cal 120      // Write here your EC value measured in resistance with solution 2

//Power strip codes
unsigned long on1 = 1597145202;
unsigned long off1 = 1597145204;
unsigned long on2 = 1597145220;
unsigned long off2 = 1597145222;
unsigned long on3 = 1597145112;
unsigned long off3 = 1597145114;
unsigned long on4 = 1597144950;
unsigned long off4 = 1597144952;
unsigned long on5 = 1597144464;
unsigned long off5 = 1597144466;


void setup() {
  Serial.begin(9600);
  OpenGarden.initSensors(); //Initialize sensors power    
  OpenGarden.sensorPowerON();//Turn On the sensors
  delay(500);
  OpenGarden.initRTC();
  OpenGarden.initIrrigation(1); //Initialize irrigation 1
  OpenGarden.initIrrigation(2); //Initialize irrigation 2
  OpenGarden.initIrrigation(3); //Initialize irrigation 3
  OpenGarden.calibratepH(calibration_point_4,calibration_point_7,calibration_point_10);
  OpenGarden.calibrateEC(point_1_cond,point_1_cal,point_2_cond,point_2_cal);
  delay(500);
}

void loop() {
  myTime = OpenGarden.getTime();


  if ( myTime.second() == 0 && flag == 0  ){ //Only enter 1 time each minute

    //Read DHT22
    float airTemperature = OpenGarden.readAirTemperature(); 
    float airHumidity = OpenGarden.readAirHumidity(); 

    //Read DS18B20
    float waterTemperature = OpenGarden.readSoilTemperature();

    //Read LDR
    int luminosity = OpenGarden.readLuminosity(); 

    //Read the pH sensor
    int mvpH = OpenGarden.readpH(); //Value in mV of pH
    float pH = OpenGarden.pHConversion(mvpH); //Calculate pH value

    //Read the conductivity sensor in µS/cm
    float resistanceEC = OpenGarden.readResistanceEC(); //EC Value in resistance
    float EC = OpenGarden.ECConversion(resistanceEC); //EC Value in µS/cm



    //Turn On plug 1 where a grown light is connected from 10:00 to 20:00 hours
    if ( myTime.hour() == 10 && myTime.minute() == 0){
      OpenGarden.sendPowerStrip(on1); 
    }

    if ( myTime.hour() == 20 && myTime.minute() == 0){
      OpenGarden.sendPowerStrip(off1); 
    }



    //Serial Print all values
    Serial.println(F("***************************")); 
    OpenGarden.printTime(myTime);
    Serial.println("");
    Serial.print(F("Air Temperature: "));
    Serial.print(airTemperature);
    Serial.println("*C");
    Serial.print(F("Air Humidity: "));
    Serial.print(airHumidity);
    Serial.println(F("% RH"));
    Serial.print(F("Water Temperature: "));
    Serial.print(waterTemperature);
    Serial.println("*C");
    Serial.print(F("Luminosity: "));
    Serial.print(luminosity);
    Serial.println("%");
    Serial.print(F("pH Level: "));
    Serial.println(pH);
    Serial.print(F("Conductivity: "));
    Serial.print(EC);
    Serial.println(F("uS/cm"));

    flag = 1;
  }  

  else if (myTime.second() == 30 && flag == 1){
    flag = 0;
  }
}

