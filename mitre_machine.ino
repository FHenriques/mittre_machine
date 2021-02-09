#include "AccelStepper.h" // AccelStepper Library
#include <Keypad.h>  // Keypad Library
#include "U8glib.h"  // U8glib for Nokia LCD

// Variables to hold entered number on Keypad
volatile int firstnumber=99;  // used to tell how many numbers were entered on keypad
volatile int secondnumber=99;
volatile int thirdnumber=99;



// Variables to hold Distance and CurrentPosition
int keyfullnumber=0;  // used to store the final calculated distance value
String currentposition = "";  // Used for display on Nokia LCD


// Keypad Setup
const byte ROWS = 4; // Four Rows
const byte COLS = 4; // Four Columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; // Arduino pins connected to the row pins of the keypad
byte colPins[COLS] = {31, 33, 35, 37}; // Arduino pins connected to the column pins of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );  // Keypad Library definition


// U8glib Setup for Nokia LCD
#define backlight_pin 11
U8GLIB_PCD8544 u8g(3, 4, 6, 5, 7);  // Arduino pins connected to Nokia pins:
                                    // CLK=3, DIN=4, CE=6, DC=5, RST=7
                                    
                                    
// AccelStepper Setup
AccelStepper stepper(1, A0, A1);  // 1 = Easy Driver interface
                                  // Arduino A0 connected to STEP pin of Easy Driver
                                  // Arduino A1 connected to DIR pin of Easy Driver
                                  


void setup(void) {
  
  //  Light up the LCD backlight LEDS
  analogWrite(backlight_pin, 0);  // Set the Backlight intensity (0=Bright, 255=Dim)
    
  //  AccelStepper speed and acceleration setup
  stepper.setMaxSpeed(1500);  // Not to fast or you will have missed steps
  stepper.setAcceleration(400);  //  Same here
  
  // Draw starting screen on Nokia LCD
  u8g.firstPage();
  do {
  u8g.drawHLine(0, 15, 84);
  u8g.drawVLine(50, 16, 38);
  u8g.drawHLine(0, 35, 84); 
  u8g.setFont(u8g_font_profont11);
  u8g.drawStr(0, 10, "ENTER DISTANCE");
  u8g.drawStr(62, 29, "MM");
  u8g.drawStr(4, 46, "cur-pos");
  }
  while( u8g.nextPage() );
  
}


void loop(){
  
  char keypressed = keypad.getKey();  // Get value of keypad button if pressed
  if (keypressed != NO_KEY){  // If keypad button pressed check which key it was
    switch (keypressed) {
      
      case '1':
        checknumber(1);
      break;
        
      case '2':
        checknumber(2);
      break;

      case '3':
        checknumber(3);
      break;

      case '4':
        checknumber(4);
      break;

      case '5':
        checknumber(5);
      break;

      case '6':
        checknumber(6);
      break;

      case '7':
        checknumber(7);
      break;

      case '8':
        checknumber(8);
      break;

      case '9':
        checknumber(9);
      break;

      case '0':
        checknumber(0);
      break;

      case '*':
        deletenumber();
      break;

      case '#':
        calculatedistance();
      break;
    }
  }

}

void checknumber(int x){
  if (firstnumber == 99) { // Check if this is the first number entered
    firstnumber=x;
    String displayvalue = String(firstnumber);  //  Transform int to a string for display
    drawnokiascreen(displayvalue); // Redraw Nokia lcd
    
  } else {
    if (secondnumber == 99) {  // Check if it's the second number entered
      secondnumber=x;
      String displayvalue = (String(firstnumber) + String(secondnumber));
      drawnokiascreen(displayvalue);

    } else {  // It must be the 3rd number entered
      thirdnumber=x;
      String displayvalue = (String(firstnumber) + String(secondnumber) + String(thirdnumber));
      drawnokiascreen(displayvalue);

    }
  }
}

 
void deletenumber() {  // Used to backspace entered numbers
  if (thirdnumber !=99) {
      String displayvalue = (String(firstnumber) + String(secondnumber));
      drawnokiascreen(displayvalue);

    thirdnumber=99;
  } 
  else {
    if (secondnumber !=99) {
      String displayvalue = String(firstnumber);
      drawnokiascreen(displayvalue);

      secondnumber=99;
   } 
   else {
     if (firstnumber !=99) {
       String displayvalue = "";
       drawnokiascreen(displayvalue);

       firstnumber=99;
      }
    }
  }
}
  
void calculatedistance() {  // Used to create a full number from entered numbers

    if (thirdnumber == 99 && secondnumber == 99 && firstnumber != 99) {
      keyfullnumber=firstnumber;
      movestepper(keyfullnumber);
    }
    
    if (secondnumber != 99 && thirdnumber == 99) {
      keyfullnumber=(firstnumber*10)+secondnumber;
      movestepper(keyfullnumber);
    }
    
    if (thirdnumber != 99) {
      keyfullnumber=(firstnumber*100)+(secondnumber*10)+thirdnumber;
      movestepper(keyfullnumber);
    }
    
    resetnumbers(); // Reset numbers to get ready for new entry
  } 


void movestepper(int z) {  //  Move the stepper

  int calculatedmove=((z*1600)/80);  //  Calculate number of steps needed in mm
  stepper.runToNewPosition(calculatedmove);
  currentposition = String(z);
  u8g.firstPage();
  do {
    u8g.drawHLine(0, 15, 84);
    u8g.drawVLine(50, 16, 38);
    u8g.drawHLine(0, 35, 84); 
    u8g.setFont(u8g_font_profont11);
    u8g.drawStr(0, 10, "ENTER DISTANCE");
    u8g.drawStr(62, 29, "MM");
    u8g.drawStr(4, 46, "cur-pos");
    u8g.setPrintPos(57,47);
    u8g.print(currentposition);       
  }
  while( u8g.nextPage() ); 
}
                
void resetnumbers() {  // Reset numbers for next entry
  firstnumber=99;
  secondnumber=99;
  thirdnumber=99;
} 
  

void drawnokiascreen(String y) {

    u8g.firstPage();
    do {
      u8g.drawHLine(0, 15, 84);
      u8g.drawVLine(50, 16, 38);
      u8g.drawHLine(0, 35, 84); 
      u8g.setFont(u8g_font_profont11);
      u8g.drawStr(0, 10, "ENTER DISTANCE");
      u8g.setPrintPos(0,29);
      u8g.print(y);  // Put entered number on Nokia lcd    
      u8g.drawStr(62, 29, "MM");
      u8g.drawStr(4, 46, "cur-pos");
      u8g.setPrintPos(57,47); 
      u8g.print(currentposition); ) //  Display current position of stepper}