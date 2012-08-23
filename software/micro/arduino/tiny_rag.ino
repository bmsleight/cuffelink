/* 
 Traffic Light Cufflinks 
 (c) Brendan M. Sleight 2012
 This file is part of Cuffelink.com project
 
 Cuffelink is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Cuffelink is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Cuffelink.  If not, see <http://www.gnu.org/licenses/>.
 
 Used Watchdog Sleep Examples 
 * KHM 2008 / Lab3/  Martin Nawrath nawrath@khm.de
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne
 *
 * Modified on 5 Feb 2011 by InsideGadgets (www.insidegadgets.com)
 * to suit the ATtiny85 and removed the cbi( MCUCR,SE ) section 
 
 */

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Time.h>  


#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define RED_PIN 2
#define AMBER_PIN 1
#define GREEN_PIN 0
#define PIN_TOUCH 4

#define NUMBER_DELAY 500
#define MAX_LOOPS 2

volatile boolean f_wdt = 1;
int calibration = 0;
unsigned long t;
unsigned long remainderMills;
unsigned long oldMills;
unsigned long newMills;

void setup() {                
  cbi(ADCSRA,ADEN);  // switch Analog to Digitalconverter OFF
  // Let everyone know the power is on
  // initialize the pins as an output.
  // Hello World!
  ragPinsToOutput();
  // Traditional Flash test
  flashRAG();
 
  // Lets count to twelve
  // This gives us time to realease the cufflinks before calibration 
  // ... so th eback can be screwed in before calibration starts.
   for (int l = 0; l < 13; l++) {
     showNumber(l);
   }
  powerDown();   // Want calibration down with RAGs off

  // Get Calibration Value
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime();
    delay(20);
  }
  calibration = (calibration + 4) / 8;

  ragPinsToOutput();
  flashRAG();
  powerDown();
  setup_watchdog(8);
  //  t = 1343261200;
  t = 0;
}


void loop() {
  oldMills = millis();
  int addSeconds = 0;
  int touches = 0;

  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag

    // If touch sense then ... 
    if (touched())  { 
      ragPinsToOutput();
      touches = getTouches(1, 1, 6, MAX_LOOPS);

      switch (touches) {
        case 1:
          junctionRAG();
          break;
        case 2:
          pelicanRAG();
          break;
        case 3:  
          showTime();
          break;
        case 4:
          showDate();
          break;
        case 5:
          setLocalTime();
          break;
        case 6:
          setLocalDate();
          break;
      }
  
      powerDown();
    }

    system_sleep();

    // I slept for four thousand milliseconds
    // Add milliseconds it took to run above
    // psudeo code + fudgae factor to add.... 
    newMills = millis();
    remainderMills = remainderMills + (newMills-oldMills) + (4000);

    t = t + (remainderMills / 1000);
    remainderMills = remainderMills % 1000;

  }
}

void showTime()  {
  showNumber(hour(t)%12);
  delay(NUMBER_DELAY*2);
  showNumber(minute(t)/10);
  delay(NUMBER_DELAY*2);
  showNumber(minute(t)%10);
//        delay(NUMBER_DELAY*2);
//        showNumber(second(t)/10);
//        delay(NUMBER_DELAY*2);
//        showNumber(second(t)%10);
}

void showDate()  {
  showNumber(day(t)/10);
  delay(NUMBER_DELAY*2);
  showNumber(day(t)%10);
  delay(NUMBER_DELAY*2);
  showNumber(month(t));
  delay(NUMBER_DELAY*2);
  showNumber((year(t)%100)/10);
  delay(NUMBER_DELAY*2);
  showNumber((year(t)%100)%10);
}


void setLocalTime() {
  // setTime(int hr,int min,int sec,int day, int month, inti yr); 
  int hr, min, min_tens, min_units, d, m, y;
  d = day(t);
  m = month(t);
  y = year(t);

  signalSettingMode();
  hr = getTouches(hour(t)%12, 0, 11, MAX_LOOPS);
  signalSettingMode();
  min_tens = getTouches(minute(t)/10, 0, 5, MAX_LOOPS);
  signalSettingMode();
  min_units = getTouches(minute(t)%10, 0, 9, MAX_LOOPS);
  setTime(hr,(min_tens*10)+min_units,0,d,m,y);
  t = now();
}

void setLocalDate() {
  // setTime(int hr,int min,int sec,int day, int month, int yr); 
  int hr, mn, am_or_pm, d_tens, d_units, m, y_tens, y_units;
  hr = hour(hr)%12;
  mn = minute(t);

  signalSettingMode();
  am_or_pm = getTouches(hour(t)/12, 0, 1, MAX_LOOPS);
  hr = (hour(hr)%12) + (12 * am_or_pm);
  signalSettingMode();
  d_tens = getTouches(day(t)/10, 0, 3, MAX_LOOPS);
  signalSettingMode();
  d_units = getTouches(day(t)%10, 0, 9, MAX_LOOPS);
  signalSettingMode();
  m = getTouches(month(t), 1, 12, MAX_LOOPS);
  signalSettingMode();
  y_tens = getTouches((year(t)%100)/10, 0, 9, MAX_LOOPS);
  signalSettingMode();
  y_units = getTouches((year(t)%100)%10, 0, 9, MAX_LOOPS);
  signalSettingMode();

  setTime(hr,mn,0,(d_tens*10)+d_units,m,2000+(y_tens*10)+y_units);
  t = now();
}





byte getTouches(byte touches, byte min_touch, byte max_touch, byte max_loops) {
  showNumber(touches);
  if (!touched()) {
    return (touches);
  }
  else {
    if (touches < max_touch) {  // No overflow
      getTouches(touches + 1, min_touch, max_touch, max_loops);
    }
    else { 
      if (max_loops<=0) {
        // Heap overflow if we keep recursing.
        return (min_touch);
      }
      else {
        // Start from min_touch and count up again
        getTouches(min_touch, min_touch, max_touch, max_loops-1);
      }
    }
  }
}


void signalSettingMode() {
  rag(0,0,0,0);
  rag(0,0,1,100);
  rag(0,1,0,100);
  rag(1,0,0,200);
  rag(0,1,0,100);
  rag(0,0,1,200);
  rag(0,1,0,100);
  rag(1,0,0,100);
  rag(0,0,0,0);
  delay(NUMBER_DELAY*2);
}

void flashRAG() {
  rag(0,0,0,0);
  rag(1,0,0,1000);
  rag(0,0,0,0);
  rag(0,1,0,1000);
  rag(0,0,0,0);
  rag(0,0,1,1000);
  rag(0,0,0,0);
}

void junctionRAG() {
  rag(1,0,0,2000);
  rag(1,1,0,2000);
  rag(0,0,1,7000);
  rag(0,1,0,3000);
  rag(1,0,0,2000);  
}

void pelicanRAG()  {
  rag(1,0,0,2000);
  for (int l = 0; l < 4; l++) {
    rag(0,1,0,500);
    rag(0,0,0,500);
  }
  rag(0,0,1,7000);
  rag(0,1,0,3000);
  rag(1,0,0,2000);
}  


void rag(boolean red, boolean amber, boolean green, float period)  {
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


void powerDown()  {
  rag(0,0,0,0);
  // Save Power
  ragPinsToInput();
}


void ragPinsToInput()  {
  pinMode(RED_PIN, INPUT);
  pinMode(AMBER_PIN, INPUT);
  pinMode(GREEN_PIN, INPUT);
}

void ragPinsToOutput()  {
  pinMode(RED_PIN, OUTPUT);
  pinMode(AMBER_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

bool touched() {
  int new_charge = chargeTime(); 
  if (new_charge > calibration) {
    return true;
  }
  else {
    return false;
  }
}

byte chargeTime()
{
  byte mask = (1 << PIN_TOUCH);
  byte i;

  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on

  for (i = 0; i < 16; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
}

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  //  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out 
  //  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {

  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
}

