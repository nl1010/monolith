//Libraries 
#include <Wire.h> // For I2C communication 


//Global Vars///
// LED
int BREATH_LED_PIN = 5;
int MAX_LED_STRENGTH = 50; //MAX = 255 
int MIN_LED_STRENGTH = 1;
int BREATH_TIME = 3000; //in ms, change for a single breath time , including full cycle
int BREATH_GAP = 2000 ; //in ms, gap between two breathing time
// slaves //
// slave identification 
int SLAVE1_ID = 11;  
int SLAVE2_ID = 12;  // 

//Linked Vars 
int BREATH_STEP = (BREATH_TIME / (MAX_LED_STRENGTH - MIN_LED_STRENGTH))/2;

//Settings
void setup() {
  Wire.begin(11); // start I2C protocal and define device as #11
  Wire.onReceive(receiveEvent); // register the device can receive bytes from the bus
  Wire.onRequest(requestEvent); // register the device can receive the request command from the master  
  Serial.begin(9600);
  pinMode(BREATH_LED_PIN,OUTPUT);
}


//Main
void loop() {
  delay(100);
}     

void receiveEvent(int rev_len){
  while( Wire.available()>1) {
    char c = Wire.read();
    Serial.print(c); // print each byte received 
  }
  char d = Wire.read();
  Serial.println(d); 
}

void requestEvent(){
  Wire.write("Get it");
}

// auxies 
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

