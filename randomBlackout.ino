/* 

  randomBlackout – 1.0.0
 
  Prompts the user to turn off a random kitchen appliance using
  an Arduino Uno hooked up to an LCD display and a buzzer.
  All in the grand name of science.

  The circuit: 
  * Arduino Uno
  * 2x16 LCD Display
  * Potentiometer to control display contrast
  * Piezo Speaker

  Author: 
  Group A, Interaction Design Master at Malmö University
  Introduction to Multidisciplinary Interaction Design
  October 2019

  License: 
  GPL-3 - https://www.gnu.org/licenses/gpl-3.0.en.html 

*/

#include <LiquidCrystal.h>
#include <Wire.h>


/*************/
/* CONSTANTS */
/*************/


const String appliances[] = { "WATER BOILER", "STOVE", "TOASTER", "LIGHTS",  "MICROWAVE" }; // the array of appliances to display
const int applianceCount  = sizeof(appliances) / sizeof(appliances[0]);  // get the length of the array

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialize the display
const int buzzer = 8;                  // initialize the buzzer

const unsigned long startupTime   = 120000; // time the device waits until it starts randomly blacking out
const unsigned long applianceTime = 60000;  // time each appliance is turned off

int runsLeft = 16;  // the amounts times we get new appliance


/******************/
/* MAIN FUNCTIONS */
/******************/


void setup() {
  /* "setup()" is like the start-block. This code only gets executed once. 
    We use it to set up things and to print the intro message. */

  // this is to define a new character for the LCD to print
  // each byte represents a dot on the matrix, 1=filled 0=empty
  // in our case, we want to fill all the dots
  byte blockChar[] = {B11111,
                      B11111,
                      B11111,
                      B11111,
                      B11111,
                      B11111,
                      B11111,
                      B11111};

  lcd.begin(16, 2);  // start up the LCD
  lcd.createChar(0, blockChar);  // add the block char
  pinMode(buzzer, OUTPUT);  // make sure we can talk to the buzzer

  displayMessage("Research Probe",
                 "Group A - IDM");

  delay(startupTime);
}

void loop() {
  /* "loop()" is like the forever block, ie an infiniate loop. 
    The Arudino will execute this until the end of time or it runs
    out of power, whichever comes first. */
  while (runsLeft) {
    alertUser(1000);  // horrible buzzer
    int appliance = nextAppliance(appliance);  // get a new appliance
    displayAppliance(appliances[appliance]);  // display the appliance to the screen
    displayEnergyLevel(runsLeft);
  
    delay(applianceTime);
    runsLeft--;
  }

  alertUser(2000);  // horrible buzzer
  displayMessage("Well done!",
                 "Enjoy yr fika :)");
  delay(999990);
}


/********************/
/* HELPER FUNCTIONS */
/********************/


void alertUser(int htz) {
  /* Rings the buzzer with a 1000hz tone */
  noTone(buzzer);
  tone(buzzer, htz);
  delay(1000);
  noTone(buzzer);
}

void displayMessage(String lineOne, String lineTwo) {
  /* Displays a message to the screen, goes line by line */
  lcd.clear();
  lcd.setCursor(0, 0);  // place the "cursor" of the screen on the first line
  lcd.print(lineOne);
  lcd.setCursor(0, 1);  // jump down to the second line
  lcd.print(lineTwo);
}

void displayAppliance(String appliance) {
  /* Displays an appliance to the screen, just a wrapper fn that only prints the first line */
  displayMessage(appliance, "");
}

void displayEnergyLevel(int powerLeft) {
  /* Displays the energy left, has to be it's own fn as we are using
    custom chars that require a special way of printing */
  lcd.setCursor(0, 1);  // make sure we are on the second line
  for (int i = 0; i != powerLeft; ++i) lcd.write(byte(0));
}

int nextAppliance(int previous) {
  /* Randomly selects an int that we use to get the next appliance.
    To make sure there are no repeats, we compare the next int
    to the previous one. If same, get a new random int until they diff. */
  int next;
  do next = random(0, applianceCount); while (next == previous);  // get a random new appliance. try until its not eq to previous
  return next;
}
