/*
Writing Moisture Sensor Data to an SD card with Relay control and Wifi.

For pin attachments see Connection List.xls file.
*/
//
#include "SD.h"
#include"SPI.h"
//
//the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD
// library functions will not work.

  const int CSpin = 10;
  const int sensor_pin = 2;	/* Soil moisture sensor O/P pin */
  String dataString =""; // holds the data to be written to the SD card
  float sensorReading1 = 0.00; // value read from your first sensor
  File sensorData;
//
  #define CH3 7   // Connect Digital Pin 7 (Pin 13 on Huzzah) on Arduino to CS on the SD Card Module.
//

void setup() {
// MS Section
  Serial.begin(9600);	/* Define baud rate for serial communication */
  
//Setup all the RELAY Pins
   pinMode(CH3, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT); // Blink lights
   
//Turn OFF any power to the Relay channels
   digitalWrite(CH3,LOW);
   delay(2000); //Wait 2 seconds before starting sequence
//
// // SD Card Section
//
// Open serial communications
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(CSpin, OUTPUT);
//
// see if the card is present and can be initialized:
  if (!SD.begin(CSpin)) {
  Serial.println("Card failed, or not present");
// don't do anything more:
  return;
}
  Serial.println("card initialized.");

// End Setup
//
}
void loop() {
//
//Blink Lights
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

//
// MS Section
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  sensorReading1 = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
//
//
// void loop(){    TAKE THIS OUT AND MAKE THIS SECTION ONE??
// build the data string
  dataString = String(sensorReading1); // convert to CSV
//
  saveData(); // save to SD card
  delay(60000); // delay before next write to SD Card, adjust as required
}
//
  void saveData(){
  if(SD.exists("data1.csv")){ // check the card is still there
// now append new data file
  sensorData = SD.open("data1.csv", FILE_WRITE);
  if (sensorData){
  sensorData.println(dataString);
  sensorData.close(); // close the file
}
}
  else{
Serial.println("Error writing to file !");
}
// RELAY Section  - Switches valve
 	// If..then..else statement required here.
 	if (sensorReading1 < 40)
 	{digitalWrite(CH3, HIGH);  //Relay 3 switches to ON  
   delay(60000);
   }
   if (sensorReading1 > 50)
   {digitalWrite(CH3,LOW);    //Relay 3 switches to OFF
   delay(60000);
   }
}
//
