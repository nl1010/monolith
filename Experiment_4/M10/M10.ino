#include <Wire.h>

//Global Vars///
// LED
int BREATH_LED_PIN = 5;
int MAX_LED_STRENGTH = 50; //MAX = 255 
int MIN_LED_STRENGTH = 1;
int BREATH_TIME = 3000; //in ms, change for a single breath time , including full cycle
int BREATH_GAP = 2000 ; //in ms, gap between two breathing time

//Tokens
int age = 0; //everytime device inits the token is 0 

/*Living List
  1.In living list, value position in the array corresponding to their device ID; 
  2.0 means they are dead/offline. all number >0 is their actual age.
*/


int LIVING_LIST[100];  //generate an array that could contain 100 valubles, all init value is 0 <- tested 


//vvvvvvv M-S Table -- shared to every module vvvvvvvv//
int MASTER_ID = 10;
int SLAVE1_ID = 11;  
int SLAVE2_ID = 12;  
int SLAVE3_ID = 13;
int DEVICE_ID_ARRY[] = {10,  11,  12,  13,  14,  15}; 
//                      M    S1   S2   S3   S4   S5
int SELF_NO = 0; // the first number in the array, which is M
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

//Linked Vars 
int BREATH_STEP = (BREATH_TIME / (MAX_LED_STRENGTH - MIN_LED_STRENGTH))/2;

//FLAGS
int BROADCAST_FLAG = 0;

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
   age = age+1;
   Serial.print("arrange token = ");
   Serial.println(age);
}



void loop() {
  //broadcast handler
  if (BROADCAST_FLAG == 1){
    broadcastSelfState();
    BROADCAST_FLAG = 0;
  }  
  
//  Wire.requestFrom(SLAVE1_ID,1);
//  while(Wire.available()>0) {
//    char sig = Wire.read();
//    Serial.print(sig);
//    if (sig == '1') {
//       led_breath(BREATH_LED_PIN); //let's flash
//    }
//  }
}


/*When receive the ON signal from the new modules, it 'knows' he is older.
STEP 1: increase its age 
STEP 1.5: everyone will broadcast its current age to everyone else in the system
STEP 1.6: everyone will receive others' device_id and corresponding age. 
STEP 1.7: everyone will check their own age if is the oldest
STEP 2: the oldest one in the system will arrange broadcasting, and other one will receive 
*/


void receiveEvent (int len) {
   while (Wire.available()>0){
    char c = Wire.read();
//    Serial.print("Signal Received: ");
//    Serial.println(c);
    /*if signal == ON from other device which means other device just attached, as the rule everyone 
    still get the power will increse their age.*/
    if (c == 'o') {
      // if it is docker, then its age should be oldest
      if (SELF_NO == 0) {
        age = 999;
      } else {
        age = age + 1;
      }
      Serial.print("New Device Detected , AT = ");
      Serial.println(age);
      //1.5 update its own LIVING LIST and tell everyone selfid and the age.
      LIVING_LIST[SELF_NO] = age;
      BROADCAST_FLAG = 1;  //start broadcast selfstate. -- if put long code block here will have issue. should move to loop()
   }

   if (c == 'c'){
    String data = "";
    data = data + (char)c;
    while (Wire.available()>0){
      data += (char)Wire.read();
    }
    Serial.print("Signal Received: ");
    Serial.println(data);
    //TODO: processing data 
    
    updateLivingList(data);   
   }
  }
}

void broadcastSelfState(){
        for (int i=0; i< (sizeof(DEVICE_ID_ARRY)/sizeof(char *));i++){ 
       if (i != SELF_NO) {   
         Wire.beginTransmission(DEVICE_ID_ARRY[i]);
         String data = "";
         data = String("c_")+ DEVICE_ID_ARRY[SELF_NO] +String("_")+ age;
         Wire.write(data.c_str());
         Serial.print("Tell ");
         Serial.print(DEVICE_ID_ARRY[i]);
         Serial.print(" :");
         Serial.println (data.c_str());
         Wire.endTransmission(DEVICE_ID_ARRY[i]);
       }
      }
}



/*living list is a list of EVERYBODY's information INCLUDING ITSELF.
It is keep updated once receive other's STATE BROADCAST SIGNAL.
For searching this list, they will be able to know if he is the guy to control light signal.
STEP 1. get the data string received, PRE: data must start with "c_[device ID]_[age]".  
STEP 2. Stretch out age and store it into the LIVING_LIST[device_id]
*/
void updateLivingList(String data){
  // data = "c_device_age"
  String device_str = getValue(data,'_',1); 
  String age_str = getValue(data,'_',2);
  Serial.print ("updating agelist");
  Serial.println(device_str+age_str);
}


/* Aux functions*/
void led_breath(int breathpin){
    for (int i= MIN_LED_STRENGTH; i<=MAX_LED_STRENGTH; i++){
    analogWrite(breathpin,i); 
    delay(BREATH_STEP);
  }
  
  for (int i=MAX_LED_STRENGTH; i>=MIN_LED_STRENGTH; i--){
    analogWrite(breathpin,i);
    delay(BREATH_STEP);
  }
  delay(BREATH_GAP);
}



//takes a string and separates it based on a given character and returns The item between the separating character
/*e.g. for getValue("c_10,77",'_', 0-2) -> "c","10","77"*/
String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

