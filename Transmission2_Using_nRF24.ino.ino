#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN 3 
#define DHTTYPE DHT11 

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001"; // العنوان المشترك
DHT dht(DHTPIN, DHTTYPE);

// هيكل البيانات - يجب أن يكون متطابقاً في الطرفين
struct DataPacket {
    float temp;
    float hum;
};

DataPacket myData;

void setup() {
    Serial.begin(9600);
    dht.begin();
    
    if (!radio.begin()) {
        Serial.println("nRF24L01 Hardware not responding!");
        while (1);
    }
    
    radio.openWritingPipe(address);
    radio.setChannel(76);          // تحديد القناة (من 0 إلى 125)
    radio.setPALevel(RF24_PA_LOW); // مستوى الطاقة
    radio.stopListening();         // وضع الإرسال
}

void loop() {
    myData.temp = dht.readTemperature();
    myData.hum = dht.readHumidity();

    if (!isnan(myData.temp) && !isnan(myData.hum)) {
        bool success = radio.write(&myData, sizeof(myData));
        
        if (success) {
            Serial.print("Sent -> Temp: ");
            Serial.print(myData.temp);
            Serial.print(" C, Hum: ");
            Serial.println(myData.hum);
        } else {
            Serial.println("Transmission failed (No ACK)");
        }
    }
    delay(2000);
}
