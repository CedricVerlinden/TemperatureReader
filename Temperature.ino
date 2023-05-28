// Libraries
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ThingSpeak.h>
#include <WiFiNINA.h>

// OneWire settings
#define ONE_WIRE_BUS 4

// WiFi settings
const char *ssid = "";
const char *password = "";

// ThingSpeak settings
const unsigned long channelID = 1;
const char *apiKey = "";

WiFiClient client;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

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
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi: " + String(ssid));

    // Connect to ThingSpeak
    ThingSpeak.begin(client);

    // Set LED pins as outputs
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(buzzer, OUTPUT);
}

void loop() {
    // Retreive temperature from sensor
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);

    Serial.print("Celsius temperature: ");
    Serial.print(temperature);

    // Check if temperature is within range and set LEDs accordingly
    if (temperature >= 15 && temperature <= 30) {
        Serial.println(" OK");
        digitalWrite(greenLed, HIGH);
    } else {
        Serial.println(" BAD");
        digitalWrite(redLed, HIGH);
        tone(buzzer, 1000, 1000);
    }

    // Send data to ThingSpeak (disabled for testing)
    // ThingSpeak.setField(1, temperature);
    // ThingSpeak.writeFields(channelID, apiKey);

    // Wait 15 seconds before next reading (ThingSpeak allows 1 update per 15
    // seconds)
    delay(1000);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    delay(14000);
}
