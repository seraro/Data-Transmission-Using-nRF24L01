#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void loop() {
    if (radio.available()) {
        char text[32] = "";
        radio.read(&text, sizeof(text));
        Serial.print("Received: ");
        Serial.println(text);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temp & Humidity");
        lcd.setCursor(0, 1);
        lcd.print(text);
    }
    delay(500);
}
