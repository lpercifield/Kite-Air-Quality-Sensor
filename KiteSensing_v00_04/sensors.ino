
//the number of readings to be taken for averaging
//analog sensors, each time they are called
#define NUMREADINGS 25
//debounce time
#define DEBOUNCETIME 10


//---------- humidity ----------
int getHumidity(){
  int humidityVal = 0;
  for(int i=0; i<NUMREADINGS; i++){
    int tempRead = analogRead(3);
    humidityVal = humidityVal + tempRead;
    delay(DEBOUNCETIME);
  }
  humidityVal = humidityVal/NUMREADINGS;    
  return humidityVal;   
}

//---------- carbon dioxide ----------
int getCO2(){
  int CO2Val = 0;
  for(int i=0; i<NUMREADINGS; i++){
    int tempRead = analogRead(0);
    CO2Val = CO2Val + tempRead;
    delay(DEBOUNCETIME);
  }
  CO2Val = CO2Val/NUMREADINGS;
  CO2Val = 1000 - CO2Val; // sensor returns lower value the more CO2 is detected
  return CO2Val;    
}

//---------- air quality ----------
int getAirQual(){
  int AirQualVal = 0;
  for(int i=0; i<NUMREADINGS; i++){
    int tempRead = analogRead(1);
    AirQualVal = AirQualVal + tempRead;
    delay(DEBOUNCETIME);
  }
  AirQualVal = AirQualVal/NUMREADINGS;  
  return AirQualVal;    
}

//---------- carbon monoxide ----------
int getCO(){
  pinMode(4, OUTPUT);

  MQ7.CoPwrCycler();  

  if(MQ7.CurrentState() == LOW){   //we are at 1.4v, read sensor data!
    int CoData = 0;
    for(int i=0; i<NUMREADINGS; i++){
      int tempRead = analogRead(2);
      CoData = CoData + tempRead;
      delay(DEBOUNCETIME);
    }
    CoData = CoData/NUMREADINGS; 
    CoPrev = CoData;
    return CoData;
    //Serial.println(CoData);
  }
  else{
    //sensor is at 5v, heating time
    //during heating time we cannot read new data, return previous reading:
    return CoPrev;
  }      
}

