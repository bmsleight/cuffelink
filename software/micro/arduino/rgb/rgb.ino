/* 
 Traffic Light Cufflinks 
 (c) Brendan M. Sleight 2012
 This file is part of Cuffelink.com project

*/

#define RED_PIN 2
#define BLUE_PIN 1
#define GREEN_PIN 0
// RGB - but keep the red as one unit and Green as 10, make Blue=5

#define MAX_RESOLUTION 64
#define MAX_RGB_LEVELS 4
#define MAC_BRIGHTNESS_LEVELS 7

void setup() {                
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
/*  delay(1000);
  light(3,3,3, 250, 1);
  light(3,3,3, 250, 2);
  light(3,3,3, 250, 3);
  light(3,3,3, 250, 4);
  light(3,3,3, 250, 5);
  light(3,3,3, 250, 6);
  light(3,3,3, 4000, 7);
  light(3,3,3, 500, 5);
  light(3,3,3, 250, 2);
  light(3,3,3, 250, 1);
  */
  delay(1000);
  fade(3,3,3, 4000 );
  delay(3000);
  fade(3,3,0, 1000 );
  delay(3000);

}


void fade(byte r, byte g, byte b, int wait) {
  light(r,g,b, wait*1/10, 1);
  light(r,g,b, wait*1/10, 3);
  light(r,g,b, wait*1/10, 5);
  light(r,g,b, wait*7/10, 7);
  light(r,g,b, wait*1/10, 5);
  light(r,g,b, wait*1/10, 2);
  light(r,g,b, wait*1/10, 1);
}


void light(byte r, byte g, byte b, int wait, byte bright) {
  unsigned long microseconds = millis();
  byte strobe_counter = 0;
  bright = 1 << bright;
  while((microseconds + wait)>millis()) {
    if((r*bright)>=strobe_counter) {
      digitalWrite(RED_PIN, HIGH);
    }
    else {
      digitalWrite(RED_PIN, LOW);
    }
    if((g*bright)>=strobe_counter) {
      digitalWrite(GREEN_PIN, HIGH);
    }
     else {
      digitalWrite(GREEN_PIN, LOW);
    }
    if((b*bright)>=strobe_counter) {
      digitalWrite(BLUE_PIN, HIGH);
    }
    else {
      digitalWrite(BLUE_PIN, LOW);
    }
    strobe_counter++;
    // 64 * 4 = 256
    if(strobe_counter>=MAX_RESOLUTION*MAX_RGB_LEVELS) {
      strobe_counter=0;
    }
//    delay(10);
  }
  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}


