int  arr[10];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  arr[9] = 99;
  for (int i=0;i<10;i++){
    Serial.println(arr[i]);  
  }

  String str = "c_10_77";

  for (int i =0; i<=3; i++){
     String r = getValue(str,'_',i);
    Serial.println(r);
  }

  
}

void loop() {
  // put your main code here, to run repeatedly:

}

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
