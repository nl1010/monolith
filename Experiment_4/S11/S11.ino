#include <Wire.h>

//Global Vars///
// LED
int BREATH_LED_PIN = 5;
int MAX_LED_STRENGTH = 50; //MAX = 255 
int MIN_LED_STRENGTH = 1;
int BREATH_TIME = 3000; //in ms, change for a single breath time , including full cycle
int BREATH_GAP = 2000 ; //in ms, gap between two breathing time

//Tokens
int arrange_token = 0; //everytime device inits the token is 0 

//vvvvvvv M-S Table -- shared to every module vvvvvvvv//
int MASTER_ID = 10;
int SLAVE1_ID = 11;  
int SLAVE2_ID = 12;  
int SLAVE3_ID = 13;
int DEVICE_ID_ARRY[] = {10,  11,  12,  13,  14,  15}; 
//                      M    S1   S2   S3   S4   S5
int SELF_NO = 1; // the first number in the array, which is M
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

//Linked Vars 
int BREATH_STEP = (BREATH_TIME / (MAX_LED_STRENGTH - MIN_LED_STRENGTH))/2;


// these should work after attached to the power source 
void setup() {
  Wire.begin(DEVICE_ID_ARRY[SELF_NO]);
  Serial.begin(9600);
  pinMode(BREATH_LED_PIN,OUTPUT);

  //register on the bus for receving the signal 
  Wire.onReceive(receiveEvent);
  
  // broadcast itself and increse its token 
  for (int i=0; i< (sizeof(DEVICE_ID_ARRY)/sizeof(char *));i++){ 
   if (i != SELF_NO) {
   Wire.beginTransmission(DEVICE_ID_ARRY[i]);
   Wire.write('o');
   Serial.print("broadcast to ");
   Serial.println (DEVICE_ID_ARRY[i]);
   Wire.endTransmission(DEVICE_ID_ARRY[i]);

   }
  }
   arrange_token = arrange_token+1;
   Serial.println(arrange_token);
}



void loop() {

//  Wire.requestFrom(SLAVE1_ID,1);
//  while(Wire.available()>0) {
//    char sig = Wire.read();
//    Serial.print(sig);
//    if (sig == '1') {
//       led_breath(BREATH_LED_PIN); //let's flash
//    }
//  }
}

void receiveEvent (int len) {
   if (Wire.available()>0){
    char c = Wire.read();
    if (c == 'o') {
      arrange_token = arrange_token + 1;
      Serial.print("New Device Detected , AT = ");
      Serial.println(arrange_token);
    }
   }
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

