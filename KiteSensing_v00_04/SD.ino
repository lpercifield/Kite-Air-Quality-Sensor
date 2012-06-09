/*Optional SD Card Use Uncomment functions to save data back up onto SD 
  referenced from: http://arduino.cc/en/Tutorial/Datalogger
  * SD card attached to SPI bus as follows:
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
  ** CS - pin 4
  
*/
/*  
#define chipSelect 4

File airData;
boolean file = false;

void setupSD(){
  
  Serial.println("Setting up SD Card");
  
  if(!SD.begin(chipSelect)){
    Serial.println("SD init Failed");
    return;
  }
  
  if(SD.exists("air_data.txt")){
    Serial.println("air_data.txt exists");
    file = true;
  }else{
    Serial.println("air_data.txt does not exist");
    Serial.println("Creating air_data.txt");
    airData = SD.open("air_data.txt", FILE_WRITE);
    airData.close();
  }
  if(file!=true){
    if(SD.exists("air_data.txt")) Serial.println("air_data.txt exists");
    else Serial.println("air_data.txt still doesn't exist");
  }
  
 }
 
void writeSD(String dataString){
  
 
    airData = SD.open("air_data.txt", FILE_WRITE);
  
    // if the file is available, write to it:
    if (airData) {
      airData.println(dataString);
      airData.close();
      // print to the serial port too:
      Serial.print("Data Written to SD: \t");
      Serial.println(dataString);
    }  
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }  
  
}
*/
