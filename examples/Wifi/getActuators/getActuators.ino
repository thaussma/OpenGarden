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
#include "Wire.h" 

//Enter here your data
const char server[] = "YOUR_SERVER_IP";
const char server_port[] = "YOUR_SERVER_PORT";
const char wifi_ssid[] = "YOUR_WIFI_SSID";
const char wifi_password[] = "YOUR_WIFI_PASSWORD";

char recv[512];
int cont;

char irrigation1_wf;
char irrigation2_wf;
char irrigation3_wf;

void setup() {
  Serial.begin(115200);
  OpenGarden.initIrrigation(1); //Initialize irrigation number 1 
  OpenGarden.initIrrigation(2); //Initialize irrigation number 2
  OpenGarden.initIrrigation(3); //Initialize irrigation number 3
  OpenGarden.irrigationOFF(1);
  OpenGarden.irrigationOFF(2);
  OpenGarden.irrigationOFF(3);
  cleanVector();
  wificonfig(); 
}


void loop() {
  getActuators();

  Serial.println();
  if (irrigation1_wf == '0'){
    OpenGarden.irrigationOFF(1); //Turn OFF the irrigation number 1 
    Serial.println("Irrigation 1 OFF");
  }
  else if (irrigation1_wf == '1'){
    OpenGarden.irrigationON(1); //Turn ON the irrigation number 1 
    Serial.println("Irrigation 1 ON");
  }
  else{
    Serial.println("Irrigation 1 BAD DATA");
  }


  if (irrigation2_wf == '0'){
    OpenGarden.irrigationOFF(2); //Turn OFF the irrigation number 2 
    Serial.println("Irrigation 2 OFF");
  }
  else if (irrigation2_wf == '1'){
    OpenGarden.irrigationON(2); //Turn ON the irrigation number 2
    Serial.println("Irrigation 2 ON");  
  }
  else{
    Serial.println("Irrigation 2 BAD DATA");
  }


  if (irrigation3_wf == '0'){
    OpenGarden.irrigationOFF(3); //Turn OFF the irrigation number 3
    Serial.println("Irrigation 3 OFF");
  }
  else if (irrigation3_wf == '1'){
    OpenGarden.irrigationON(3); //Turn ON the irrigation number 3
    Serial.println("Irrigation 3 ON");
  }
  else{
    Serial.println("Irrigation 3 BAD DATA");
  }

  cleanVector();
  enterCMD();

  delay(5000);
}


//*********************************************************************
//*********************************************************************


void cleanVector(){
  recv[0] = 0; 
  recv[1] = 0;
  recv[2] = 0;
}


void wificonfig() {
  while (Serial.available() > 0) {
  }

  enterCMD();
  // Sets DHCP and TCP protocol
  Serial.print(F("set ip dhcp 1\r"));
  check();
  Serial.print(F("set ip protocol 18\r"));
  check();

  // Configures the way to join the network AP, sets the encryption of the
  // network and joins it
  Serial.print(F("set wlan join 0\r")); //The auto-join feature is disabled
  check();
  Serial.print(F("set wlan phrase "));
  Serial.print(wifi_password);
  Serial.print(F("\r"));
  check();
  Serial.print(F("join "));
  Serial.print(wifi_ssid);
  Serial.print(F("\r"));
  check();
}

void getActuators(){
  
  //Configures HTTP connection
  Serial.print(F("set dns name "));
  Serial.print(server);
  Serial.print(F("\r"));
  check();
   
  Serial.print("set ip adress 0\r"); 
  check();

  Serial.print(F("set i r "));
  Serial.print(server_port);
  Serial.print(F("\r"));
  
  check();
  Serial.print("set com remote 0\r"); 
  check();
  Serial.print("open\r"); 
  check();
  Serial.flush(); 
  Serial.print("GET /get_actuators.php?actuators\r"); 
  checkData();


}

void enterCMD() {
  Serial.println("");
  // Enters in command mode
  Serial.print(F("$$$"));
  delay(100);
  check();
  Serial.flush();
}


void check(){
  delay(1500);
  autoflush(); 
}


void checkData(){
  cont=0;   
  delay(3000);
  while (Serial.available()>0)
  {
    recv[cont]=Serial.read(); 
    delay(10);
    cont++;
  }
  recv[cont]='\0';

  irrigation1_wf= recv[0]; 
  irrigation2_wf= recv[1];
  irrigation3_wf= recv[2];

  //Serial.print("Received:");
  //Serial.print(recv);

  autoflush(); 
  delay(100);
}


void autoflush()
{
  while (Serial.available()>0)
  {
    Serial.read(); 
  }
}

