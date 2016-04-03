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
  Wire.begin(); // start I2C communication and nothing in() as the master device 
  Serial.begin(9600);
  pinMode(BREATH_LED_PIN,OUTPUT);
}


//Main
void loop() {
  // vv I2C TRANSMISSION vv //
  Wire.beginTransmission(SLAVE1_ID); 
  Wire.write("hello");  // send to slave strings
  Wire.endTransmission(SLAVE1_ID);
  // ^^ I2C TRANSMISSION ^^ //

  // vv I2C LISTENNING vv // 
  Wire.requestFrom(SLAVE1_ID, 10); // request 10 byte from the target slave. 
  while (Wire.available()>1) {    // keep listenning in each loop, until detected receive byte from the target slave do something futhur 
    char c = Wire.read();
    Serial.print(c);
  }
  char d = Wire.read();
  Serial.print(d);
  led_breath(BREATH_LED_PIN);
  // ^^ I2C LISTENNING ^^ //
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

