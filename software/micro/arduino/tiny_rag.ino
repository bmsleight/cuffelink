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
#include <TinyWireM.h>  

#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define RED_PIN 1
#define AMBER_PIN 4
#define GREEN_PIN 3
// Pin 0 SDA
// Pin 2 SCL
#define PIN_TOUCH 5

#define NUMBER_DELAY 500
#define MAX_LOOPS 2

volatile boolean f_wdt = 1;
int touch_calibration = 0;

struct {
  int second;
  int minute;
  int hour; //24 hour time
  int weekDay;
  int monthDay;
  int month;
  int year;
} rtcDate;


void setup() {                
  cbi(ADCSRA,ADEN);  // switch Analog to Digitalconverter OFF
  // Let everyone know the power is on
  // initialize the pins as an output.
  // Hello World!
  ragPinsToOutput();
  // Traditional Flash test
  flashRAG();
   // Lets count to twelve
  // CHANGE TO 13(12) FOR PRODUCTION 
  // This gives us time to realease the cufflinks before touch_calibration 
  // ... so th eback can be screwed in before touch_calibration starts.
   for (int l = 0; l < 6; l++) {
     showNumber(l);
   }
  powerDown();   // Want touch_calibration down with RAGs off

  // Get touch_calibration Value
  for (int i = 0; i < 8; i++) {
    touch_calibration += chargeTime();
    delay(20);
  }
  touch_calibration = (touch_calibration + 4) / 8;

  ragPinsToOutput();
  flashRAG();
  powerDown();
  
  TinyWireM.begin();

  setRtcTime(12, 9, 1, 12, 0); //MUST CONFIGURE IN FUNCTION 01-09-2012, 12:00
 // Sleep set-up
  setup_watchdog(8);
}


void loop() {
  int touches = 0;
  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag
    // If touch sensed then ... 
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
  }
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
  for (int l = 0; l < 2; l++) {
    rag(0,0,1,500);
    rag(0,0,0,500);
  }
  rag(0,0,1,5000);
  rag(0,1,0,3000);
  rag(1,0,0,2000);
}  

void showTime()  {
  getRtcTime();
  showNumber(rtcDate.hour%12);
  delay(NUMBER_DELAY*2);
  showNumber(rtcDate.minute/10);
  delay(NUMBER_DELAY*2);
  showNumber(rtcDate.minute%10);
}

void showDate()  {
  getRtcTime();
  showNumber(rtcDate.monthDay/10);
  delay(NUMBER_DELAY*2);
  showNumber(rtcDate.monthDay%10);
  delay(NUMBER_DELAY*2);
  showNumber(rtcDate.month);
  delay(NUMBER_DELAY*2);
  showNumber((rtcDate.year%100)/10);
  delay(NUMBER_DELAY*2);
  showNumber((rtcDate.year%100)%10);
}

void setLocalTime() {
  int hour, minute;
  getRtcTime();
  // Get value by touching
  signalSettingMode();
  hour = getTouches(rtcDate.hour%12, 0, 11, MAX_LOOPS);
  signalSettingMode();
  minute = 10 * getTouches(rtcDate.minute/10, 0, 5, MAX_LOOPS);
  signalSettingMode();
  minute = minute + getTouches(rtcDate.minute%10, 0, 9, MAX_LOOPS);
  // Update RTC
  setRtcTime(rtcDate.year, rtcDate.month, rtcDate.monthDay, hour, minute);
}

void setLocalDate() {
  // setTime(int hr,int min,int sec,int day, int month, int yr); 
  int hour, year, month, monthDay, am_or_pm;
  getRtcTime();
  // Get value by touching
  signalSettingMode();
  am_or_pm = getTouches(rtcDate.hour/12, 0, 1, MAX_LOOPS);
  hour = (rtcDate.hour%12) + (12 * am_or_pm);
  signalSettingMode();
  monthDay = getTouches(rtcDate.monthDay/10, 0, 3, MAX_LOOPS);
  signalSettingMode();
  monthDay = monthDay + getTouches(rtcDate.monthDay%10, 0, 9, MAX_LOOPS);
  signalSettingMode();
  month = getTouches(rtcDate.month, 1, 12, MAX_LOOPS);
  signalSettingMode();
  year = 10 * getTouches((rtcDate.year%100)/10, 0, 9, MAX_LOOPS);
  signalSettingMode();
  year = year + getTouches((rtcDate.year%100)%10, 0, 9, MAX_LOOPS);
  signalSettingMode();
  // Update RTC
//  setTime(rtcDate.year,mn,0,(d_tens*10)+d_units,m,2000+(y_tens*10)+y_units);
  setRtcTime(year + ((rtcDate.year/100) * 100), month, monthDay, rtcDate.hour, rtcDate.minute);

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

void getRtcTime() {
  TinyWireM.beginTransmission(DS1307_ADDRESS);
  TinyWireM.send(zero);
  TinyWireM.endTransmission();

  TinyWireM.requestFrom(DS1307_ADDRESS, 7);

  rtcDate.second = bcdToDec(TinyWireM.receive());
  rtcDate.minute = bcdToDec(TinyWireM.receive());
  rtcDate.hour = bcdToDec(TinyWireM.receive() & 0b111111); //24 hour time
  rtcDate.weekDay = bcdToDec(TinyWireM.receive()); //0-6 -> sunday - Saturday
  rtcDate.monthDay = bcdToDec(TinyWireM.receive());
  rtcDate.month = bcdToDec(TinyWireM.receive());
  rtcDate.year = bcdToDec(TinyWireM.receive());
}


void setRtcTime(byte year, byte month, byte monthDay, byte hour, byte minute) {
  TinyWireM.beginTransmission(DS1307_ADDRESS);
  TinyWireM.send(zero); //stop Oscillator
  TinyWireM.send(decToBcd(0)); // Always ^Dhalfway start through the minute ?
  TinyWireM.send(decToBcd(minute));
  TinyWireM.send(decToBcd(hour));
  TinyWireM.send(decToBcd(1)); // WeekDay not used
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
/*    if ((red_v<2) && (n!=0)) {
      rag(0,amber_v,green_v,(NUMBER_DELAY*(2-red_v)));
    }
    // A lack of LEDs will make it hard to work out 10 or 01 or 100  */
    if (n==0) {
      rag(1,1,1,(NUMBER_DELAY));
    }
    // Pause at the end so it is clear not 
    rag(0,0,0,(NUMBER_DELAY));
  }

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
  if (new_charge > touch_calibration) {
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

