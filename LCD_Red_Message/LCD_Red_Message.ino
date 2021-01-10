// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h> 
 
#define REDLITE 3
#define GREENLITE 5
#define BLUELITE 6
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
 
// you can change the overall brightness by range 0 -> 255
int brightness = 0;
int increment = 5;
 
void setup() {
  
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
}
 
void loop() {
  
  // Control brightness.  0 is max, 255 is min
  analogWrite(REDLITE,brightness);
  
  // Display the brightness setting
  lcd.setCursor(0,0);
  lcd.print("Brightness: ");
  lcd.print(brightness);
  lcd.print(" ");
 
  // Adjust brightness using 'increment'
  if(brightness == 0) {
    increment = 5;
  }
  else if(brightness == 255) {
    increment = -5;
  }
  brightness += increment;
  
  delay(1000);
}
