#include <PinChangeInterruptSimple.h>

/* 
 Traffic Light Cufflinks 
 (c) Brendan M. Sleight 2012
 This file is part of Cuffelink.com project

*/

#define RED_PIN 2
#define AMBER_PIN 1
#define GREEN_PIN 0
#define PUSH_B_PIN 5
#define RED_MAN_PIN 3
#define GREEN_MAN_PIN 4

volatile boolean push_button = false;

void setup() {                
  pinMode(RED_PIN, OUTPUT);
  pinMode(AMBER_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_MAN_PIN, OUTPUT);
  pinMode(GREEN_MAN_PIN, OUTPUT);
  pinMode(PUSH_B_PIN,INPUT);
  digitalWrite(PUSH_B_PIN, HIGH); // turn on pullup resistors
  attachPcInterrupt(PUSH_B_PIN,PUSH_ISR,LOW); 
}

void loop() {
  push_button = false;
  rag(1,0,0,0,1,2000);
  for (int l = 0; l < 2; l++) {
    rag(1,0,0,0,1,500);
    rag(1,0,0,0,0,500);
  }
  for (int l = 0; l < 2; l++) {
    rag(0,1,0,0,1,500);
    rag(0,0,0,0,0,500);
  }
  for (int l = 0; l < 2; l++) {
    rag(0,1,0,1,0,500);
    rag(0,0,0,1,0,500);
  }
  //Min Green time
  rag(0,0,1,1,0,6000); 
  while(push_button == false)  {
    rag(0,0,1,1,0,1000);
  }
  rag(0,0,1,1,0,1000); // Slight delay
  rag(0,1,0,1,0,3000); // leaving amber
  rag(1,0,0,1,0,2000); // 2 Sec all Red
  rag(1,0,0,0,1,7000); // Green Man
}

void PUSH_ISR() {
  push_button = true;
}



void rag(boolean red, boolean amber, boolean green, boolean rm, boolean gm, float period)  {
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

  if (rm) {
    digitalWrite(RED_MAN_PIN, HIGH);
  }
  else  {
    digitalWrite(RED_MAN_PIN, LOW);
  }

  if (gm) {
    digitalWrite(GREEN_MAN_PIN, HIGH);
  }
  else  {
    digitalWrite(GREEN_MAN_PIN, LOW);
  }

  delay((int)period);
}

