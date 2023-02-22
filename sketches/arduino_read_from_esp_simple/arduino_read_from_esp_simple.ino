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
} v;

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
  float a = output2;
  float b = output1;
  float cos_a = (b*b + c*c - a*a) / (2*b*c);
  float y_val = b * sqrt(1 - (cos_a * cos_a));
  if (!isnan(cos_a)){
    v.x = b * cos_a;
  }
  if (!isnan(y_val)){
    v.y = y_val;
  }
  Serial.print(a); 
  Serial.print(","); 
  Serial.print(b); 
  Serial.println(); 
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
      if(output1>0.01 && output1<10.0 && output2>0.01 && output2<10.0){
        update_pos();
      }
  }
  if (counter2 == 4){
      memcpy(&output2, input_stack2, sizeof(float));
      // Serial.print("Output2: ");
      // Serial.println(output2);
      counter2 = 0;  
      if(output1>0.01 && output1<10.0 && output2>0.01 && output2<10.0){
        update_pos();
      }   
  }

}

void turn_left() {
  // Serial.println("left"); 
  roboclaw.ForwardM2(address, 30); 
}

void turn_right() {
  // Serial.println("right"); 
  roboclaw.ForwardM1(address, 30); 
}

void forward() {
  // Serial.println(" forward"); 
  roboclaw.ForwardM1(address, 30) && roboclaw.ForwardM2(address, 30); 
}

void forwardSlow() {
  // Serial.println("going forward"); 
  roboclaw.ForwardM1(address, 10); 
  roboclaw.ForwardM2(address, 10); 
}

void stop () {
  // Serial.println("stopping"); 
  roboclaw.ForwardM1(address, 0); 
  roboclaw.ForwardM2(address, 0); 
}


void loop () {
  read_serial(); 
  // if (v.y > 1.0){
  //   return forward();
  // }
  if (v.x < -0.1){
     return turn_right();
  } 
  if (v.x > 0.6){
     return turn_left();
  } 
  stop();
  delay(300);
}
