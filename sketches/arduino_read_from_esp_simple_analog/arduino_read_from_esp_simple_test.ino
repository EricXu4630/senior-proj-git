#include "RoboClaw.h"
#include <SoftwareSerial.h>
#include "SerialTransfer.h"


SoftwareSerial serial(10,11); 
// RoboClaw roboclaw(&serial, 10000); 

SerialTransfer anchor1_transfer;
SerialTransfer anchor2_transfer;



#define address 0x80

// int counter1 = 0;
// char input_stack1[4];
float output1; 
// int counter2 = 0;
// char input_stack2[4];
float output2; 

struct Vec {
  float x; 
  float y; 
} v;

unsigned long starttime;
unsigned long endtime;

void setup() {
  // put your setup code here, to run once:
  serial.begin(38400); 
  Serial.begin(115200); 
  Serial3.begin(115200);
  anchor1_transfer.begin(Serial3); 
  Serial2.begin(115200);
  anchor2_transfer.begin(Serial2); 
  // roboclaw.begin(38400); 
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
    Serial.print("ERROR: ");
    Serial.println(anchor2_transfer.status);
    anchor2_transfer.available();

  }

  if(anchor1_transfer.available()){
    uint16_t recSize = 0;
    Serial.println("New Data 1");
    recSize = anchor1_transfer.rxObj(output1, recSize);
    Serial.println(output1);
  }
  else if(anchor1_transfer.status < 0){
    Serial.print("ERROR: ");
    Serial.println(anchor1_transfer.status);
    anchor1_transfer.available();
  }
}

void turn_left() {
  serial.write(72);

  // Serial.println("left"); 
  // roboclaw.ForwardM1(address, 20); 
  // roboclaw.TurnLeftMixed(address, 20); 
}

void turn_right() {
    serial.write(200);
  // Serial.println("right"); 
  // roboclaw.ForwardM2(address, 20); 
  // roboclaw.TurnRightMixed(address, 20); 
}

void forward() {
    serial.write(72) && serial.write(200);

  // Serial.println("forward"); 
  // roboclaw.ForwardM1(address, 20);
  // roboclaw.ForwardM2(address, 20); 
  // roboclaw.ForwardMixed(address, 30); 
}


void stop () {
  // serial.write(64) && serial.write(192);

  // Serial.println("stopping"); 
  // roboclaw.ForwardM1(address, 0);
  // roboclaw.ForwardM2(address, 0); 
  // roboclaw.ForwardMixed(address, 0); 
}


void loop () {
  read_serial();
  // delay(500); 

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


  if (output1 > 0.5){
    return forward(); 
      if (output1 - output2 > 0.4){
        return turn_right();
      }
      else if (output2 - output1 > 0.4){
        return turn_left();
      }
      else{
        return forward();
      }    
    }
  else{
    return stop();
  }


  //   // endtime = millis();
  // // }
  // delay(1000);

}
