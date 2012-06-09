/* GPRS INFO
   Seeed Studio GPRS SHIELD using Software Serial 
   http://www.seeedstudio.com/depot/gprs-shield-p-779.html

*/
String feedId = "YOUR FEED ID";
String apiKey = "YOUR API KEY";

int retries=0;

void configModem(){
  
   int reps = 0;
   config_start:
   Serial.println("ConfigAttempt");
   reps++;
   if(reps>3){
     powerOff();
   }
   if(!power){
    powerOn();
  }
  
  Serial.println("Executing AT Commands for one time power on configuration");
  GPRS_Serial.flush();
  GPRS_Serial.println("ATE0"); //Command echo off
  Serial.println("ATE0   Sent");
  
  if(GPRS_Listener(4,10)==false) goto config_start;
  
  GPRS_Serial.println("AT+CIPMUX=0"); //We only want a single IP Connection at a time.
  Serial.println("AT+CIPMUX=0   Sent");
  
  if(GPRS_Listener(4,10)==false) goto config_start;

  GPRS_Serial.println("AT+CIPMODE=0"); //Selecting "Normal Mode" and NOT "Transparent Mode" as the TCP/IP Application Mode
  Serial.println("AT+CIPMODE=0    Sent!");
  
  if(GPRS_Listener(4,10)==false) goto config_start;
 
  GPRS_Serial.println("AT+CGDCONT=1,\"IP\""); //Defining your packet data here
  Serial.println("AT+CGDCONT=1,\"IP\" Sent!");
  
  if(GPRS_Listener(4,10)==false) goto config_start;
  
  GPRS_Serial.println("AT+CSTT=\"CMNET\" "); //Start Task and set Access Point Name (and username and password if any) http://www.flexispy.com/Mobile%20APN%20Setting%20to%20use%20GPRS.htm
  Serial.println("AT+CSTT=\"wap.cingular\"");
  
  if(GPRS_Listener(4,10)==false) goto config_start;
  
  GPRS_Serial.println("AT+CIPSHUT"); //Close any GPRS Connection if open
  Serial.println("AT+CIPSHUT  Sent!");
  
  if(GPRS_Listener(7,10)==false) goto config_start; 

 }


/////////////////////////////////////////
////*SEND DATA THROUGH GPRS TO COSM *////
/////////////////////////////////////////

int sendToPachube(String length, float data[NUM_SENSORS], String header[NUM_SENSORS]){
  retries=0;
  data_start:
  if(retries>0){
    configModem();
  }
  Serial.println("LOOP START");
  GPRS_Serial.flush();
  retries++;
  if(retries<3){
    GPRS_Serial.println("AT+CIPSTART=\"TCP\",\"api.pachube.com\",\"80\""); //Open a connection to Pachube.com
    Serial.println("AT+CIPSTART=\"TCP\",\"api.pachube.com\",\"80\"  Sent!");
    
    if(GPRS_Listener(12,255)==false) goto data_start;
    
    GPRS_Serial.flush(); 
    GPRS_Serial.println("AT+CIPSEND"); //Start data through TCP connection
    Serial.println("AT+CIPSEND  Sent!");
    
    if(GPRS_Listener(1,100) == false) goto data_start;
    GPRS_Serial.flush();

    //Emulate HTTP and use PUT command to upload temperature datapoint using Comma Seperate Value Method
    //For more info see https://cosm.com/docs/v2/feed/update.html
    
    GPRS_Serial.print("PUT /v2/feeds/"+feedId+".csv HTTP/1.1\r\n");
    Serial.println("PUT /v2/feeds/"+feedId+".csv HTTP/1.1  Sent!");
    delay(300);

    GPRS_Serial.print("Host: api.pachube.com\r\n"); 
    Serial.println("Host: api.pachube.com  Sent!");
    delay(300);

    GPRS_Serial.print("X-PachubeApiKey: "+apiKey+"\r\n"); //MAKE SURE YOU SET YOUR API KEY!
    Serial.println("X-PachubeApiKey: "+apiKey+"  Sent!"); 
    delay(300);

    GPRS_Serial.print("Content-Length: "+length+"\r\n"); 
    Serial.print("Content-Length: "+length+"  Sent!"); 
    delay(300);

    GPRS_Serial.print("Connection: close\r\n\r\n"); 
    Serial.print("Connection: close  Sent!"); 
    delay(300);
    
    for(int i = 0; i < NUM_SENSORS; i++){
     GPRS_Serial.print(header[i]);
     GPRS_Serial.print(",");
     delay(300);
     GPRS_Serial.print(data[i]);
     delay(300);
     GPRS_Serial.print("\r\n");
     delay(300);
    }
    GPRS_Serial.print("\r\n"); 
    delay(300);
    GPRS_Serial.write(0x1A);
    delay(300); //Send End Of Line Character to send all the data and close connection
    
    if(GPRS_Listener(20,255) == false) goto data_start;     

    GPRS_Serial.flush();
    GPRS_Serial.println("AT+CIPSHUT"); //Close the GPRS Connection
    Serial.println("AT+CIPSHUT  Sent!");
    
    if(GPRS_Listener(4,100) == false) goto data_start;
    else config = false; 
    
  }else {
    config = false;
  }
}

boolean GPRS_Listener(char bytes, int timeout ){
 
  if(GPRS_Serial_wait_for_bytes(bytes,timeout) == 0)
  {  
    Serial.println("Timeout");
    return false;
  }
  else
  {
    Serial.print("Received:");
    while(GPRS_Serial.available()!=0)
    {
      char in = (char)GPRS_Serial.read();
      Serial.print(in);
    }
  Serial.print("\n");
  return true;
  } 
  
}


/////////////////////////////////////////
/////////* GET BYTES FROM GPRS*//////////
/////////////////////////////////////////

char GPRS_Serial_wait_for_bytes(char no_of_bytes, int timeout)
{
  //Serial.println("starting countdown"); //uncomment for debugging
  while(GPRS_Serial.available() < no_of_bytes)
  {
    delay(200);
    timeout-=1;
    //Serial.println(timeout); //uncomment for debuggin
    if(timeout == 0)
    {
      return 0;
    }
  }
  return 1;
}

/////////////////////////////////////////
///////* Power Functions for GPRS*///////
/////////////////////////////////////////
void powerOn(){
  Serial.println("Powering On...");
  delay(1000);
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2500);
  digitalWrite(9,LOW);
  delay(15500);
  power = true;
}
/////////////////////////////////////////
void powerOff(){
  delay(1000);
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2500);
  digitalWrite(9,LOW);
  power = false;
  //delay(10500);
}



