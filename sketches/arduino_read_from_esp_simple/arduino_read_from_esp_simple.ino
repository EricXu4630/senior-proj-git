#include "RoboClaw.h"
#include <SoftwareSerial.h>
#include "SerialTransfer.h"


SoftwareSerial serial(10,11); 
RoboClaw roboclaw(&serial, 10000); 

SerialTransfer anchor1_transfer;
SerialTransfer anchor2_transfer;

#define address 0x80

float output1; 
float output2; 

struct Vec {
  float x; 
  float y; 
} v;

unsigned long starttime;
unsigned long endtime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  Serial3.begin(115200);
  anchor1_transfer.begin(Serial3); 
  Serial2.begin(115200);
  anchor2_transfer.begin(Serial2); 
  roboclaw.begin(38400); 
}

void update_pos(){
  // CHECK FOLLOWING
  const float c = 0.495;
  float b = output1;
  float a = output2;
  float cos_a = (b*b + c*c - a*a) / (2*b*c);
  v.x = b * cos_a;  
  v.y = b * sqrt(1 - (cos_a * cos_a));

}

void read_serial() {
  if(anchor2_transfer.available()){
    uint16_t recSize = 0;
    Serial.println("New Data 2");
    recSize = anchor2_transfer.rxObj(output2, recSize);
    Serial.println(output2);
  }
  else if(anchor2_transfer.status < 0){
    Serial.print("ERROR anch2: ");
    Serial.println(anchor2_transfer.status);
  }

  if(anchor1_transfer.available()){
    uint16_t recSize = 0;
    Serial.println("New Data 1");
    recSize = anchor1_transfer.rxObj(output1, recSize);
    Serial.println(output1);
  }
  else if(anchor1_transfer.status < 0){
    Serial.print("ERROR anch1: ");
    Serial.println(anchor1_transfer.status);
  }
}

void turn_left() {
  // Serial.println("left"); 
  // roboclaw.ForwardM1(address, 40); 
  roboclaw.TurnLeftMixed(address, 40); 
}

void turn_right() {
  // Serial.println("right"); 
  // roboclaw.ForwardM2(address, 40); 
  roboclaw.TurnRightMixed(address, 40); 
}

void forward(float speed) {
  // Serial.println("forward"); 
  // roboclaw.ForwardM1(address, speed);
  // roboclaw.ForwardM2(address, speed); 
  // roboclaw.ForwardMixed(address, speed); 
}

void stop () {
  // Serial.println("stopping"); 
  // roboclaw.ForwardM1(address, 0);
  // roboclaw.ForwardM2(address, 0); 
  roboclaw.ForwardMixed(address, 0); 
}

void loop () {
  read_serial();

  // starttime = millis();
  // endtime = starttime;
  // while ((endtime - starttime) <=1000) // do this loop for up to 1000ms
  // {
  // if (!isnan(v.x) && !isnan(v.y)){
  //   if (v.y > 0.5 && v.y < 10.0){
  //     if (v.x > 0.52 && v.x < 5.0){
  //       return turn_right();
  //     }
  //     else if (v.x < 0 && v.x > -4.5){
  //       return turn_left();
  //     }
  //     else if (v.x >= 0.0 && v.x <= 0.52){
  //       return forward();
  //     }    
  //   }
  //   else if (v.y <=0.5){
  //     return stop;
  //   }
  // }


  delay(random(50, 100));
  float avg = (output1+output2)/2;

  starttime = millis();
  endtime = starttime;
  while ((endtime - starttime) <= 1000) // do this loop for up to 700ms
  {
    if (avg>=0 && avg<4.0 && !isnan(output1) && !isnan(output2)){
      if (avg > 0.5){ 
        if (output1-output2 > 0.05){ 
          delay(10);
          turn_right();
        }
        else if (output2-output1 > 0.05){
          delay(10);
          turn_left();
        }
        else{
          delay(10);
          forward(15+(avg*15));
        }    
      }
      else{
        stop();
      }
    }
    endtime = millis();
  }

}
