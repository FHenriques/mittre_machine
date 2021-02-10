#include "AccelStepper.h" // AccelStepper Library
#include <Keypad.h>  // Keypad Library
#include "U8glib.h"  // U8glib for Nokia LCD

// Variables to hold entered number on Keypad
#define SIZE_STR_MAX_DIGITS 4
String value_str;

String currentposition = ""; // Used for display on Nokia LCD


// Keypad Setup
const byte ROWS = 4; // Four Rows
const byte COLS = 4; // Four Columns
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; // Arduino pins connected to the row pins of the keypad
byte colPins[COLS] = {31, 33, 35, 37}; // Arduino pins connected to the column pins of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Keypad Library definition


// U8glib Setup for Nokia LCD
#define backlight_pin 11
U8GLIB_PCD8544 u8g(3, 4, 6, 5, 7); // Arduino pins connected to Nokia pins:
// CLK=3, DIN=4, CE=6, DC=5, RST=7


// AccelStepper Setup
AccelStepper stepper(1, A0, A1); // 1 = Easy Driver interface
// Arduino A0 connected to STEP pin of Easy Driver
// Arduino A1 connected to DIR pin of Easy Driver

void setup(void) {
    //  Light up the LCD backlight LEDS
    analogWrite(backlight_pin, 0); // Set the Backlight intensity (0=Bright, 255=Dim)

    //  AccelStepper speed and acceleration setup
    stepper.setMaxSpeed(1500); // Not to fast or you will have missed steps
    stepper.setAcceleration(400); //  Same here

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
    } while (u8g.nextPage());
}

void loop() {

    char keypressed = keypad.getKey(); // Get value of keypad button if pressed
    if (keypressed != NO_KEY) { // If keypad button pressed check which key it was
        switch (keypressed) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (value_str.length() < SIZE_STR_MAX_DIGITS) {
                    value_str.concat(keypressed);
                    drawnokiascreen(value_str);
                }
                break;

            case '*':
                if (value_str.length() > 0) {
                    value_str.remove(value_str.length() - 1);
                    drawnokiascreen(value_str);
                }
                break;
            case '#':
                if (value_str.length() > 0) {
                    movestepper(value_str.toInt());
                    while (value_str.length() != 0) {
                        value_str.remove(0);
                    }
                    drawnokiascreen(value_str);
                }
                break;
            default:
                break;
        }

    }
}

void movestepper(int z) { //  Move the stepper
    int calculatedmove = ((z * 1600) / 80); //  Calculate number of steps needed in mm
    Serial.print("Value MM: ");
    Serial.println(z);
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
        u8g.setPrintPos(57, 47);
        u8g.print(currentposition);
    } while (u8g.nextPage());
}

void drawnokiascreen(String y) {

    u8g.firstPage();
    do {
        u8g.drawHLine(0, 15, 84);
        u8g.drawVLine(50, 16, 38);
        u8g.drawHLine(0, 35, 84);
        u8g.setFont(u8g_font_profont11);
        u8g.drawStr(0, 10, "ENTER DISTANCE");
        u8g.setPrintPos(0, 29);
        u8g.print(y); // Put entered number on Nokia lcd    
        u8g.drawStr(62, 29, "MM");
        u8g.drawStr(4, 46, "cur-pos");
        u8g.setPrintPos(57, 47);
        u8g.print(currentposition); //  Display current position of stepper
    } while (u8g.nextPage());
}
