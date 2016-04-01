//Libraries 
#include <Wire.h>


//Global Vars 
int BREATH_LED_PIN = 5;
int MAX_LED_STRENGTH = 50; //MAX = 255 
int MIN_LED_STRENGTH = 1;
int BREATH_TIME = 3000; //in ms, change for a single breath time , including full cycle
int BREATH_GAP = 2000 ; //in ms, gap between two breathing time
//Linked Vars 
int BREATH_STEP = (BREATH_TIME / (MAX_LED_STRENGTH - MIN_LED_STRENGTH))/2;

//Settings
void setup() {


   pinMode(BREATH_LED_PIN,OUTPUT);
  
}

//Main
void loop() {
  for (int i= MIN_LED_STRENGTH; i<=MAX_LED_STRENGTH; i++)
  {
    analogWrite(BREATH_LED_PIN,i);
    delay(BREATH_STEP);
  }
  
  for (int i=MAX_LED_STRENGTH; i>=MIN_LED_STRENGTH; i--)
  {
    analogWrite(BREATH_LED_PIN,i);
    delay(BREATH_STEP);
  }
  delay(BREATH_GAP);

}     
