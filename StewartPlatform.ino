/**
 * stewart-platform
 * 
 * Authors: Leroy Sibanda, Michael Rosa
 * Date: August 2018
 * 
 * Program to test and calibrate an Stewart platform based on an Arduino Mega and motor drivers
 */

#include "configuration.h"
#include "Actuator.h"
#include "Platform.h"
#include "eeprom.h"

#define PERIOD 60

// ------ main program ------ //
Platform platform;

// change these 12 parameters to implement different movements

float theta1 = TWO_PI/8 * 2;
float theta2 = TWO_PI/8 * 2;
float theta3 = TWO_PI/8 * 4;
float theta4 = TWO_PI/8 * 6;
float theta5 = TWO_PI/8 * 6;
float theta6 = TWO_PI/8 * 4;

float freq1 = 2.3;
float freq2 = 4.7;
float freq3 = 0.4;
float freq4 = 6.1;
float freq5 = 0.7;
float freq6 = 4.4;

float speed_coffecient = 250;
float t=0.0;
float position[6]={0,0,6,0,0,0};
  
void setup() {
  Serial.begin(9600);

  // set the analog ref (5V regulator)
  analogReference(EXTERNAL);

  pinMode(SHDN_BTN, INPUT_PULLUP);

  platform.setup();

  platform.setPlatformPosition(position);

  while(!platform.isPlatformReady())
  {
    platform.loop();
  }
  
  // Serial.println("Actuator info");
  // for (int i = 0; i < NUM_ACTUATORS; i++) {
  //   Serial.print("Actuator "); Serial.print(i); 
  //   Serial.print(" position: ");
  //   Serial.println(platform.getActuatorPosition(i));
  //   Serial.print("min: "); 
  //   Serial.println(platform.getActuatorMinPosition(i));
  //   Serial.print("max: "); 
  //   Serial.println(platform.getActuatorMaxPosition(i));
  //   Serial.print("target: ");
  //   Serial.println(platform.getActuatorTarget(i));
  //   Serial.print("ready: ");
  //   Serial.println(platform.isActuatorReady(i));
  //   Serial.println();
  // } 
}

void loop() {
  if(digitalRead(SHDN_BTN) == LOW) {
    platform.retract();
  } 
  else {
    t++;
    float speeds[] = {speed_coffecient*cos(freq1*t/PERIOD+theta1),speed_coffecient*cos(freq2*t/PERIOD+theta2),speed_coffecient*cos(freq3*t/PERIOD+theta3),speed_coffecient*cos(freq4*t/PERIOD+theta4),speed_coffecient*cos(freq5*t/PERIOD+theta5),speed_coffecient*cos(freq6*t/PERIOD+theta6)};
    // float speeds[] = {255*cos(t/FREQ+theta1),0,0,0,0,0};
    platform.setAcuatorsSpeeds_(speeds);   // self defined function to set acuators' speeds in each loop
  }
  
  // print the current raw and filtered positions of each actuator
  for (int i = 0; i < NUM_ACTUATORS; i++) {
    Serial.print(platform.getActuatorPosition(i));
    Serial.print(',');
  } 

  Serial.println();
  delay(PERIOD);
}
