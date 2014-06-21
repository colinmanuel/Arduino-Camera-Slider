/*
LCD screen for motorized camera dolly
Original Code by: Philip Sorri
Code Modified by: Colin Manuel
*/
#include <Wire.h> 
#include <AccelStepper.h>
#include <AFMotor.h>
#include <LiquidCrystal.h> //LCD library
#include <Bounce.h> //debounce library


// define some values used by the panel and buttons
int lcd_key = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT; 
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}

// did I adequately replace startStop?
// int startStop = 10;
int runTime1;
int show = 1;     //what menu to show
int dollySpeedNum = 0; //number of the speed
int delayTime = 100; //delay after pressedbutton (see the functions)
int delayDolly = 0; //delay before running 
int runTime = 0;    //time to run after delay
int motorPin = 3;
int motorState = 0;      // the current state of the output pin
int val4;
// shutter was set to 2 instead of 1 for some reason??
int shutter = 1;
int shutterCount = 0;
int shutterVal = 0;

/*int bbtnRIGHT = read_LCD_buttons(btnRIGHT);
int bbtnUP = read_LCD_buttons(btnUP);
int bbtnDOWN = read_LCD_buttons(btnDOWN);
int bbtnSELECT = read_LCD_buttons(btnSELECT);*/

/*KEY
  button1 - btnUP - increase
  button2 - btnDOWN - decrease
  button3 - btnSELECT - select
  startStop - btnLEFT - startStop*/

Bounce bouncer1 = Bounce(btnUP, 15); //deblounce (button, milliseconds)
Bounce bouncer2 = Bounce(btnDOWN, 15);
Bounce bouncer3 = Bounce(btnRIGHT, 15);
// Bounce bouncer4 = Bounce(btnLEFT, 15);
// replacing this with btnSELECT
Bounce bouncer4 = Bounce(btnSELECT, 15);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  // add serial definition - cm
  Serial.begin(9600);
  pinMode( adc_key_in, INPUT );         //ensure A0 is an input
  pinMode(motorPin, OUTPUT);
  pinMode(shutter, OUTPUT);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("DOLLYDUINO V 2.0");
  lcd.setCursor(0,1);
  lcd.print("By Colin");
  delay(1000);
}



// ####### added bounce from rfduino
/*  int debounce(int state)
  {
    int start = millis();
    int debounce_start = start;
    
    while (millis() - start < debounce_timeout)
      if (digitalRead(lcd_key) == state)
      {
        if (millis() - debounce_start >= debounce_time)
          return 1;
      }
      else 
        debounce_start = millis();

    return 0;
  }

  int delay_until_button(int state)
  {
    // set button edge to wake up on
    if (state)
      // RFduino_pinWake(button, HIGH);
    motorState = 1;

    else
      // RFduino_pinWake(button, LOW);
    motorState = 0;
      
  }*/


void loop() {
 // hiding lcd positioning as it wasn't here in original
 // lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 // lcd.print(millis()/1000);      // display seconds elapsed since power-up
 
 
 // lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons


/*int bbtnRIGHT = read_LCD_buttons(btnRIGHT);
int bbtnUP = read_LCD_buttons(btnUP);
int bbtnDOWN = read_LCD_buttons(btnDOWN);
int bbtnSELECT = read_LCD_buttons(btnSELECT);*/

//#############
// create switch functionality for btnSELECT

  if(motorState > 1) {
    motorState = 0;
  }

  if(lcd_key == btnSELECT) {
    motorState = motorState + 1;
      // delay(delayTime*3);
    delay(delayTime * 3);

  }
  // Serial.println(motorState);


//#############
  if(shutterVal > 1) {
    shutterVal = 0;
  }
  
  runTime1 = runTime;
  if(runTime == 0) {
    runTime1 = 10;
  }
  else {
    runTime1 = runTime;
  }


  // this is where it gets tricky
// how to replace bouncer reading/updating with my buttons???
  bouncer1.update();  //update the buttons
  bouncer2.update();
  bouncer3.update();
  bouncer4.update();
  int val1 = bouncer1.read(); //reads the value 
  int val2 = bouncer2.read(); //and sets the new variables to it
  int val3 = bouncer3.read(); //So if button is pressed value = HIGH
  val4 = bouncer4.read();
  

  if (show > 4) {              //Number of menus
    show = 1;
  }
  // if(val3 == HIGH) {    //scroll through the different menus
    /*KEY
  button1 - btnUP - increase
  button2 - btnDOWN - decrease
  button3 - btnSELECT - select
  startStop - btnLEFT - startStop*/
  if(lcd_key == btnRIGHT) {    //scroll through the different menus
    show = show + 1;    //the show number indicates which menu we're in
    delay(delayTime * 5);
  }
  if(show == 1) {      //if show is 1 the dollySpeed function is chosen/shown
    dollySpeed(val1, val2);  //To change values in this menu with same buttons
  }                          //in all the menus
  else if(show == 2) {  //if show is 2 delay function is chosen/shown on LCD
    dollyDelay(val1, val2);
  }
  else if(show == 3) { 
    runningTime(val1, val2);
  }
  else if(show == 4) { //shows all the setting that are chosen
    settings();
    if(lcd_key == btnUP || lcd_key == btnDOWN) {
      shutterVal = shutterVal + 1;
      delay(delayTime*3);
    }
  }


// create separate switch function to replace
// btnSELECT with HIGH and LOW
  // if(val4 == HIGH && shutterVal == 0){
  // if(lcd_key == btnSELECT && shutterVal == 0){
  if(motorState == 1 && shutterVal == 0){
    // Add code here to keep motor ON until turned OFF
    motorOn();
  }

    // ####### take another look here. adequate replacement?
    // if(lcd_key != btnSELECT){
    // if(val4 == LOW){
  if(motorState == 0){
    // Add code here to keep motor OFF until turned ON
      analogWrite(motorPin, LOW);
    }

  // if(val4 == HIGH && shutterVal == 1) {
  // if(lcd_key == btnSELECT && shutterVal == 1) {
// original location
  if(motorState == 1 && shutterVal == 1) {
      motorOn();
      lcd.setCursor(0,0);
      lcd.print("P: ");
      lcd.setCursor(3,0);
      lcd.print(shutterCount);
  }
}

void dollySpeed(int num1, int num2) { 
  lcd.setCursor(0,0);    //sets the startpoint of the text to (0,0)
  lcd.print("Dolly speed:    "); //Writes "Dolly speed:" to the LCD
  if(lcd_key == btnUP) { //if num1 which is val1/button1 is pressed the speed 
    dollySpeedNum = dollySpeedNum + 1;  //increases with 1
    delay(delayTime);      //delay before next press, otherwise it would
  }                         //scroll fast as hell
  if(lcd_key == btnDOWN) {        //if button2 is pressed decrease with 1
    dollySpeedNum = dollySpeedNum - 1;
    delay(delayTime);
  }
  lcd.setCursor(0,1);  //sets the starting point of the print
  lcd.print(dollySpeedNum); //prints to the start point (0,1)
  if(dollySpeedNum < 10) {
    lcd.setCursor(1, 1);
    lcd.print("%             "); //makes a percent sign after the number
  }                      //the space after it is to hide numbers that might
  if(dollySpeedNum >= 10 && dollySpeedNum < 100) { //be shown from the last
    lcd.setCursor(2, 1);            //menu that we scrolled from.
    lcd.print("%      "); //if the speed if over 10 and less than 100 it moves
  }                        //the percentsign one step to the right.
  if(dollySpeedNum == 100) { //same here
    lcd.setCursor(3, 1);
    lcd.print("%      ");
  }
  if(dollySpeedNum > 100) { //I wanted the speed to be in percent so I
    dollySpeedNum = 0;      //have to limit the value of dollySpeedNum.
  }                         //if the value is higher than 100 it makes it to 0
  if(dollySpeedNum < 0) {   //and if it's lower than 0 it goes to 100.
    dollySpeedNum = 100;
  }
}

void dollyDelay(int but1, int but2) {
  int decimal;
  decimal = delayDolly%10;
  lcd.setCursor(0,0);
  lcd.print("Delay:          ");
  // if(but1 == HIGH) {
  if(lcd_key == btnUP) {
    delayDolly = delayDolly + 1;
    delay(delayTime);
  }
  // if(but2 == HIGH) {
  if(lcd_key == btnDOWN) {
    delayDolly = delayDolly - 1;
    delay(delayTime);
  }
  lcd.setCursor(0,1);
  lcd.print(delayDolly/10);
  if(delayDolly < 10) {
    lcd.setCursor(1, 1);
    lcd.print(".");
    lcd.setCursor(2,1);
    lcd.print(decimal);
    lcd.setCursor(3,1);
    lcd.print(" s    ");
  }
  if(delayDolly >= 10 && delayDolly < 100) {
    lcd.setCursor(1, 1);
    lcd.print(".");
    lcd.setCursor(2,1);
    lcd.print(decimal);
    lcd.setCursor(3,1);
    lcd.print(" s    ");
  }
  if(delayDolly > 99) {
    lcd.setCursor(2, 1);
    lcd.print(".");
    lcd.setCursor(3,1);
    lcd.print(decimal);
    lcd.setCursor(4,1);
    lcd.print(" s    ");
  }
  if(delayDolly > 999) {
    delayDolly = 0;
  }
  if(delayDolly < 0) {
    delayDolly = 999;
  }
}

void runningTime(int num1, int num2) {
  int decimal1;
  decimal1 = runTime%10;
  lcd.setCursor(0,0);
  // why does run time not appear normally????
  lcd.print("Run time:       ");
  if(lcd_key == btnUP) {
    runTime = runTime + 1;
    delay(delayTime);
  }
  if(lcd_key == btnDOWN) {
    runTime = runTime - 1;
    delay(delayTime);
  }
  lcd.setCursor(0,1);
  lcd.print(runTime/10);
  if(runTime < 10) {
    lcd.setCursor(1, 1);
    lcd.print(".");
    lcd.setCursor(2,1);
    lcd.print(decimal1);
    lcd.setCursor(3,1);
    lcd.print(" s    ");
  }
  if(runTime >= 10 && runTime < 100) {
    lcd.setCursor(1, 1);
    lcd.print(".");
    lcd.setCursor(2,1);
    lcd.print(decimal1);
    lcd.setCursor(3,1);
    lcd.print(" s    ");
  }
  if(runTime > 99) {
    lcd.setCursor(2, 1);
    lcd.print(".");
    lcd.setCursor(3,1);
    lcd.print(decimal1);
    lcd.setCursor(4,1);
    lcd.print(" s    ");
  }
  if(runTime > 999) {
    runTime = 0;
  }
  if(runTime < 0) {
    runTime = 999;
  }
}



void settings() {
  int decimal1;
  int decimal2;
  decimal1 = runTime%10;
  decimal2 = delayTime%10;
  lcd.setCursor(0,0);
  lcd.print("S: ");
  lcd.setCursor(3,0);
  lcd.print(dollySpeedNum);
  if(dollySpeedNum < 10) {
    lcd.setCursor(4,0);
    lcd.print("%   ");
  }
  if(dollySpeedNum >= 10 && dollySpeedNum < 100) {
    lcd.setCursor(5,0);
    lcd.print("%  ");
  }
  if(dollySpeedNum == 100) {
    lcd.setCursor(6,0);
    lcd.print("% ");
  }
  
  lcd.setCursor(8,0);
  lcd.print("R: ");
  lcd.setCursor(11,0);
  lcd.print(runTime/10);
  if(runTime < 10) {
    lcd.setCursor(12, 0);
    lcd.print(".");
    lcd.setCursor(13,0);
    lcd.print(decimal1);
    lcd.setCursor(14,0);
    lcd.print("s");
  }
  if(runTime >= 10 && runTime < 100) {
    lcd.setCursor(12,0);
    lcd.print(".");
    lcd.setCursor(13,0);
    lcd.print(decimal1);
    lcd.setCursor(14,0);
    lcd.print("s");
  }
  if(runTime > 99) {
    lcd.setCursor(13,0);
    lcd.print(".");
    lcd.setCursor(14,0);
    lcd.print(decimal1);
    lcd.setCursor(15,0);
    lcd.print("s");
}

lcd.setCursor(0,1);
  lcd.print("D: ");
  lcd.setCursor(3,1);
  lcd.print(delayDolly/10);
  if(delayDolly < 10) {
    lcd.setCursor(4, 1);
    lcd.print(".");
    lcd.setCursor(5,1);
    lcd.print(decimal1);
    lcd.setCursor(6,1);
    lcd.print("s");
  }
  if(delayDolly >= 10 && delayDolly < 100) {
    lcd.setCursor(4,1);
    lcd.print(".");
    lcd.setCursor(5,1);
    lcd.print(decimal2);
    lcd.setCursor(6,1);
    lcd.print("s");
  }
  if(delayDolly > 99) {
    lcd.setCursor(5, 1);
    lcd.print(".");
    lcd.setCursor(6,1);
    lcd.print(decimal1);
    lcd.setCursor(7,1);
    lcd.print("s");
}

/*  if(lcd_key != btnSELECT) {
  if(val4 == LOW) {*/
  if(motorState == 0) {
    if(shutterVal == 0) {
      lcd.setCursor(10,1);
      lcd.print("I=OFF");
    }
    if(shutterVal == 1) {
      lcd.setCursor(10,1);
      lcd.print("I=ON ");
      }
    }
  }

void motorOn() {
  int speedValue = map(dollySpeedNum, 0, 100, 0, 255);
    // delay(1);    
    // delay(3000);    
    delay(delayDolly*100);

    analogWrite(motorPin, speedValue);
    Serial.println(runTime1);
    delay(runTime1*100);

    // delay(delayDolly*100);    
    if(delayDolly > 0) {
      // turn the motor off if the delayDolly variable is greater than 0
      analogWrite(motorPin, LOW);
      // Serial.println(delayDolly);
      // Serial.println(shutterCount);
      // delay(delayDolly*100);    
      delay(1000);    
      picture();
      shutterCount = shutterCount + 1;
    }
    // delay(delayDolly*100);

}

void picture() {
  pinMode(shutter, LOW);
  delay(50);
  pinMode(shutter, HIGH);
  
}
