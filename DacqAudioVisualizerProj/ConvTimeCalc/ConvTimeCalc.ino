int analogVal;
unsigned long newTime;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

void loop() {
   // put your main code here, to run repeatedly:
   newTime =micros();
   for(int i =0; i<10000;i++){
    analogVal=analogRead(A5);
    }
   float conversationtime = (micros()-newTime)/10000.0;

   Serial.print("Conversation Time : ");
   Serial.print(conversationtime);
   Serial.println("uS");

   Serial.print("Max Sampling freq : ");
   Serial.print((1.0/conversationtime)*1000000.0);
   Serial.println(" Hz");
}
