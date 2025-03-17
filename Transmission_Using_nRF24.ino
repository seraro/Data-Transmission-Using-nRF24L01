//Experiment: Wireless Sensor Data Transmission Using nRF24L01 and Arduino
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (!isnan(temperature) && !isnan(humidity)) {
        String data = String(temperature) + "," + String(humidity);
        radio.write(&data, sizeof(data));
        Serial.print("Sent: ");
        Serial.println(data);
    }
    delay(2000);
}