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

Payload node1Packet;

void setup () {
  Serial.begin(9600);
  OpenGarden.initRF();
}

void loop() {

  //Receive data from node
  OpenGarden.receiveFromNode();

  //Extract the last data received from nodes in a packet
  node1Packet = OpenGarden.getNodeData(node1); 

  int soilMoisture1 = node1Packet.moisture;
  float airTemperature1 = node1Packet.temperature;
  float airHumidity1 = node1Packet.humidity;
  int luminosity1 = node1Packet.light;
  int battery1 = node1Packet.supplyV;

  Serial.println("Node 1:");
  Serial.println("**************");
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture1);
  Serial.print("Air Temperature: ");
  Serial.print(airTemperature1);
  Serial.println("*C");
  Serial.print("Air Humidity: ");
  Serial.print(airHumidity1);
  Serial.println("% RH");
  Serial.print("Luminosity: ");
  Serial.print(luminosity1);
  Serial.println("%");
  Serial.print("Baterry: ");
  Serial.print(battery1);
  Serial.println("mV");

  delay(5000);
}


