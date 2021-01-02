/*This a minigame where the active light moves back and forth across a row of lights in order.  
 * When it reaches the end it travels back in the opposite direction.
 * When a button is pressed the light stops and if the light stops in the middle the level increases also increasing the speed.
 * if the button is pressed one away from the middle the speed and level stays the same
 * if the button is pressed and it lands on a light which isn't the middle or next to the middle the level and speed is lowered.
 * 
 * Digital ports 2-13 are eached wired directly to the positive end of LEDs.
 * The other end of the LED is in ground.
 * The button has 5V in and a second wire in inputting to A)
 * 
 * The Matrix screen is wired:
 * 5v to vcc
 * GND to GND
 * A4 to SDA
 * A5 to SCL
 * 
 * At a negative level a sad face is displayed and at levels > 10 a happy face is displayed.
 */





#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

static const uint8_t PROGMEM    // Setting Static Constants for the LED Matrix
  Sad_bmp[] =                    // Declaring happy face
  { B00000000,
    B01100110,
    B01100110,
    B00000000,
    B00111100,
    B01000010,
    B01000010,
    B00000000
 },
  Happy_bmp[] =     //declaring sad face
 {
  B00000000,
  B01100110,
  B01100110,
  B00000000,
  B01000010,
  B01000010,
  B00111100,
  B00000000
 };

const int buttonPin = 13;     //Where the button inputs
int i = 2;      //The port number which the first LED is hooked on
int dirflag = 0;      //flag used to determine the direction of the LED
int lightRate = 125;      //rate in ms for how long each light stays on
int level = 0;     //stores what level the player is on
boolean buttonUp = true;      //flage which ensures the button isn't being held
void setup() {
  matrix.setTextSize(1);                          // Setting matrix text size to 1
  matrix.setTextWrap(true);                     // Preventing text wrapping to scroll text continuously through matrix
  matrix.setTextColor(LED_ON);                  // Turning LED On
  matrix.setRotation(1);                  // rotate
  matrix.begin(0x70);  // pass in the address
  matrix.setCursor(random(0,4),random(0,2));                   // Where the numbers should be printed, can't center so why not have them jump around!!!
  matrix.clear();
  matrix.print(level);            //start with 0 on screen
  matrix.writeDisplay();
  Serial.begin(9600);
  //Initialize LED pins
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 
  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  pinMode(12, OUTPUT); 
  //initialize pin attatched to button
  pinMode(buttonPin, INPUT); 
}
void loop() {
  int buttonState = digitalRead(buttonPin);       //get the state of the input when button pressed == HIGH when not pressed == LOW
  if (buttonState == HIGH) {              //if the button is pressed
    if (buttonUp) {                 //ensure the button is not being held to repeat the loop
      digitalWrite(i, HIGH);          //light up on the light where the button was pressed
      if (i == 7) {             //if stopped on the middle light
        lightRate /= 1.2;       //speed up
        level++;                //increment level
        matrix.setCursor(random(0,4),random(0,2));                  
        matrix.clear();               //matrix must be clear before printing
        if (level<0) {
          matrix.drawBitmap(0, 0, Sad_bmp, 8, 8, LED_ON);      // Drawing the sad face when the level is negative
        }
        else if (level>9) {           //draw a happy face if the level is 2 digits
          matrix.drawBitmap(0, 0, Happy_bmp, 8, 8, LED_ON);
        }
        else {
          matrix.print(level);            //display the current level
        }
        matrix.writeDisplay();
      }
      else if (i != 6 & i !=8) {        //if the stop is not on a green light (middle light was checked, making sure it's not the two lights next to that)
        lightRate *= 1.2;       //slow down
        level --;         //decrease the level
         matrix.setCursor(random(0,4),random(0,2));
         matrix.clear();
         if (level<0) {
          matrix.drawBitmap(0, 0, Sad_bmp, 8, 8, LED_ON); 
        }
         else if (level>9) {
          matrix.drawBitmap(0, 0, Happy_bmp, 8, 8, LED_ON);
         }
        else {
          matrix.print(level);
        }
         matrix.writeDisplay();
      }
    }
      delay(2000);        //stay with the stopped light for 2 seconds
      buttonUp = false;         //set the flag to false so if the button is held the loop isn't repeated
    }

  else {                    //when the button is up
    buttonUp = true;            //reset the flag so the button can be pressed to stop again
    digitalWrite(i, HIGH);            //light the current light
    delay(lightRate);                 //wait
    digitalWrite(i, LOW);             //turn off light
    if (i == 2){            //if the led being lit is on the end change the direction flag
      dirflag = 1;
    }
    else if (i == 12){
      dirflag = 0;
    }
    if (dirflag == 0) {         //Increment based on the direction the led is going
      i--;
    }
    else {
      i++;
    }   
  } 
}
