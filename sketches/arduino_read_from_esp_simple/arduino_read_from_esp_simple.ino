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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  Serial3.begin(115200);
  anchor1_transfer.begin(Serial3); 
  Serial2.begin(115200);
  anchor2_transfer.begin(Serial2); 
  roboclaw.begin(38400); 
}

void read_serial() {
  if(anchor2_transfer.available()){
    uint16_t recSize = 0;
    // Serial.println("New Data 2");
    recSize = anchor2_transfer.rxObj(output2, recSize);
    // Serial.println(output2);
  }
  else if(anchor2_transfer.status < 0){
    Serial.print("ERROR anch2: ");
    Serial.println(anchor2_transfer.status);
  }

  if(anchor1_transfer.available()){
    uint16_t recSize = 0;
    // Serial.println("New Data 1");
    recSize = anchor1_transfer.rxObj(output1, recSize);
    // Serial.println(output1);
  }
  else if(anchor1_transfer.status < 0){
    Serial.print("ERROR anch1: ");
    Serial.println(anchor1_transfer.status);
  }
}

void turn_left() {
  roboclaw.ForwardM1(address, 35); 
  roboclaw.BackwardM2(address, 10); 
}

void turn_right() {
  roboclaw.BackwardM1(address, 10);
  roboclaw.ForwardM2(address, 35); 
}

void forward(float speed) {
  roboclaw.ForwardM1(address, speed);
  roboclaw.ForwardM2(address, speed); 
}

void stop() {
  roboclaw.ForwardM1(address, 0);
  roboclaw.ForwardM2(address, 0); 
}

void loop () {
  read_serial();
  
  delay(random(10, 20));
  float avg = (output1+output2)/2;

  if (avg>=0 && avg<4.0 && !isnan(output1) && !isnan(output2)){
    if (avg > 0.5){ 
      if (output1-output2 > 0.2){ 
        turn_right();
      }
      else if (output2-output1 > 0.2){
        turn_left();
      }
      else{
        forward(20+(avg*30));
      }    
    }
    else{

      stop();
    }
  }
}


