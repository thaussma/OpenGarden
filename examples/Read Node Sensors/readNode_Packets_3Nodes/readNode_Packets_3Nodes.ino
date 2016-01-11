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

void setup () {
  Serial.begin(9600);
  OpenGarden.initRF();
  OpenGarden.initRTC();
}

void loop() {

  OpenGarden.receiveFromNode(); //Receive data from node
  myTime = OpenGarden.getTime();

  Payload node1Packet = OpenGarden.getNodeData(node1); //Get the last data received
  Payload node2Packet = OpenGarden.getNodeData(node2);
  Payload node3Packet = OpenGarden.getNodeData(node3);

  if ( myTime.second() == 0 && flag == 0  ){ //Only enter 1 time each minute

    OpenGarden.printTime(myTime); 

    Serial.println("Node 1:");
    OpenGarden.printNode(node1Packet);  //Serial Print data
    Serial.println("************"); 
    Serial.println("Node 2:");
    OpenGarden.printNode(node2Packet);  //Serial Print data
    Serial.println("************"); 
    Serial.println("Node 3:");
    OpenGarden.printNode(node3Packet);  //Serial Print data
    Serial.println("**************************"); 

    flag = 1;
  }  

  else if (myTime.second() == 30 && flag == 1){
    flag = 0;
  }


}






