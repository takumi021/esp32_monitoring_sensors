#include <WiFi.h>
#include "DHTesp.h"
#include "ThingSpeak.h"

const int DHT_PIN = 15;
const int LED_PIN = 13;
const int trigPin = 5;
const int echoPin = 18;
const char* WIFI_NAME = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";
const int myChannelNumber = yourchannelnumber;
const char* myApiKey = "apikey";
const char* server = "api.thingspeak.com";

DHTesp dhtSensor;
WiFiClient client;
long duration;
float Speed_of_sound = 0.034; // Speed of sound in cm/microsecond (adjust if needed)

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(LED_PIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wifi not connected");
  }
  Serial.println("Wifi connected !");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  // DHT Sensor readings
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  ThingSpeak.setField(1, data.temperature);
  ThingSpeak.setField(2, data.humidity);

  // Ultrasonic distance measurement (simulation for Wokwi)
  int randomDistance = random(100, 200); // Replace with actual pulseIn measurement for hardware
  float dist_in_cm = randomDistance;

  // LED control based on combined conditions
  if (data.temperature > 35 || data.temperature < 12 || data.humidity > 70 || data.humidity < 40 || dist_in_cm < 20) { // Adjust distance threshold as needed
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Send data to ThingSpeak (add distance field)
  ThingSpeak.setField(3, dist_in_cm); // Assuming field 3 stores distance
  int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);

  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("Distance: " + String(dist_in_cm, 2) + " cm"); // Print simulated distance

  if (x == 200) {
    Serial.println("Data pushed successfully");
  } else {
    Serial.println("Push error: " + String(x));
  }
  Serial.println("---");

  delay(10000);
}
