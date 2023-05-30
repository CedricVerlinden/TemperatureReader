// Libraries
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ThingSpeak.h>
#include <WiFiNINA.h>

// OneWire settings
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// WiFi settings
const char *ssid = "";
const char *password = "";
WiFiClient client;

// ThingSpeak settings
const unsigned long channelID = 0;
const char *apiKey = "";

// LED pins
int redLed = 6, greenLed = 3, buzzer = 8;

void setup() {
    Serial.begin(9600);

    // Wait for serial to initialize.
    while (!Serial) {
    };

    // Start the OneWire library
    sensors.begin();

    // Connect to WiFi
    connectToWiFi();

    // Connect to ThingSpeak
    connectToThingSpeak();

    // Set LED pins as outputs
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(buzzer, OUTPUT);
}

void loop() {
    // Retrieve temperature from sensor
    float temperature = getTemperature();

    // Print temperature to serial monitor
    printTemperature(temperature);

    // Send data to ThingSpeak
    sendDataToThingSpeak(temperature);

    // Check if temperature is within range and set LEDs accordingly
    setLEDs(temperature);

    delay(1000);

    // Reset LEDs
    resetLEDs();

    // Wait 15 seconds before next reading (ThingSpeak allows 1 update per 15
    // seconds)
    delay(14000);
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi: " + String(ssid));
}

void connectToThingSpeak() { ThingSpeak.begin(client); }

float getTemperature() {
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}

void printTemperature(float temperature) {
    Serial.print("Celsius temperature: ");
    Serial.print(temperature);
    Serial.println((temperature >= 15 && temperature <= 30) ? " OK" : " BAD");
}

void setLEDs(float temperature) {
    if (temperature >= 15 && temperature <= 30) {
        digitalWrite(greenLed, HIGH);
        digitalWrite(redLed, LOW);
        noTone(buzzer);
    } else {
        digitalWrite(greenLed, LOW);
        digitalWrite(redLed, HIGH);
        tone(buzzer, 1000, 1000);
    }
}

void resetLEDs() {
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    noTone(buzzer);
}

void sendDataToThingSpeak(float temperature) {
    ThingSpeak.setField(1, temperature);
    ThingSpeak.writeFields(channelID, apiKey);
}