#include "RoboClaw.h"
#include <SoftwareSerial.h>
#include "SerialTransfer.h"
#include <elapsedMillis.h>
#include <HCSR04.h>

SoftwareSerial serial(10,11); 
RoboClaw roboclaw(&serial, 10000); 

SerialTransfer anchor1_transfer;
SerialTransfer anchor2_transfer;

HCSR04 hc(5, new int[2]{2,4}, 2);

#define address 0x80

float output1; 
float output2; 
float ultraSonicDist;
bool blocked = false;
bool blocked2 = false;

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
  roboclaw.ForwardM1(address, 30); 
  roboclaw.BackwardM2(address, 5); 
}
void turn_right() {
  roboclaw.BackwardM1(address, 5);
  roboclaw.ForwardM2(address, 30); 
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


void makeBeep(){
  tone(8, 350, 125);
  delay(250);
}

void stopBeep(){
  noTone(8);
}


void loop () {
  read_serial();

  // read switch data
  int switchValue = analogRead(A0);
  // Serial.println(switchValue);
  if (switchValue > 1000)
  {
    // delay(60); 
    // Serial.print("Sensor 0: ");
    // Serial.println(hc.dist(0)); 
    // Serial.print("Sensor 1: ");
    // Serial.println(hc.dist(1));


    // if (hc.dist(0)<5 && hc.dist(0)>0 && hc.dist(1)<5 && hc.dist(1)>0){
    //   makeBeep();
    //   stop();  
    // }
    // else if (hc.dist(0)<5 && hc.dist(0)>0 && hc.dist(1)>5){
    //   makeBeep();
    //   stop();    

    //   elapsedMillis timeElapsed1;
    //   unsigned int interval1 = 500; 
    //   while(timeElapsed1 < interval1){
    //       backward();
    //   }
     
    //   elapsedMillis timeElapsed2;
    //   unsigned int interval2 = 500; 
    //   while(timeElapsed2 < interval2){
    //       turn_left();
    //   }      	  
    // }    
    // else if (hc.dist(1)<5 && hc.dist(1)>0 && hc.dist(0)>5){
    //   makeBeep();
    //   stop();    
    
    //   elapsedMillis timeElapsed1;
    //   unsigned int interval1 = 750; 
    //   while(timeElapsed1 < interval1){
    //       backward();
    //   }
     
    //   elapsedMillis timeElapsed2;
    //   unsigned int interval2 = 750; 
    //   while(timeElapsed2 < interval2){
    //       turn_right();
    //   }
    // }
    // else{
    //   stopBeep();
    // }


    delay(60); 
    if (hc.dist(0)<30.0 && hc.dist(0)>0 && !isnan(hc.dist(0))){
      makeBeep();  
      blocked = true;  
      stop();
      
    }
    else{
      stopBeep();
      blocked = false;
    }
    
    delay(10);
    if (hc.dist(1)<30.0 && hc.dist(1)>0 && !isnan(hc.dist(1))){
      makeBeep();  
      blocked2 = true;  
      stop();
      
    }
    else{
      stopBeep();
      blocked2 = false;
    }

    delay(20);  
    float avg = (output1+output2)/2;
    if (output1>0 && output2>0 && avg<5.0 && !isnan(output1) && !isnan(output2) && !blocked && !blocked2){
      if (avg > 0.4){ 
        if (output1-output2 > 0.3){ 
          turn_right();
        }
        else if (output2-output1 > 0.3){
          turn_left();
        }
        else{
          forward(40);
        }    
      }
      else{
        stop();
      }
    }

    // delay(60); 
    // Serial.print("Sensor 0: ");
    // Serial.println(hc.dist(0)); 
    // Serial.print("Sensor 1: ");
    // Serial.println(hc.dist(1)); 
    // if (hc.dist(0)<5 && hc.dist(0)>0 && hc.dist(1)<5 && hc.dist(1)>0){
    //   makeBeep();
    //   stop();  
    // }
    // else if (hc.dist(0)<5 && hc.dist(0)>0 && hc.dist(1)>5){
    //   makeBeep();
    //   stop();    

    //   elapsedMillis timeElapsed1;
    //   unsigned int interval1 = 750; 
    //   while(timeElapsed1 < interval1){
    //       backward();
    //   }
     
    //   elapsedMillis timeElapsed2;
    //   unsigned int interval2 = 750; 
    //   while(timeElapsed2 < interval2){
    //       turn_left();
    //   }      	  
    // }    
    // else if (hc.dist(1)<5 && hc.dist(1)>0 && hc.dist(0)>5){
    //   makeBeep();
    //   stop();    
    
    //   elapsedMillis timeElapsed1;
    //   unsigned int interval1 = 750; 
    //   while(timeElapsed1 < interval1){
    //       backward();
    //   }
     
    //   elapsedMillis timeElapsed2;
    //   unsigned int interval2 = 750; 
    //   while(timeElapsed2 < interval2){
    //       turn_right();
    //   }
    // }
    // else{
    //   stopBeep();
    // }


  }
  else{
    stop();
  }  
}