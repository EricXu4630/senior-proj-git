#include "RoboClaw.h"
#include <SoftwareSerial.h>

SoftwareSerial serial(10,11); 
RoboClaw roboclaw(&serial, 10000); 

#define address 0x80

int counter1 = 0;
char input_stack1[4];
float output1; 
int counter2 = 0;
char input_stack2[4];
float output2; 

struct Vec {
  float x; 
  float y; 
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  Serial3.begin(115200);
  Serial2.begin(115200);
  roboclaw.begin(38400); 
}

void update_pos(){
  // c is distance between anchors. b is output2, a is output1
  const float c = 0.52;
  float cos_a = (output2 * output2 + c*c - output1 * output1) / (2 * output2 * c);
  Vec v; 
  v.x = output2 * cos_a;
  v.y = output2 * sqrt(1 - cos_a * cos_a);
  return v; 
}

void read_serial() {
  // put your main code here, to run repeatedly:
  if(Serial3.available()){
      input_stack1[counter1++] = Serial3.read(); 
  }
  if(Serial2.available()){
      input_stack2[counter2++] = Serial2.read(); 
  }
  if (counter1 == 4){
      memcpy(&output1, input_stack1, sizeof(float));
      // Serial.print("Output1: ");
      // Serial.println(output1);
      counter1 = 0;
  }
  if (counter2 == 4){
      memcpy(&output2, input_stack2, sizeof(float));
      // Serial.print("Output2: ");
      // Serial.println(output2);
      counter2 = 0;     
  }
  if (output1 > 0.0 && output1 < 10.0 && output2 > 0.0 && output2 < 10.0){
    update_pos()
  }
}

void turn_left() {
  // Serial.println("turning"); 
  // roboclaw.BackwardM1(address, 30); 
  roboclaw.ForwardM2(address, 30); 
}

void turn_right() {
  // Serial.println("turning"); 
  roboclaw.ForwardM1(address, 30); 
  // roboclaw.BackwardM2(address, 30); 
}

void forward() {
  // Serial.println("going forward"); 
  roboclaw.ForwardM1(address, 30); 
  roboclaw.ForwardM2(address, 30); 
}

void stop () {
  // Serial.println("stopping"); 
  roboclaw.ForwardM1(address, 0); 
  roboclaw.ForwardM2(address, 0); 
}


void loop () {
  read_serial(); 
  if (v.x < -0.5) {
     return turn_right();
  } 
  if (v.x > 1.0) {
     return turn_left();
  } 
  if (v.y >= 2.0){
    return forward(); 
  }
  stop();
}
