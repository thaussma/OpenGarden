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

#include "OpenGarden.h" 
#include "Wire.h" 
#include "DHT.h" 
#include "OneWire.h" 
#include "RF12.h" 
#include "Ports.h" 
#include "RTClib.h"
#include "DallasTemperature.h"
#include "RemoteReceiver.h"
#include "RemoteSwitch.h"
#include "filter.h"

RTC_DS1307 RTC;
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(soilTemperaturePin); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature. 


OpenGardenClass::OpenGardenClass() 
{
}

//Sensor initializing functions:
void OpenGardenClass::initSensors(void){
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(ecPowerPin, OUTPUT);
  digitalWrite(sensorPowerPin, LOW); 
  digitalWrite(ecPowerPin, LOW);
  sensors.begin();
  dht.begin();
}


void OpenGardenClass::sensorPowerON(void){
  digitalWrite(ecPowerPin, LOW);
  delay(100);
  digitalWrite(sensorPowerPin, HIGH);

}

void OpenGardenClass::sensorPowerOFF(void){
  digitalWrite(ecPowerPin, LOW);
  digitalWrite(sensorPowerPin, LOW);

}


//Transceiver functions:
void OpenGardenClass::initRF(void){
  rf12_initialize(myNodeID,RF12_433MHZ,100); // (GatewayNodeID,freq,network)
  rf12_recvDone();
}

void OpenGardenClass::receiveFromNode(void){

  if (rf12_recvDone() && rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
    nodeID = rf12_hdr & 0x1F;  // get node ID
    airPacket = *(Payload*) rf12_data;


    if (RF12_WANTS_ACK) {           // Send ACK if requested
      rf12_sendStart(RF12_ACK_REPLY, 0, 0);
    }
  }
  //Receive data and store the last value in buffer
  if (nodeID == 1) 
    nodesBuffer[0] = airPacket; //Node:1->Buffer:0
  else if (nodeID == 2) 
    nodesBuffer[1] = airPacket; //Node:2->Buffer:1
  else if (nodeID == 3) 
    nodesBuffer[2] = airPacket; //Node:3->Buffer:2
  else if (nodeID == 4) 
    nodesBuffer[3] = airPacket; //Node:4->Buffer:3
  else if (nodeID == 5) 
    nodesBuffer[4] = airPacket; //Node:5->Buffer:4
  else if (nodeID == 6) 
    nodesBuffer[5] = airPacket; //Node:6->Buffer:5
  else if (nodeID == 7) 
    nodesBuffer[6] = airPacket; //Node:7->Buffer:6
}

Payload OpenGardenClass::getNodeData(uint8_t node){	
  if (node > 10)
    return nodesBuffer[0];
  else 
    return nodesBuffer[node];
}

void OpenGardenClass::printNode(Payload nodePacket){

  if (nodePacket.temperature == 0 && nodePacket.humidity == 0 && nodePacket.moisture == 0 && nodePacket.light == 0 && nodePacket.supplyV == 0){
    Serial.println("No node connected");
  }
  else{
    Serial.print("Temperature:");
    Serial.print(nodePacket.temperature);
    Serial.println("*C");
    Serial.print("Humidity:");
    Serial.print(nodePacket.humidity);
    Serial.println("%RH");
    Serial.print("Soil Moisture:");
    Serial.println(nodePacket.moisture);
    Serial.print("Luminosity:");
    Serial.print(nodePacket.light);
    Serial.println("%");
    Serial.print("Battery Voltage:");
    Serial.print(nodePacket.supplyV);
    Serial.println("mV");
  }
}

void OpenGardenClass::debugRF(void){

  if (rf12_recvDone() && rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
    nodeID = rf12_hdr & 0x1F;  // get node ID
    airPacket = *(Payload*) rf12_data;

    Serial.print("Received packet from node ");
    Serial.println(nodeID);
    Serial.print("Temperature:");
    Serial.print(airPacket.temperature);
    Serial.println("*C");
    Serial.print("Humidity:");
    Serial.print(airPacket.humidity);
    Serial.println("%RH");
    Serial.print("Soil Moisture:");
    Serial.println(airPacket.moisture);
    Serial.print("Luminosity:");
    Serial.println(airPacket.light);
    Serial.print("Battery Voltage:");
    Serial.print(airPacket.supplyV);
    Serial.println("mV");
    Serial.println("***************");

    if (RF12_WANTS_ACK) {           // Send ACK if requested
      rf12_sendStart(RF12_ACK_REPLY, 0, 0);
    }
  }
}


//RTC functions:
void OpenGardenClass::initRTC(void){
  Wire.begin();
  RTC.begin();
}

void OpenGardenClass::setTime(void){
  RTC.adjust(DateTime(__DATE__, __TIME__));
}

DateTime OpenGardenClass::getTime(void){
  return  RTC.now(); 
}

void OpenGardenClass::printTime(DateTime now){

  Serial.print(now.month(), DEC);
  Serial.print("/");

  Serial.print(now.day(), DEC);
  Serial.print("/");

  Serial.print(now.year(), DEC);
  Serial.print(" ");

  switch (now.dayOfWeek()) // Friendly printout the weekday
  {
  case 1:
    Serial.print("MON");
    break;
  case 2:
    Serial.print("TUE");
    break;
  case 3:
    Serial.print("WED");
    break;
  case 4:
    Serial.print("THU");
    break;
  case 5:
    Serial.print("FRI");
    break;
  case 6:
    Serial.print("SAT");
    break;
  case 7:
    Serial.print("SUN");
    break;
  }
  Serial.print(" ");

  if(now.hour()<10){
    switch (now.hour()) 
    {
    case 0:
      Serial.print("00");
      break;
    case 1:
      Serial.print("01");
      break;
    case 2:
      Serial.print("02");
      break;
    case 3:
      Serial.print("03");
      break;
    case 4:
      Serial.print("04");
      break;
    case 5:
      Serial.print("05");
      break;
    case 6:
      Serial.print("06");
      break;
    case 7:
      Serial.print("07");
      break;
    case 8:
      Serial.print("08");
      break;
    case 9:
      Serial.print("09");
      break;	 
    }
  }
  else{
    Serial.print(now.hour(), DEC);
  }

  Serial.print(":");

  if(now.minute()<10){
    switch (now.minute()) 
    {
    case 0:
      Serial.print("00");
      break;
    case 1:
      Serial.print("01");
      break;
    case 2:
      Serial.print("02");
      break;
    case 3:
      Serial.print("03");
      break;
    case 4:
      Serial.print("04");
      break;
    case 5:
      Serial.print("05");
      break;
    case 6:
      Serial.print("06");
      break;
    case 7:
      Serial.print("07");
      break;
    case 8:
      Serial.print("08");
      break;
    case 9:
      Serial.print("09");
      break;

    }
  }
  else{
    Serial.print(now.minute(), DEC);
  }

  Serial.print(":");

  if(now.second()<10){
    switch (now.second()) 
    {
    case 0:
      Serial.print("00");
      break;
    case 1:
      Serial.print("01");
      break;
    case 2:
      Serial.print("02");
      break;
    case 3:
      Serial.print("03");
      break;
    case 4:
      Serial.print("04");
      break;
    case 5:
      Serial.print("05");
      break;
    case 6:
      Serial.print("06");
      break;
    case 7:
      Serial.print("07");
      break;
    case 8:
      Serial.print("08");
      break;
    case 9:
      Serial.print("09");
      break;

    }
  }
  else{
    Serial.print(now.second(), DEC);
  }

}


//Irrigation functions:
void OpenGardenClass::initIrrigation(int out) {
  if (out == 1) 
  {
    pinMode(irrigation1, OUTPUT);
    digitalWrite(irrigation1, LOW);
  } 
  if (out == 2) 
  {
    pinMode(irrigation2, OUTPUT);
    digitalWrite(irrigation2, LOW);
  }
  if (out == 3) 
  {
    pinMode(irrigation3, OUTPUT);
    digitalWrite(irrigation3, LOW);
  }

}

void OpenGardenClass::irrigationON(int out) {
  if (out == 1) 
  {
    digitalWrite(irrigation1, HIGH);
  } 
  if (out == 2) 
  {
    digitalWrite(irrigation2, HIGH);
  }
  if (out == 3) 
  {
    digitalWrite(irrigation3, HIGH);
  }

}

void OpenGardenClass::irrigationOFF(int out) {
  if (out == 1) 
  {
    digitalWrite(irrigation1, LOW);
  } 
  if (out == 2) 
  {
    digitalWrite(irrigation2, LOW);
  }
  if (out == 3) 
  {
    digitalWrite(irrigation3, LOW);
  }

}


//Power strip functions:
void OpenGardenClass::sendPowerStrip(unsigned long plug){
  RemoteSwitch::sendTelegram(plug,3);
}


//Sensor reading functions	
int OpenGardenClass::readSoilMoisture() {
  return analogRead(soilMoisturePin);
}

float OpenGardenClass::readAirTemperature() {
  return dht.readTemperature(); 
}

float OpenGardenClass::readAirHumidity() {
  return dht.readHumidity(); 
}

float OpenGardenClass::readSoilTemperature() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  return sensors.getTempCByIndex(0);
}

int OpenGardenClass::readLuminosity() {
  float luminosity = analogRead(luminosityPin);
  return ((luminosity/1023)*100);
}

void OpenGardenClass::calibratepH(int _calibration_point_4,int _calibration_point_7,int _calibration_point_10){
  calibration_point_10 = _calibration_point_10;
  calibration_point_7 = _calibration_point_7;
  calibration_point_4 = _calibration_point_4;
}


int OpenGardenClass::readpH(){

  float value_array[FILTER_SAMPLES];

  // Take some measurements to filter the signal noise and glitches
  for(int i = 0; i < FILTER_SAMPLES; i++)
  {
    //Read from the ADC channel selected
    value_array[i] = analogRead(pHLevelPin);
  }

  delay(100);

  pH = myFilter.median(value_array,FILTER_SAMPLES);

  return (pH*5000.0)/1024.0; 
}


float OpenGardenClass::pHConversion(int _mvpH) {
  float sensitivity;
  // Two ranges calibration
  if (_mvpH > calibration_point_7 ) {
    // The sensitivity is calculated using the other two calibration values
    // Asumme that the pH sensor is lineal in the range.
    // sensitivity = pHVariation / volts
    // Divide by 3 = (pH) 7 - (pH) 4
    sensitivity = (calibration_point_4-calibration_point_7)/3;

    // The value at pH 7.0 is taken as reference
    // => Units balance => 7 (pH) + (volts) / ((pH) / (volts))
    return 7.0 + (calibration_point_7-_mvpH) / sensitivity;
    // | |
    // (pH 7 voltage - Measured volts) = Variation from the reference
  } 
  else {
    // The sensitivity is calculated using the other two calibration values
    sensitivity = (calibration_point_7-calibration_point_10) / 3;

    return 7.0 + (calibration_point_7-_mvpH)/sensitivity;
  }

}


void OpenGardenClass::calibrateEC(float _point_1_cond,float _point_1_cal,float _point_2_cond,float _point_2_cal){
  point_1_cond = _point_1_cond;
  point_1_cal = _point_1_cal;
  point_2_cond = _point_2_cond;
  point_2_cal = _point_2_cal;
}


float OpenGardenClass::ECConversion(unsigned long _resistanceEC) {
  float value;
  float SW_condK;
  float SW_condOffset;

  // Calculates the cell factor of the conductivity sensor and the offset from the calibration values
  SW_condK = point_1_cond * point_2_cond * ((point_1_cal-point_2_cal)/(point_2_cond-point_1_cond));
  SW_condOffset = (point_1_cond * point_1_cal-point_2_cond*point_2_cal)/(point_2_cond-point_1_cond);

  // Converts the resistance of the sensor into a conductivity value
  value = SW_condK * 1 / (_resistanceEC+SW_condOffset);

  return value;
}



float OpenGardenClass::readResistanceEC(void) {
  //Check if sensorPowerPin is HIGH or LOW because
  //sensorPowerPin and ecPowerPin NEVER must be connected at the same time
  if(digitalRead(sensorPowerPin)==HIGH){
    digitalWrite(sensorPowerPin, LOW);
    delay(100);
    digitalWrite(ecPowerPin, HIGH);
    delay(100);
    float ecmeasure = calculateResistanceEC();
    digitalWrite(ecPowerPin, LOW);
    digitalWrite(sensorPowerPin, HIGH);
    return ecmeasure;
  }


  if(digitalRead(sensorPowerPin)==LOW){
    digitalWrite(ecPowerPin, HIGH);
    delay(100);
    float ecmeasure = calculateResistanceEC();
    digitalWrite(ecPowerPin, LOW);
    digitalWrite(sensorPowerPin, LOW);
    return ecmeasure;
  }

}

float OpenGardenClass::calculateResistanceEC(void) {

  float value_array[FILTER_SAMPLES];
  // Take some measurements to filter the signal noise and glitches
  for(int i = 0; i < FILTER_SAMPLES; i++)
  {
    //Read from the ADC channel selected
    value_array[i] = analogRead(ecLevelPin);
    
  }

  delay(100);

  EC = myFilter.median(value_array,FILTER_SAMPLES);


  if(EC <= 11){
    resistance = EC/0.050;
  }
  else if((EC > 11) && (EC <= 23)){
    resistance = ((EC-0.44)/0.0480);
  }
  else if((EC > 23) && (EC <= 47)){
    resistance = ((EC-1.7170)/0.0453);
  }
  else if((EC > 47) && (EC <= 94)){
    resistance = ((EC-7.8333)/0.0392);
  }
  else if((EC > 94) && (EC <= 162)){
    resistance = ((EC-34.16)/0.0272);
  }
  else if((EC > 162) && (EC <= 186)){
    resistance = ((EC-36.667)/0.0267);
  }
  else if((EC > 186) && (EC <= 259)){
    resistance = ((EC-93.0909)/0.0161);
  }
  else if((EC > 259) && (EC <= 310)){
    resistance = ((EC-157.0)/0.0102);
  }
  else if((EC > 310) && (EC <= 358)){
    resistance = ((EC-207.1429)/0.0069);
  }
  else if((EC > 358) && (EC <= 401)){
    resistance = ((EC-272.0)/0.0039);
  }
  else if((EC > 401) && (EC <= 432)){
    resistance = ((EC-327.9286)/0.0022);
  }
  else if((EC > 432) && (EC <= 458)){
    resistance = ((EC-373.8095)/0.0012);
  }
  else if((EC > 458) && (EC <= 479)){
    resistance = ((EC-413.3750)/0.0007);
  }
  else if((EC > 479) && (EC <= 525)){
    resistance = ((EC-473.8889)/0.0001);
  }
  else if((EC > 525)){
    resistance = ((EC-525.7778)/0.00001);
  }

  return resistance;
}

OpenGardenClass OpenGarden;
