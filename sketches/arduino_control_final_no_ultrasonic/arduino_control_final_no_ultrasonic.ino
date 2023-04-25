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
    // Serial.print("New Data 2: ");
    recSize = anchor2_transfer.rxObj(output2, recSize);
    // Serial.println(output2);
  }
  else if(anchor2_transfer.status < 0){
    // Serial.print("ERROR anch2: ");
    // Serial.println(anchor2_transfer.status);
  }

  if(anchor1_transfer.available()){
    uint16_t recSize = 0;
    // Serial.print("New Data 1: ");
    recSize = anchor1_transfer.rxObj(output1, recSize);
    // Serial.println(output1);
  }
  else if(anchor1_transfer.status < 0){
    // Serial.print("ERROR anch1: ");
    // Serial.println(anchor1_transfer.status);
  }
}

void turn_left() {
  roboclaw.ForwardM1(address, 35); 
  roboclaw.ForwardM2(address, 0); 
}
void turn_right() {
  roboclaw.ForwardM1(address, 0);
  roboclaw.ForwardM2(address, 35); 
}
void forward(float speed) {
  roboclaw.ForwardM1(address, speed);
  roboclaw.ForwardM2(address, speed); 
}
void backward() {
  roboclaw.BackwardM1(address, 25);
  roboclaw.BackwardM2(address, 25);
}
void stop() {
  roboclaw.ForwardM1(address, 0);
  roboclaw.ForwardM2(address, 0); 
}



void loop () {
  read_serial();

  // read switch data
  int switchValue = analogRead(A0);
  if (switchValue > 1000)
  {
    delay(50);  
    float avg = (output1+output2)/2;
    if (output1>0 && output2>0 && avg<5.0 && !isnan(output1) && !isnan(output2)){
      if (avg > 0.4){ 
        if (output1-output2 > 0.3){ 
          delay(10);
          turn_right();
        }
        else if (output2-output1 > 0.3){
          delay(10);
          turn_left();
        }
        else{
          delay(10);
          forward(100);
        }    
      }
      else{
        stop();
      }
    }
  }
  else{
    stop();
  }  
}