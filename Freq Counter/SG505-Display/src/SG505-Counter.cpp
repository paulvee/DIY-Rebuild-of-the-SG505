/**************************************************************************
 Display the frequency from the SG505 oscillator on a 128x32 OLED display

 The output of the SG505 is connected to pin 5 of the arduino through
 a 100nF capacitor.

 The 0.9' 128x32 OLED display is connected to the I2C pins of the arduino.
 SCK to A5, SDA to A4, VCC to 5V, GND to GND.

 The frequency count is done using the FreqCount library from
 Paul Stoffregen.

 The Adafruit code is written by Limor Fried/Ladyada for Adafruit
 Industries, with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <FreqCount.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an Arduino Nano: A4(SDA), A5(SCL)

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long count = 0;

void setup() {
    // Serial.begin(9600); // for debugging
    FreqCount.begin(1000);  // also sets the delay between measurements to 1 second

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);  // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.setFont(&FreeMono12pt7b);
    display.setTextSize(1);               // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);  // Draw white text (yellow on my display)
    display.setCursor(0, 24);             // Start at left, centered vertically

    // welcome msg
    display.print("DIY SG505");
    display.display();
    delay(2000);
}

/*
 Helper function to format a number with dots every 3 digits
 to make the reading of larger numbers more easy.

*/
String formatNumberWithDots(unsigned long number) {
    String formattedNumber = "";
    String numberStr = String(number);
    int len = numberStr.length();

    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            formattedNumber += '.';
        }
        formattedNumber += numberStr[i];
    }

    return formattedNumber;
}

void loop() {
    if (FreqCount.available()) {
        count = FreqCount.read();
        // Serial.println(count); // for debugging
    }

    display.clearDisplay();

    // Format the count with dots and display it
    String formattedCount = formatNumberWithDots(count);
    display.setCursor(0, 24);  // Start at the begin, and center vertically
    display.print(formattedCount);
    display.setCursor(100, 24);  // add the unit at the far right
    display.print("Hz");

    display.display();  // show it on the display

    // count function has a delay of 1 second by default defined in setup()
}  // so we don't need a delay here
