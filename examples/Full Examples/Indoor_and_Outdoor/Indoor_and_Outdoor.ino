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
  OpenGarden.initRF();
  OpenGarden.initRTC();
  OpenGarden.initIrrigation(1); //Initialize irrigation 1
  OpenGarden.initIrrigation(2); //Initialize irrigation 2
  OpenGarden.initIrrigation(3); //Initialize irrigation 3
}

void loop() {
  myTime = OpenGarden.getTime();
  OpenGarden.receiveFromNode();


  if ( myTime.second() == 0 && flag == 0  ){ //Only enter 1 time each minute

    //Get Gateway Sensors
    int soilMoisture0 = OpenGarden.readSoilMoisture();
    float airTemperature0 = OpenGarden.readAirTemperature();
    float airHumidity0 = OpenGarden.readAirHumidity();
    float soilTemperature0 = OpenGarden.readSoilTemperature();
    int luminosity0 = OpenGarden.readLuminosity();

    //Get Node Sensors
    Payload node1Packet = OpenGarden.getNodeData(node1); 
    Payload node2Packet = OpenGarden.getNodeData(node2);
    Payload node3Packet = OpenGarden.getNodeData(node3); 


    int soilMoisture1 = node1Packet.moisture;
    float airTemperature1 = node1Packet.temperature;
    float airHumidity1 = node1Packet.humidity;
    int luminosity1 = node1Packet.light;
    int battery1 = node1Packet.supplyV;

    int soilMoisture2 = node2Packet.moisture;
    float airTemperature2 = node2Packet.temperature;
    float airHumidity2 = node2Packet.humidity;
    int luminosity2 = node2Packet.light;
    int battery2 = node2Packet.supplyV;

    int soilMoisture3 = node3Packet.moisture;
    float airTemperature3 = node3Packet.temperature;
    float airHumidity3 = node3Packet.humidity;
    int luminosity3 = node3Packet.light;
    int battery3 = node3Packet.supplyV;

    //Turn On Irrigation 1 if gateway soil moisture falls under value: 350
    if(soilMoisture0 < 350 ){
      OpenGarden.irrigationON(1); 
    }
    else{
      OpenGarden.irrigationOFF(1);
    }

    //Turn On Irrigation 2 if node 1 or node 2 soil moisture falls under value: 300
    if( soilMoisture1<300 || soilMoisture2<300){
      OpenGarden.irrigationON(2);
    }
    else{
      OpenGarden.irrigationOFF(2);
    }

    if( soilMoisture3<300 ){
      OpenGarden.irrigationON(3);
    }
    else{
      OpenGarden.irrigationOFF(3);
    }

    //Turn On plug 1 where a lamp light is connected, if node 1 or node 2 
    //or node 3 luminosity falls under 30%
    if( luminosity1<30 || luminosity2<30 || luminosity3<30 ){
      OpenGarden.sendPowerStrip(on1); //Turn ON plug 1, 
    }
    else{
      OpenGarden.sendPowerStrip(off1);
    }


    //Serial Print all values
    Serial.println("***************************"); 
    OpenGarden.printTime(myTime);  
    Serial.println("*********"); 
    Serial.println("Node 1:");
    OpenGarden.printNode(node1Packet);  
    Serial.println("*********"); 
    Serial.println("Node 2:");
    OpenGarden.printNode(node2Packet); 
    Serial.println("*********"); 
    Serial.println("Node 3:");
    OpenGarden.printNode(node3Packet); 
    Serial.println("*********"); 

    Serial.println("Gateway:");
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoisture0);
    Serial.print("Air Temperature: ");
    Serial.print(airTemperature0);
    Serial.println("*C");
    Serial.print("Air Humidity: ");
    Serial.print(airHumidity0);
    Serial.println("% RH");
    Serial.print("Soil Temperature: ");
    Serial.print(soilTemperature0);
    Serial.println("*C");
    Serial.print("Luminosity: ");
    Serial.print(luminosity0);
    Serial.println("%");


    flag = 1;
  }  

  else if (myTime.second() == 30 && flag == 1){
    flag = 0;
  }
}


