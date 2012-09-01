//Arduino 1.0+ Only
//Arduino 1.0+ Only

#include <TinyWireM.h>  

#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527

#define RED_PIN 1
#define AMBER_PIN 4
#define GREEN_PIN 3
// Pin 0 SDA
// Pin 2 SCL

#define NUMBER_DELAY 500


void setup(){
  TinyWireM.begin();
  setDateTime(); //MUST CONFIGURE IN FUNCTION
  pinMode(RED_PIN, OUTPUT);
  pinMode(AMBER_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop(){
  printDate();
  delay(5000);
}

/*USAGE is modeled after the standard Wire library . . .
  Put in setup():
	TinyWireM.begin(){                               // initialize I2C lib
  To Send:
	TinyWireM.beginTransmission(uint8_t slaveAddr){  // setup slave's address (7 bit address - same as Wire)
	TinyWireM.send(uint8_t data){                    // buffer up bytes to send - can be called multiple times
	someByte = TinyWireM.endTransmission(){          // actually send the bytes in the buffer
	                                                 // returns (optional) 0 = sucess or see USI_TWI_Master.h for error codes
  To Receive:
	someByte = TinyWireM.requestFrom(uint8_t slaveAddr, uint8_t numBytes){      // reads 'numBytes' from slave's address
	                                                 // (usage optional) returns 0= success or see USI_TWI_Master.h for error codes
	someByte = TinyWireM.receive(){                  // returns the next byte in the received buffer - called multiple times
	someByte = TinyWireM.available(){                // returns the number of unread bytes in the received buffer
*/

void setDateTime(){

  byte second =      0; //0-59
  byte minute =      25; //0-59
  byte hour =        18; //0-23
  byte weekDay =     2; //1-7
  byte monthDay =    1; //1-31
  byte month =       8; //1-12
  byte year  =       12; //0-99

  TinyWireM.beginTransmission(DS1307_ADDRESS);
  TinyWireM.send(zero); //stop Oscillator

  TinyWireM.send(decToBcd(second));
  TinyWireM.send(decToBcd(minute));
  TinyWireM.send(decToBcd(hour));
  TinyWireM.send(decToBcd(weekDay));
  TinyWireM.send(decToBcd(monthDay));
  TinyWireM.send(decToBcd(month));
  TinyWireM.send(decToBcd(year));

  TinyWireM.send(zero); //start 

  TinyWireM.endTransmission();

}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void printDate(){

  // Reset the register pointer
  TinyWireM.beginTransmission(DS1307_ADDRESS);
  TinyWireM.send(zero);
  TinyWireM.endTransmission();

  TinyWireM.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(TinyWireM.receive());
  int minute = bcdToDec(TinyWireM.receive());
  int hour = bcdToDec(TinyWireM.receive() & 0b111111); //24 hour time
  int weekDay = bcdToDec(TinyWireM.receive()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(TinyWireM.receive());
  int month = bcdToDec(TinyWireM.receive());
  int year = bcdToDec(TinyWireM.receive());

  //print the date EG   3/1/11 23:59:59

/*
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
*/

  showNumber(hour%12);
  delay(NUMBER_DELAY*2);
  showNumber(minute/10);
  delay(NUMBER_DELAY*2);
  showNumber(minute%10);

}


void rag(boolean red, boolean amber, boolean green, unsigned int period)  {
  if (red) {
    digitalWrite(RED_PIN, HIGH);
  }
  else  {
    digitalWrite(RED_PIN, LOW);
  }    

  if (amber) {
    digitalWrite(AMBER_PIN, HIGH);
  }
  else  {
    digitalWrite(AMBER_PIN, LOW);
  }    

  if (green) {
    digitalWrite(GREEN_PIN, HIGH);
  }
  else  {
    digitalWrite(GREEN_PIN, LOW);
  }    

  delay((int)period);
}


void showNumber(int n) {
  // I am never goes to change the powers
  // 10, 5, 0
  // max value is 19
  int remainder = n;
  int green_v, amber_v, red_v;
  if (n<20) {
    green_v = n / 10;
    remainder = remainder % 10;
    amber_v = remainder / 5;
    red_v = remainder % 5;
    rag(0,amber_v,green_v,(NUMBER_DELAY*2));
    for (int l = 0; l < red_v; l++) {
      rag(1,amber_v,green_v,NUMBER_DELAY);
      rag(0,amber_v,green_v,NUMBER_DELAY);
    }
    if ((red_v<2) && (n!=0)) {
      rag(0,amber_v,green_v,(NUMBER_DELAY*(2-red_v)));
    }
    // A lack of LEDs will make it hard to work out 10 or 01 or 100  
    if (n==0) {
      rag(1,1,1,(NUMBER_DELAY));
    }
    // Pause at the end so it is clear not 
    rag(0,0,0,(NUMBER_DELAY));
  }

}

