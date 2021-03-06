int SER_Pin = 11;   //pin 14 on the 75HC595
int RCLK_Pin = 8;   //pin 12 on the 75HC595
int SRCLK_Pin = 12; //pin 11 on the 75HC595
int OE_Pin = 3;     //pin 13 on the 75HC595

//How many of the shift registers - change this
#define number_of_74hc595s 14

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

void setup(){
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);
  pinMode(OE_Pin, OUTPUT);
  Serial.begin(115200);
  //Serial.println("Ready");       

  //reset all register pins
  clearRegisters();
  writeRegisters();
}               

//set all register pins to LOW
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
  Serial.flush(); //Flushes the transmit buffer
  Serial.read(); //Flushes the recieve buffer
} 

//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}

//adjusts brightness of all LEDs
void setBrightness(byte brightness) // 0 to 255
{
  analogWrite(OE_Pin,255-brightness);
}


void loop() {
  setBrightness(50); // Change this to change the brightness of the LEDs (50)
  if (Serial.available() > 0) {
    // Read the matlab integer
    int incomingMat = Serial.read();
    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(incomingMat, DEC);
    if (incomingMat == 0) {
      clearRegisters();
    }
    else if (incomingMat > 120) { //abritrarly picked that number as it needs to be more than LED pins but 103 less than 255 because of uint8 function
      setRegisterPin(incomingMat - 121, LOW);
      writeRegisters();
    }     
    else {
      setRegisterPin(incomingMat - 1, HIGH);
      writeRegisters();
    }
  }
}
