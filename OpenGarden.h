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

// Ensure this library description is only included once
#ifndef OpenGarden_included
#define	OpenGarden_included

#include <Arduino.h>
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

#define pHLevelPin A1
#define ecLevelPin A0
#define soilTemperaturePin 5
#define soilMoisturePin A2

#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6
#define SUN 7

#define DHTPIN 4
#define luminosityPin 3
#define sensorPowerPin 8
#define ecPowerPin 9
#define myNodeID 30 //Gateway nodeID

#define node1 0	//Node:1->Buffer:0
#define node2 1	//Node:2->Buffer:1
#define node3 2	//Node:3->Buffer:2
#define node4 3	//Node:4->Buffer:3
#define node5 4 //Node:5->Buffer:4
#define node6 5 //Node:6->Buffer:5
#define node7 6 //Node:6->Buffer:5

#define irrigation1 6
#define irrigation2 7
#define irrigation3 9

#define RF12_NORMAL_SENDWAIT 0

#define FILTER_SAMPLES   7

typedef struct MyStruct {
  float humidity;	
  float temperature;	
  int moisture;        
  int light;
  int supplyV;	      
} 
Payload;
typedef MyStruct Payload; 


// Library interface description
class OpenGardenClass  {

public:

  //***************************************************************
  // Constructor of the class									*
  //***************************************************************

  //! Class constructor.
  OpenGardenClass();

  //***************************************************************
  // Public Methods & Variables									*
  //***************************************************************

  //Sensor initializing functions
  void initSensors(void);    //Initializing necessary for all the sensors
  void sensorPowerON(void);  //Turns on the sensor power supply
  void sensorPowerOFF(void); //Turns off the sensor power supply


  //Transceiver functions
  void initRF(void);                //Initializing transceiver
  void receiveFromNode(void);       //Receive data from node and store it in a internal buffer
  Payload getNodeData(uint8_t node);  //Get a struct with the last data stored in the internal buffer
  void printNode (Payload nodePacket);//Serial print all the node data obtained with getData(node)
  void debugRF(void);               //Serial print node data as it comes to transceiver without buffering it


  //RTC functions
  void initRTC(void);	      //Initializing necessary for the RTC
  void setTime(void); 	      //Get the computer's time and date
  DateTime getTime(void);		  //Get a struct with current time and date
  void printTime(DateTime now); //Serial print the struct obtained with getTime

  //Irrigation functions
  void initIrrigation(int out); //Initializing necessary for irrigation with the number “out”
  void irrigationON(int out);   //Turns on the irrigation with the number “out”
  void irrigationOFF(int out);  //Turns off the irrigation with the number “out”

  //Power Strip function
  void sendPowerStrip(unsigned long plug); //Send a signal to turn on or off a plug


  //Sensor reading functions	
  int readSoilMoisture(void);     //Returns the value of the soil moisture sensor
  float readAirTemperature(void); //Returns the temperature value of the DHT22 sensor
  float readAirHumidity(void);    //Returns the humidity value of the DHT22 sensor
  float readSoilTemperature(void);//Returns the value of the DS18B20  sensor
  int readLuminosity(void);       //Returns the value of the LDR sensor

  void calibratepH(int _calibration_point_4,int _calibration_point_7,int _calibration_point_10); //Calibrate the pH sensor
  int readpH(); //Return pH value in mV
  float pHConversion(int _mvpH); //Returns the value of the pH sensor

  void calibrateEC(float _point_1_cond,float _point_1_cal,float _point_2_cond,float _point_2_cal); //Calibrate the EC sensor
  float readResistanceEC(); //Returns EC Value in resistance
  float ECConversion(unsigned long _resistanceEC); //Returns EC Value in µS/cm

  int nodeID;        //Identification of nodes


private:

  //***************************************************************
  // Private Methods & Variables							                  	*
  //***************************************************************
  float calculateResistanceEC(void);

  Payload airPacket;     //Struct used to transmit info through RF	
  int EC;
  int pH;
  float resistance;
  Payload nodesBuffer[6]; //Buffer for nodes

  float calibration_point_10;
  float calibration_point_7;
  float calibration_point_4;

  float point_1_cond;
  float point_1_cal;
  float point_2_cond;
  float point_2_cal;
}; 

extern OpenGardenClass OpenGarden;

#endif

