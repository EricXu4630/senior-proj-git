#include "SerialTransfer.h"


SerialTransfer anchor2_buf;
SerialTransfer anchor1_buf;



void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  anchor2_buf.begin(Serial2);
  Serial3.begin(115200); 
  anchor1_buf.begin(Serial3); 
}


void loop()
{
  if(anchor1_buf.available()){
    uint16_t recSize1 = 0;
    float res1; 
    recSize1 = anchor1_buf.rxObj(res1, recSize1);
    Serial.print("res1: ");
    Serial.println(res1);

  }

  if(anchor2_buf.available())
  {
    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize2 = 0;

    float res2;

    // recSize = anchor2_buf.rxObj(testStruct, recSize);
    // Serial.print(testStruct.z);
    // Serial.print(testStruct.y);
    // Serial.print(" | ");

    recSize2 = anchor1_buf.rxObj(res2, recSize2);
    
    Serial.print("res2: ");
    Serial.println(res2);
  } 
}