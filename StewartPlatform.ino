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

#define PERIOD_QUICK 50   // 120 会变陡峭但慢，30 会变快但是不陡峭
#define PERIOD_MEDIUM 100
#define PERIOD_SLOW 150

// ------ main program ------ //
Platform platform;

// change these 12 parameters to implement different movements

float theta1 = TWO_PI/8 * 2;
float theta2 = TWO_PI/8 * 3;
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
unsigned long startTime;
unsigned long currentTime;

int PERIOD;
  
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
  
  startTime = millis();
}

void loop() {
  currentTime = (millis() - startTime) / 1000;
  Serial.print(currentTime);
  Serial.print(",");

  // pause
  if((currentTime>0&&currentTime<30)||(currentTime>120&&currentTime<155)||(currentTime>190&&currentTime<210)||(currentTime>350&&currentTime<420)
  ||(currentTime>500&&currentTime<520)||(currentTime>550&&currentTime<595)||(currentTime>640&&currentTime<655)||(currentTime>715&&currentTime<795)){
    platform.setPlatformPosition(position);
    while(!platform.isPlatformReady()){
      platform.loop();
      Serial.print("pause");
      Serial.println();
    }
    Serial.print("pause");
    Serial.println();
  }
  else{
    // quick
    if((currentTime>90&&currentTime<100)||(currentTime>110&&currentTime<120)||(currentTime>280&&currentTime<350)||(currentTime>420&&currentTime<500)||(currentTime>595&&currentTime<640)){
      PERIOD = PERIOD_QUICK;
      Serial.print("quick");
    }
    // medium
    else if((currentTime>30&&currentTime<90)||(currentTime>155&&currentTime<190)||(currentTime>210&&currentTime<280)||(currentTime>655&&currentTime<715)){
      PERIOD = PERIOD_MEDIUM;
      Serial.print("medium");
    }
    // slow
    else if((currentTime>100&&currentTime<110)||(currentTime>520&&currentTime<550)||(currentTime>795&&currentTime<960)){
      PERIOD = PERIOD_SLOW;
      Serial.print("slow");
    }

    if(digitalRead(SHDN_BTN) == LOW) {
      platform.retract();
    } 
    else {
      t++;
      float speeds[] = {speed_coffecient*cos(freq1*t/PERIOD+theta1),speed_coffecient*cos(freq2*t/PERIOD+theta2),speed_coffecient*cos(freq3*t/PERIOD+theta3),speed_coffecient*cos(freq4*t/PERIOD+theta4),speed_coffecient*cos(freq5*t/PERIOD+theta5),speed_coffecient*cos(freq6*t/PERIOD+theta6)};
      // float speeds[] = {255*cos(t/FREQ+theta1),0,0,0,0,0};
      platform.setAcuatorsSpeeds_(speeds);   // self defined function to set acuators' speeds in each loop
    }
    Serial.println();
    delay(PERIOD);
  }
}
