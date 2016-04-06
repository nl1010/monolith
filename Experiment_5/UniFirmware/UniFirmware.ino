 #include <Wire.h>

int SELF_NO = 0; // the first number in the array, which is M
int DEVICE_ADDR_BOOK[] = {10,  11,  12,  13,  14,  15}; 
//                      M    S1   S2   S3   S4   S5
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
int living_list[50];  //generate an array that could contain 50 valubles, all init value is 0 <- tested 



 
//Global Vars///
// LED
int BREATH_LED_PIN = 5;
int MAX_LED_STRENGTH = 50; //MAX = 255 
int MIN_LED_STRENGTH = 1;
int BREATH_TIME = 3000; //in ms, change for a single breath time , including full cycle
int BREATH_GAP = 2000 ; //in ms, gap between two breathing time

//Tokens
int age = 0; //everytime device inits the token is 0 



//Linked Vars 
int BREATH_STEP = (BREATH_TIME / (MAX_LED_STRENGTH - MIN_LED_STRENGTH))/2;

//FLAGS
int BROADCAST_FLAG = 0;
int UPDATE_LIVINGLIST_FLAG = 0;
int UPDATE_SELF_LIVINGLIST_FLAG = 0;

//Tempulate storage variable to serve flag functions 
String received_data = "";

// these should work after attached to the power source 
void setup() {
  Wire.begin(DEVICE_ADDR_BOOK[SELF_NO]);
  Serial.begin(9600);
  pinMode(BREATH_LED_PIN,OUTPUT);

  
  //register on the bus for receving the signal 
  Wire.onReceive(receiveEvent);
  
  // broadcast itself and increse its token 
  for (int i=0; i< (sizeof(DEVICE_ADDR_BOOK)/sizeof(char *));i++){ 
   if (i != SELF_NO) {
   
   Wire.beginTransmission(DEVICE_ADDR_BOOK[i]);
   Wire.write('o');
   Serial.print("broadcast to ");
   Serial.println (DEVICE_ADDR_BOOK[i]);
   Wire.endTransmission(DEVICE_ADDR_BOOK[i]);
   }
  }
   age = age+1;
   Serial.print("arrange token = ");
   Serial.println(age);
}



void loop() {

  if (BROADCAST_FLAG == 1){
    broadcastSelfState();
    BROADCAST_FLAG = 0;
  }

  if (UPDATE_LIVINGLIST_FLAG == 1){
    updateLivinglist(received_data);
    UPDATE_LIVINGLIST_FLAG = 0;
  }

  if (UPDATE_SELF_LIVINGLIST_FLAG == 1){
    updateSelfLivinglist();
    UPDATE_SELF_LIVINGLIST_FLAG = 0;
  }
  // keep listenning 
  
  
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
    if (c == 'o') {
      age = age + 1;
      Serial.print("o signal received New Device Detected , AT = ");
      Serial.println(age);
      /*update self living status in the living list first */
      UPDATE_SELF_LIVINGLIST_FLAG = 1;
      //1.5 tell everyone selfid and the age.
      BROADCAST_FLAG = 1;
   } 
   /*if receive other broadcasted live state then it will update their's living state*/
   else if (c == 'b'){
    String data = "";
    data = data + (char)c;
    while (Wire.available()>0){
      data += (char)Wire.read();
      delay(10);
    }
    Serial.print("Signal Received: ");
    Serial.println(data);
    Serial.println("Start updating living list");
    // then updating the list 
    received_data = data; 
    UPDATE_LIVINGLIST_FLAG = 1;
   }
  }
}


/*when self age state changing, it need to tell outside its state ASAP*/
void broadcastSelfState(){
        for (int i=0; i< (sizeof(DEVICE_ADDR_BOOK)/sizeof(char *));i++){ 
       if (i != SELF_NO) {   
         Wire.beginTransmission(DEVICE_ADDR_BOOK[i]);
         String data = "";
         data = String("b_")+ DEVICE_ADDR_BOOK[SELF_NO] +String("_")+ age; //b is for 'boradcast'
         Wire.write(data.c_str());
         Serial.print("Tell ");
         Serial.print(DEVICE_ADDR_BOOK[i]);
         Serial.print(" :");
         Serial.println (data.c_str());
         Wire.endTransmission(DEVICE_ADDR_BOOK[i]);
       }
      }
}

void updateLivinglist(String data){
  Serial.print("start updating with: ");
  Serial.println(data);
  int received_device_addr =  getValue(data,'_',1).toInt();
  int received_age = getValue(data,'_',2).toInt();
  living_list[received_device_addr] = received_age;
  //debug
  Serial.print("Updated Living List is :");
   for (int i=0; i<50;i++){
    Serial.print("[");
    Serial.print(i);
    Serial.print("]");
    Serial.print(living_list[i]);


   }  
   Serial.println("");
}


void updateSelfLivinglist(){
   living_list[DEVICE_ADDR_BOOK[SELF_NO]] = age;
   //debug vvvv
   Serial.print("Start update self... Updated Living List is :");
   for (int i=0; i<50;i++){
        Serial.print("[");
    Serial.print(i);
    Serial.print("]");
    Serial.print(living_list[i]);
   }  
   Serial.println("");
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
String getValue(String data, char separator, int index){
 int found = 0;
  int strIndex[] = {0, -1 };
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


