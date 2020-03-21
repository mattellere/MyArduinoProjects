/*
   -- RemoteXYRobot --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.5.1 or later version;
     - for iOS 1.4.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "MyRobot"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,5,0,0,0,58,0,10,13,1,
  5,47,9,36,49,49,2,26,31,3,
  132,16,5,33,9,2,26,2,1,41,
  91,20,10,4,26,31,31,79,78,0,
  79,70,70,0,4,160,6,22,54,6,
  37,26,129,0,24,-1,18,6,17,80,
  111,119,101,114,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t theJoystick_x; // =-100..100 x-coordinate joystick position 
  int8_t theJoystick_y; // =-100..100 y-coordinate joystick position 
  uint8_t selectVelocity; // =0 if select position A, =1 if position B, =2 if position C, ... 
  uint8_t btnOn; // =1 if switch ON and =0 if OFF 
  int8_t sliderBalance; // =-100..100 slider position 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


#define RightMotorSpeed 5
#define RightMotorDir 0

#define LeftMotorSpeed 4
#define LeftMotorDir 2


double multiplier = 0.60;

void setup() 
{
  RemoteXY_Init (); 
  
  // TODO you setup code
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  
  switch (RemoteXY.selectVelocity)
  {
    default:
    case 0:
    {
      multiplier = 0.60;
      break;
    }
    case 1:
    {
      multiplier = 0.75;
      break;
    }
    case 2:
    {
      multiplier = 0.90;
      break;
    }
    case 3:
    {
      multiplier = 1;
      break;
    }
  }
  
  if (RemoteXY.btnOn == 0 || RemoteXY.connect_flag == 0)
  {
    Halt();
  }
  else
  {
    if (RemoteXY.theJoystick_y >= 0)
      Forward(RemoteXY.theJoystick_x, RemoteXY.theJoystick_y, multiplier);
    else
      Reverse(RemoteXY.theJoystick_x, RemoteXY.theJoystick_y, multiplier); 
  }
}

void Halt()
{
 digitalWrite(RightMotorSpeed, LOW);
 digitalWrite(LeftMotorSpeed, LOW);
}

void Forward(int x, int y, double speedFactor)
{
 digitalWrite(RightMotorDir, LOW);
 digitalWrite(LeftMotorDir, LOW);

 MoveWheels(x, y, speedFactor);
}

void Reverse(int x, int y, double speedFactor)
{
 digitalWrite(RightMotorDir, HIGH);
 digitalWrite(LeftMotorDir, HIGH);

 MoveWheels(x, y, speedFactor);
}

void MoveWheels(int x, int y, double speedFactor)
{
  int rightBalance = 0;
  int leftBalance = 0;
  
  if (RemoteXY.sliderBalance >= 0)
    leftBalance = RemoteXY.sliderBalance;
  else
    rightBalance = abs(RemoteXY.sliderBalance);
  
  int RightSpeed = map((abs(y) + (x * 0.5) - rightBalance), -50, 150, 0, 1024);
  int LeftSpeed = map((abs(y) - (x * 0.5) - leftBalance), -50, 150, 0, 1024);
 
  analogWrite(RightMotorSpeed, RightSpeed * speedFactor);
  analogWrite(LeftMotorSpeed, LeftSpeed * speedFactor);
}
