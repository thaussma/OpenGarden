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

void setup() {
  Serial.begin(9600);
  OpenGarden.initIrrigation(1); //Initialize irrigation number 1 (insert 1,2 or 3)
}

void loop() {
  OpenGarden.irrigationON(1); //Turn ON the irrigation number 1 (insert 1,2 or 3)
  Serial.println("Irrigation 1: ON");
  delay (5000); //Wait five seconds

  OpenGarden.irrigationOFF(1); //Turn OFF the irrigation number 1 (insert 1,2 or 3)
  Serial.println("Irrigation 1: OFF");
  delay (5000); //Wait five seconds
}


