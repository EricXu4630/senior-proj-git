#include "RoboClaw.h"
#include <SoftwareSerial.h>
#include "SerialTransfer.h"
#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library

SoftwareSerial serial(10,11); 
RoboClaw roboclaw(&serial, 10000); 

SerialTransfer anchor1_transfer;
SerialTransfer anchor2_transfer;

// #define SONAR_NUM 3      // Number of sensors.
// #define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.

// NewPing sonar[SONAR_NUM] = {   // Sensor object array.
//   NewPing(4, 5, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
//   NewPing(6, 7, MAX_DISTANCE), 
//   NewPing(8, 9, MAX_DISTANCE)
// };

#define address 0x80

float output1; 
float output2; 
float ultraSonicDist;

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
    // Serial.println("New Data 2");
    recSize = anchor2_transfer.rxObj(output2, recSize);
    // Serial.println(output2);
  }
  else if(anchor2_transfer.status < 0){
    // Serial.print("ERROR anch2: ");
    // Serial.println(anchor2_transfer.status);
  }

  if(anchor1_transfer.available()){
    uint16_t recSize = 0;
    // Serial.println("New Data 1");
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
  roboclaw.BackwardM2(address, 15); 
}

void turn_right() {
  roboclaw.BackwardM1(address, 15);
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

  // for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
  //   delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  //   Serial.print(i);
  //   Serial.print("=");
  //   Serial.print(sonar[i].ping_cm());
  //   Serial.print("cm ");
  // }
  // Serial.println();
  
  // read switch data
  int switchValue = analogRead(A0);

  delay(random(10, 20));
  float avg = (output1+output2)/2;
  Serial.print(switchValue);
  Serial.println();

    /*
    if obstacle...
    from right: stop and reverse for 1 second, turn left.
    from left: stop and reverse for 1 second, turn right.
    else...
    beep
    */
    //put ultrasonic code here
  if (avg>=0 && avg<4.0 && !isnan(output1) && !isnan(output2)){
    if (avg > 0.4){ 
      if (output1-output2 > 0.2){ 
        turn_right();
      }
      else if (output2-output1 > 0.2){
        turn_left();
      }
      else{
        forward(18+(avg*30));
      }    
    }
    else{
      stop();
    }
  }
}



