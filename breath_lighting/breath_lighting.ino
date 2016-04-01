void setup() {

   pinMode(5,OUTPUT);
  
}

void loop() {
  for (int i=0; i<=255; i++)
  {
    analogWrite(5,i);
    delay(14);
  }
  
  for (int i=255; i>=0; i--)
  {
    analogWrite(5,i);
    delay(14);
  }
  delay(800);

}     
