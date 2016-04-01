#include <Wire.h>

//Global Vars///
// LED
int BREATH_LED_PIN = 5;
int MAX_LED_STRENGTH = 50; //MAX = 255 
int MIN_LED_STRENGTH = 1;
int BREATH_TIME = 3000; //in ms, change for a single breath time , including full cycle
int BREATH_GAP = 2000 ; //in ms, gap between two breathing time

//vvvvvvv M-S Table -- shared to every module vvvvvvvv//
int MASTER_ID = 10;
int SLAVE1_ID = 11;  
int SLAVE2_ID = 12;  
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

//Linked Vars 
int BREATH_STEP = (BREATH_TIME / (MAX_LED_STRENGTH - MIN_LED_STRENGTH))/2;

// flags 
int shining = 0;

void setup() {
  Wire.begin(SLAVE1_ID);
  Serial.begin(9600);
  pinMode(BREATH_LED_PIN,OUTPUT);

  // listen from master 
//  Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

}

void loop() {
   if (shining == 1) {
    led_breath(BREATH_LED_PIN);
   }
   shining = 0;
}

void requestEvent(){
  // cannot delay, must use flag to execute local event
   Wire.write('1');
   shining = 1;
//  delay(5000);
//  
}



/* Aux functions*/
void led_breath(int breathpin){
    for (int i= MIN_LED_STRENGTH; i<=MAX_LED_STRENGTH; i++)
  {
    analogWrite(breathpin,i);
    delay(BREATH_STEP);
  }
  
  for (int i=MAX_LED_STRENGTH; i>=MIN_LED_STRENGTH; i--)
  {
    analogWrite(breathpin,i);
    delay(BREATH_STEP);
  }
  delay(BREATH_GAP);
}

