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
 

 Used Thermometer Example
 * http://www.instructables.com/id/Hidden-Arduino-Thermometer/ 

/ */

#include <avr/eeprom.h>
#include <EEPROM.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <TinyWireM.h>  

#define RV8564C2_WRITE_ADDRESS 81
#define RV8564C2_START_REGISTER_ADDRESS 0x03
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

#define EEPROM_START_OFFSET 0
#define EEPROM_START_STORE_NUMBER 128
#define EEPROM_START_STORE_NUMBER_MAX_LEN 20


volatile boolean f_wdt = 1;
int touch_calibration = 0;

struct {
  int minute;
  int hour; //24 hour time
  int monthDay;
  int month;
  int year;
} rtcDate;

struct settings_in_eeprom
{
  byte found_stored_settings; // Must store as byte as defualt value 0xFF, not 0x00 or 0x01
  int raw_temp_1;
  int calibrated_temp_1;
} settings_in_eeprom;

void setup() {                
  cbi(ADCSRA,ADEN);  // switch Analog to Digitalconverter OFF

  // Let everyone know the power is on
  // initialize the pins as an output.
  // Hello World!
  ragPinsToOutput();
  // Traditional Flash test
  flashRAG();
  // Load the setting from EEPROM
  eeprom_read_block((void*)&settings_in_eeprom, (void*)EEPROM_START_OFFSET, sizeof(settings_in_eeprom));
  if (settings_in_eeprom.found_stored_settings==255) {
      default_setting_in_eeprom();
  }

  // This gives us time to realease the cufflinks before touch_calibration 
  // ... so the back can be screwed in before touch_calibration starts.
   for (int l = 0; l < 11; l++) {
     showNumber(l);
   }
  powerDown();   // Want touch_calibration done with RAGs off

  // Get touch_calibration Value
  for (int i = 0; i < 8; i++) {
    touch_calibration += chargeTime();
    delay(20);
  }
  touch_calibration = (touch_calibration + 4) / 8;

  ragPinsToOutput();
  showNumber(0); // Visual clue to here comes RTC configuration
  TinyWireM.begin(); // Open up wire comms to RTC
  showNumber(setRtcTime(12, 10, 1, 12, 0)); //MUST CONFIGURE IN FUNCTION 01-10-2012, 12:00 
                                           // Also will report return code (see if rtc comms are okay)
  showNumber(0); // Visual clue to end of RTC configuration
  powerDown();

 // Sleep set-up
  setup_watchdog(8);
}


void loop() {
  byte touches = 0;
//  int second_touches = 0;
  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag
    // If touch sensed then ... 
    if (touched())  { 
      ragPinsToOutput();
      touches = getTouches(1, 1, 8, MAX_LOOPS);
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
          showTemp();
          break;
        case 6:
          showStoredNumber();
          break;
        case 7:
          if (getpasswd() == 321) {
            flashRAG(); // Nice sign that we have entered password correctly
//            second_touches = getTouches(1, 1, 5, MAX_LOOPS);
//            switch (second_touches) {
            touches = getTouches(1, 1, 5, MAX_LOOPS);
            switch (touches) {
              case 1:
                setLocalTime();
                break;
              case 2:
                setLocalDate();
                break;
              case 3:
                setRefTempReading();
                break;
              case 4:
                setRefTempValue();
                break;
              case 5:
                setStoredNumber();
                break;
            }
          }
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
  for (int l = 0; l < 6; l++) {
    rag(0,1,0,500);
    rag(0,0,0,500);
  }
  rag(0,0,1,6000);
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
  byte hour, minute;
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
  byte hour, year, month, monthDay, am_or_pm;
  getRtcTime();
  // Get value by touching
  signalSettingMode();
  am_or_pm = getTouches(rtcDate.hour/12, 0, 1, MAX_LOOPS);
  hour = (rtcDate.hour%12) + (12 * am_or_pm);
  signalSettingMode();
  monthDay = getTouches(rtcDate.monthDay/10, 0, 3, MAX_LOOPS);
  signalSettingMode();
  monthDay = (monthDay*10) + getTouches(rtcDate.monthDay%10, 0, 9, MAX_LOOPS);
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

void showTemp() {
  int t_result = getTempCalibrated(); 
  // Assuming this is less than 100 and is 0 or greater
  showNumber(t_result/10);
  delay(NUMBER_DELAY*2);
  showNumber(t_result%10);
}

int getTempCalibrated(){
  return getTemp() - settings_in_eeprom.raw_temp_1 + settings_in_eeprom.calibrated_temp_1;
}

int getTemp() {
  // http://www.instructables.com/id/Hidden-Arduino-Thermometer/   
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
  // Read temperature sensor against 1.1V reference
  ADMUX = _BV(REFS1) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
  delay(2); // Wait for ADMUX setting to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
  uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  return (high << 8) | low; // combine the two
}

void setRefTempReading() {
  delay(1000); // Settle down before reading temp
  settings_in_eeprom.raw_temp_1 = getTemp();
  signalSettingMode();
  write_setting_in_eeprom();
}

void setRefTempValue() {
  // Actually value is between 0 - 100, might as well use passwd function
  settings_in_eeprom.calibrated_temp_1 = getpasswd();
  signalSettingMode();
  write_setting_in_eeprom();
}

void showStoredNumber() {
// #define EEPROM_START_STORE_NUMBER 128
// #define EEPROM_START_STORE_NUMBER_MAX_LEN
//EEPROM.read(a)
  int address = EEPROM_START_STORE_NUMBER;
  while( (EEPROM.read(address) != 255) && (address <= EEPROM_START_STORE_NUMBER + EEPROM_START_STORE_NUMBER_MAX_LEN) ) {
    showNumber(EEPROM.read(address));
    delay(NUMBER_DELAY*2);
    address++;
  }
}

void setStoredNumber() {
  int address = EEPROM_START_STORE_NUMBER;
  byte digit = 0;
  while( (digit != 11) && (address <= EEPROM_START_STORE_NUMBER + EEPROM_START_STORE_NUMBER_MAX_LEN) ) {
    signalSettingMode();
    digit = EEPROM.read(address);
    if (digit = 255) {
      digit = 11; // End of number, 11 is entered
    }
    digit = getTouches(digit, 0, 11, MAX_LOOPS);
    delay(NUMBER_DELAY*2);
    if (digit == 11) {
      EEPROM.write(address, 255); // End of number, 11 is entered
    }
    else {
      EEPROM.write(address, digit); 
    }
    address++;
  }
}

int getpasswd() {
  int entered_pass = 0;
  signalSettingMode();
  entered_pass = 100 * getTouches(0, 0, 9, MAX_LOOPS);
  signalSettingMode();
  entered_pass = entered_pass + (10 * getTouches(0, 0, 9, MAX_LOOPS));
  signalSettingMode();
  entered_pass = entered_pass + getTouches(0, 0, 9, MAX_LOOPS);
  return entered_pass;
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

/*
Master sends-out the “Start Condition”.
Master sends-out the “Slave Address”, A2h for the RV-8564-C2; the R/W bit in write mode.
Acknowledgement from the RV-8564-C2.
Master sends-out the “Word Address” to the RV-8564-C2.
Acknowledgement from the RV-8564-C2.
Master sends-out the “Start Condition”. “Stop Condition” has not been sent.
Master sends-out the “Slave Address”, A3h for the RV-8564-C2; the R/W bit in read mode.
Acknowledgement from the RV-8564-C2.
At this point, the Master becomes a Receiver, the Slave becomes the Transmitter.
The Slave sends-out the “Data” from the Word Address specified in step 4).
Acknowledgement from the Master.
Steps 9) and 10) can be repeated if necessary. The address will be incremented automatically in the RV-8564-C2.
The Master, addressed as Receiver, can stop data transmission by not generating an acknowledge on the last byte
that has been sent from the Slave Transmitter. In this event, the Slave-Transmitter must leave the data line HIGH
to enable the Master to generate a stop condition.
Master sends-out the “Stop Condition”.
*/
  // Master sends-out the “Slave Address”, A2h for the RV-8564-C2; the R/W bit in write mode.
  // See datasheet fro explaination of 7/8 bits Wire Address.
  TinyWireM.beginTransmission(RV8564C2_WRITE_ADDRESS); 
  TinyWireM.send(RV8564C2_START_REGISTER_ADDRESS); // “Word Address” to the RV-8564-C2
  // Master sends-out the “Start Condition”. “Stop Condition” has not been sent.
  // Not sure from the datasheet if this command is to be send. But I need to send what is in the buffer.
  TinyWireM.endTransmission();
  // Master sends-out the “Slave Address”, A3h for the RV-8564-C2; the R/W bit in read mode.
  TinyWireM.requestFrom(RV8564C2_WRITE_ADDRESS, 6); // Request 7 bytes and send stop at the end.
  //  showNumber(TinyWireM.available()); // Should be 7
  //  delay(NUMBER_DELAY*4);
//  rtcDate.second = bcdToDec(TinyWireM.receive()   & 0b01111111); // & 0bXX to clear out any non-used bit - see datasheet
  rtcDate.minute = bcdToDec(TinyWireM.receive()   & 0b01111111);
  rtcDate.hour = bcdToDec(TinyWireM.receive()     & 0b00111111); //24 hour time
  rtcDate.monthDay = bcdToDec(TinyWireM.receive() & 0b00111111);
  byte weekDay = bcdToDec(TinyWireM.receive()  & 0b00000111); //0-6 -> sunday - Saturday // weekDay not Used - lack of memory
  rtcDate.month = bcdToDec(TinyWireM.receive()    & 0b00011111);
  rtcDate.year = bcdToDec(TinyWireM.receive());
}


byte setRtcTime(byte year, byte month, byte monthDay, byte hour, byte minute) {
  TinyWireM.beginTransmission(RV8564C2_WRITE_ADDRESS);
  TinyWireM.send(0); // “Word Address” to the RV-8564-C2, starting at Address 0
  TinyWireM.send(0); // Clear all flag, start the clock.
  TinyWireM.send(0); // Clear Control / Status 2
  TinyWireM.send(decToBcd(0));
  TinyWireM.send(decToBcd(minute));
  TinyWireM.send(decToBcd(hour));
  TinyWireM.send(decToBcd(monthDay));
  TinyWireM.send(decToBcd(1)); // WeekDay not used
  TinyWireM.send(decToBcd(month & 0b00011111 ) ); // Or century Bit set to 1, 21st Century boy.
  TinyWireM.send(decToBcd(year));
  return(TinyWireM.endTransmission()); // Return Success 0 or other code. 
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

void showNumber(byte n) {
  // I am never goes to change the powers
  // 10, 5, 0
  // max value is 19
  byte remainder = n;
  int green_v, amber_v, red_v; // Bizarre int uses less memory than byte
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
    // A lack of LEDs will make it hard to work out 10 or 01 or 100 
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


void default_setting_in_eeprom() {
  settings_in_eeprom.found_stored_settings = 0;
  settings_in_eeprom.raw_temp_1 = getTemp();
  settings_in_eeprom.calibrated_temp_1 = 20;
  signalSettingMode();
  write_setting_in_eeprom();
}

void write_setting_in_eeprom() {
  eeprom_write_block((const void*)&settings_in_eeprom, (void*)EEPROM_START_OFFSET, sizeof(settings_in_eeprom));
}
