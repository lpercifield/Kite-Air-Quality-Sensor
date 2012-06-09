/*
   ***_-_-_ Air Quality Kite Sensing v00_04 _-_-_***
 
 Circuit:
 * RESET pin connected to: pin 7
 
 Sensors:
 * humidity on                     pin A3 - +5v - GND
 * MQ-7 Carbon Monoxide sensor on  pin A2 - +5v - GND - D14
 * MG-811 Carbon Dioxide sensor on pin A0 - +9v - GND
 * MQ-135 Air Quality sensor on    pin A1 - +5v - GND
 * DS18B20 (Dallas 1-wire          pin D5 - +5v 680k pull up - GND 
 
 c.piuggi & l.percifield
 june 2012
 
 modified from
 
 joseph saavedra
 http://jos.ph
 january 2011
 weather tunnel sensors
 
 
 */
 
#include <SoftwareSerial.h>
//#include <SD.h> //uncomment to enable SD Card storage functions
#include <CS_MQ7.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//One Wire Temp Sensor
#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 12
#define NUM_SENSORS 5 //set this to the number of sensors you will be using

//enter the string names as found on pachube
String sensorTitle[NUM_SENSORS] = { "Temp", "Humidity", "CO2", "CO", "VOC"};
float sensorData[NUM_SENSORS];


OneWire onewire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&onewire);

DeviceAddress temp = { 0x28, 0x10, 0x6D, 0xDC, 0x3, 0x00,0x00, 0x4F};

CS_MQ7 MQ7(3, 2); //Citizen Sensor MQ-7 Carbon Monoxide Breakout instance (toggle,indicator)int CoPrev = 0; //for storing CO Data
int CoPrev = 0;

SoftwareSerial GPRS_Serial(7, 8);
boolean config = false;
boolean power = false;
void setup() {
  
  GPRS_Serial.begin(19200);
  Serial.begin(19200);
  
  Serial.println("start setup");
  tempSensor.begin();
  tempSensor.setResolution(temp, TEMPERATURE_PRECISION);
  tempSensor.requestTemperatures();
  float celcius = tempSensor.getTempC(temp);
  Serial.println(celcius);
  
  configModem();
  
  Serial.println("end setup");
  delay(1000);
  
}

void loop() {
  Serial.println("Reading Sensors...");

  // read the sensors:
  Serial.print("sensor 1 - temperature..."); //temperature
  tempSensor.requestTemperatures();
  sensorData[0] = tempSensor.getTempC(temp);
  Serial.println( sensorData[0] );

  Serial.print("sensor 2 - humidity..."); //humidity
  sensorData[1] = getHumidity();
  Serial.println(sensorData[1]);

  Serial.print("sensor 3 - CO2..."); //CO2
  sensorData[2] = getCO2();
  Serial.println(sensorData[2]);

  Serial.print("sensor 4 - CO..."); //CO
  sensorData[3] = getCO();
  Serial.println(sensorData[3]);
  
  Serial.print("sensor 5 - Air Qual..."); //Air Quality
  sensorData[4] = getAirQual();
  Serial.println(sensorData[4]);
  
  int length= 56; //set a length for the CSV string you will send to Pachube
  
  Serial.print("String Length: ");
  Serial.println(length);
  
  String sLength = String(length);
  
  if(!config) configModem();
  sendToPachube(sLength, sensorData, sensorTitle);
  
  Serial.println("... end of loop ...");
  delay(10000);
  
}


