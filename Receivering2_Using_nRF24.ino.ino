#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// إعدادات الشاشة: العنوان غالباً 0x27، الحجم 16 عمود و 2 صف
LiquidCrystal_I2C lcd(0x27, 16, 2);

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

struct DataPacket {
    float temp;
    float hum;
};

DataPacket receivedData;

void setup() {
    Serial.begin(9600);
    
    // تشغيل الشاشة
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");

    if (!radio.begin()) {
        lcd.clear();
        lcd.print("Radio Error!");
        while (1);
    }
    
    radio.openReadingPipe(1, address);
    radio.setChannel(76);
    radio.setPALevel(RF24_PA_LOW);
    radio.startListening();
    
    lcd.clear();
    lcd.print("Waiting Data...");
}

void loop() {
    if (radio.available()) {
        radio.read(&receivedData, sizeof(receivedData));
        
        // عرض البيانات على الشاشة
        lcd.clear();
        
        // الصف الأول: الحرارة
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(receivedData.temp, 1); // عرض رقم عشري واحد
        lcd.print((char)223); // رمز الدرجة المئوية
        lcd.print("C");

        // الصف الثاني: الرطوبة
        lcd.setCursor(0, 1);
        lcd.print("Hum:  ");
        lcd.print(receivedData.hum, 1);
        lcd.print(" %");

        // عرض على السيريال للتأكيد
        Serial.print("Temp: "); Serial.print(receivedData.temp);
        Serial.print(" | Hum: "); Serial.println(receivedData.hum);
    }
}
