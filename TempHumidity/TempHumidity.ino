/*
 * Simple temperature & humidity sensor built with the HTU21D (from sparkfun.com)
 *
 * Arduino Pinouts
 *   A4 = SCA to HTU21D (CHECK TO SEE IF YOU NEED IN-LINE RESISTORS (see below))
 *   A5 = SCL to HTU21D (CHECK TO SEE IF YOU NEED IN-LINE RESISTORS (see below))
 *    6 = PWM control for red backlight LED brightness -> LCD16
 *    7 = LCD RS -> LCD4
 *    8 = LCD EN -> LCD6
 *    9 = LCD DB4 (data) -> LCD11
 *   10 = LCD DB5 (data) -> LCD12
 *   11 = LCD DB6 (data) -> LCD13
 *   12 = LCD DB7 (data) -> LCD14
 *
 * Additional LCD connections 
 *    LCD1 = GND
 *    LCD2 = VCC
 *    LCD3 = Contrast voltage (wiper of contrast pot)
 *    LCD5 = GND
 *    LCD15 = VCC
 *
 * Additional HTU21D connections.  Note that newer versions of the Sparkfun breakout
 * board have in-line pull-up resistors for SCA & SDL given the sensor itself is
 * a 3.3V part and most Arduinos have 5V I/O lines. Check the documentation  for your
 * HTU21D part or breakout board to be sure. If not you'll need pull-ups resistors on
 * SCA and SDL (4.7K works well).
 *
 *   VCC = 3.3V
 *   GND = GND
 */


#include <LiquidCrystal.h>
#include <Wire.h>
#include "SparkFunHTU21D.h"

//Create an instance of the HTU21D temperature/humidity sensor object
HTU21D Htu21d;

/* Define custom degree symbol */
/* The tool at http://www.quinapalus.com/hd44780udg.html is very useful for this... */
#define DEG_CHAR  1

byte degree[8] = {
  0b01100,    /* .##.. */
  0b10010,    /* #..#. */
  0b10010,    /* #..#. */
  0b01100,    /* .##.. */
  0b00000,    /* ..... */
  0b00000,    /* ..... */
  0b00000,    /* ..... */
  0b00000     /* ..... */
};

/*
 * Need to accumulate several pieces of data to calculate ongoing readings
 *
 * In a moving vehicle good values are a sample delay of 2 seconds and a capture
 * delay (reporting inverval) of 2 minutes.  At a stationary location such as a home
 * weather station good values are a sample delay of 5 seconds and a capture delay
 * of 5 minutes.
 *
 */
const int sampleDelay = 5;           /* Interval at which temperature sensor is read (in seconds) */
const int reportDelay = 1;          /* Interval at which samples are averaged & reported (in minutes) */
const unsigned long reportDelayMs = reportDelay * 60 * 1000L;  /* Calculation interval */
const unsigned long sampleDelayMs = sampleDelay * 1000L;       /* Sample delay interval */
unsigned long prevReportMs = 0;  /* Timestamp for measuring elapsed capture time */
unsigned long prevSampleMs  = 0;  /* Timestamp for measuring elapsed sample time */
unsigned int numSamples = 0;  /* Number of overall sensor readings over reporting interval */
unsigned int numReports = 0; /* Number of capture intervals observed */

float TempF = 0;        /* Temperature F reading over capture interval */
float CurrentTempF = 0; /* Current reading from temperature sensor */
float AvgTempF;         /* Average temperature as reported last capture interval */
float MinTempF = 199;   /* Observed minimum temperature */
float MaxTempF = -99;   /* Observed maximum temperature */

float Humidity = 0;     /* Humidity reading over capture interval */
float CurrentHum = 0;   /* Current reading from humidity sensor */
float AvgHum;           /* Average humidity as reported last capture interval */
float MinHum = 199;     /* Observed minimum humidity */
float MaxHum = -99;     /* Observed maximum humidity */

LiquidCrystal lcd(7,8,9,10,11,12);

int brightness = 100;
int redLEDPin = 6;

void setup() {
  
  analogWrite(redLEDPin,brightness);
  
  randomSeed(analogRead(0));

  // Create custom characters using LCD library
  lcd.createChar(DEG_CHAR,degree);

  lcd.begin(16,2);
  lcd.print("Sampling...");
  lcd.setCursor(0,1);
  lcd.print("please wait");
  delay(1500);

  // Initialize HTU21D
  Htu21d.begin();

  // Remember current clock time
  prevReportMs = prevSampleMs = millis();
}

void  loop() {

  unsigned long currentMillis = millis();    // Check current timer value
  
  /* See if it is time to read the sensors */
  if( (currentMillis - prevSampleMs) >= sampleDelayMs) {
    numSamples ++;  // Since we started at 0...

    read_sensors();
    TempF  += CurrentTempF;
    Humidity += CurrentHum;
    
    // Save sample time and redraw display to update
    prevSampleMs = currentMillis;
  }
  /* Now check and see if it is time to report averaged values */
  if( (currentMillis - prevReportMs) >= reportDelayMs) {

    AvgTempF = TempF / numSamples;
    if(AvgTempF > MaxTempF) MaxTempF = AvgTempF;
    if(AvgTempF < MinTempF) MinTempF = AvgTempF;
    
    AvgHum  =  Humidity / numSamples;
    if(AvgHum >  MaxHum) MaxHum = AvgHum;
    if(AvgHum < MinHum)  MinHum = AvgHum;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(AvgTempF,1);
    lcd.write(DEG_CHAR);      // Custom degree symbol
    lcd.print("  "); lcd.print(MinTempF,1); lcd.print("/"); lcd.print(MaxTempF,1);
    
    lcd.setCursor(0,1);
    lcd.print(AvgHum,1);   lcd.print("% ");
    lcd.print(" "); lcd.print(MinHum,1); lcd.print("/"); lcd.print(MaxHum,1);
    
    // Reset counters and accumulators
    prevReportMs = currentMillis;
    numSamples = 0;
    TempF = 0;
    Humidity = 0;
  }
}

void read_sensors() {
    long randNumber;
    randNumber = random(-80,80);
    // CurrentTempF = 58 + (((float)randNumber)/10);

    randNumber = random(-100,100);
    // CurrentHum = 50 + (((float)randNumber)/10);

    CurrentHum   = Htu21d.readHumidity();
    CurrentTempF = 32.0+((Htu21d.readTemperature()*9.0)/5.0);
}

  
