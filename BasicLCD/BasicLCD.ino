/*
 * Basic LCD display sketch
 *
 * Arduino Pinouts -- (without use of a shift register)
 *    
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
 */

#include <LiquidCrystal.h>
#include <Wire.h>

LiquidCrystal lcd(7,8,9,10,11,12);

int brightness = 100;
int redLEDPin = 6;

void setup() {
  
  analogWrite(redLEDPin,brightness);

  /* 16 characters by 2 lines
  lcd.begin(16,2);
  lcd.print("RGB 16x2 Display");
  lcd.setCursor(0,1);
  lcd.print(" Red 3.3V LCD");
  */

  /* 20 characters by  4 lines */
  lcd.begin(20,4);
  lcd.print("LCD 20x4  Display");
  lcd.setCursor(0,1);
  lcd.print("5V monochrome");
  lcd.setCursor(0,2);
  lcd.print("Backlit display");
  lcd.setCursor(0,3);
  lcd.print("Enjoy!!");
}

void loop() {
  int i;
  
  for(i=0;i<256;i++) {
    analogWrite(redLEDPin,i);
    delay(5);
  }
  for(i=255;i>=0;i--) {
    analogWrite(redLEDPin,i);
    delay(5);
  }
}
  
