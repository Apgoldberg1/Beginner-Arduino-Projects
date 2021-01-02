/*Test your visual reaction time andd utilize your Arduino skills
 * 
 * Uses Arduino UNO, resisitor, LED, button, and LCD screen
 * 
 * follow instruction here to connect lcd https://www.arduino.cc/en/Tutorial/LibraryExamples/HelloWorld
 * Wire GPIO18 to a resistor which connects to the negative side of an LED.  Wire the other side of the LED to digital pin 8
 * Wire 5V into a button, wire the other button into digital 10
*/
#include <LiquidCrystal.h> //LCD library

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);              //Set up the LCD screen
void scrollingtext(String displayText) {              //function which displays text in a scrolling form
  for (int i = 0; i <= displayText.length(); i++) {           //Iterate through each letter in th input text
    int toclear = (i+1) % 16;             //toclear is the square in front of the character being printed
    lcd.setCursor(toclear, 1);            //putting it in the spot we want to clear
    lcd.print("     ");       //blank space upcoming squares for readability
    int pos = i % 16;           //posititon where the charecter should be printed since there is 6 character spaces, also enables wrapping
    lcd.setCursor(pos, 1);
    lcd.print(displayText.substring(i,i+1));          //print next char
    delay(250);                             //delay before the next character is displayed
  }
   lcd.setCursor(0, 1);               
   lcd.print("                ");   //clear only the bottom line
}

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 8;     // the number of the pushbutton pin  
const int ledPin2 = 7;  // Reaction LED

// variables will change:
int i = 0;
int buttonState = 0;         // variable for reading the pushbutton status
unsigned long time;           //variable to store tie
int randomDelay;                //variable to determine time before light turns on
void setup() {
  lcd.begin(16,2);                  //initialize LCD having 16 columns 2 rows
  randomSeed(analogRead(0));            //randomize the seed by using an unnattached analog pin
  randomDelay = random(2000, 6000);         //set random number
  Serial.begin(9600);               //start monitor
  pinMode(buttonPin, INPUT);        //set buttonpin to input mode
  pinMode(ledPin2, OUTPUT);             //set ledPin2 to output mode
  digitalWrite(ledPin2, LOW);         //turn off led2
  time = millis();                //gets the time since the program started running
  delay(randomDelay);             //delay by a random amount of time
}

void loop() {
  if (i == 0) {                       //This allows the code to move on once the button's been pressed once
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);                   //Get state of the button
      if (buttonState == HIGH) {              //if the button is pressed
      time = millis();
      digitalWrite(ledPin2, LOW);             //When it's pressed turn the light off
      i++;            //flag to make sure the code moves on
    } else {
      digitalWrite(ledPin2, HIGH);            //if the button isn't pressed leave the light on
    }
  } else {              //once the button is pressed
     String message;              //initialize varaible which will store the message for the bottom line
     lcd.setCursor(7,0);            //center the cursor for the number
     int reactTime = time-randomDelay;                //reactTime is the time between the light and the button press
     //Select the message based on the reaction time
     if (reactTime < 150) {
      message = "Woah you're hot stuff!!! Wait a minute... YOU CHEATED!";
     }
     else if (reactTime < 200) {
      message = "NIIIIICE!! NIIIIIIIIIIIIICE!!!!! NIIIIIIIIIIIIIIIIIICE!!!!!!!";
     }
     else if (reactTime < 250) {
      message = "faster than the average time!";
     }
     else if (reactTime < 400) {
      message = "You can go quicker!";
     }
     else {
      message = "You'll get em next time";
     }
     lcd.print(reactTime);      //Prints the reaction time onto the LCD
     scrollingtext(message);     //Calls function to print message scrolling to the bottom LCD row
    }
  }
