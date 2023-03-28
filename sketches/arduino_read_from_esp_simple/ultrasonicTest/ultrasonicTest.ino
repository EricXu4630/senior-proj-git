#include <HCSR04.h>

HCSR04 hc(2, 3); //initialisation class HCSR04 (trig pin , echo pin)

void setup()
{ Serial.begin(9600); }

void loop()
{
    Serial.println( hc.dist() ); //return current distance (cm) in serial
    if(hc.dist()<15 && hc.dist() > 0){
      Serial.println("Obstacle Detected");
    }
    delay(60);                   // we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.
}