#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <FastLED.h>


#define DHTPIN 12
#define LED 26
#define SERVO_PIN 2
#define LED_PIN 4
#define NUM_LEDS 16


#define DHTTYPE DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);


Servo servo;


CRGB leds[NUM_LEDS];

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
const char* clientID = "SmartGardeningESP32";
const char* topic = "Tempdata";

WiFiClient espClient;
PubSubClient client(espClient);


unsigned long previousMillis = 0;
const long interval = 1000;


bool simulateWiFiDisconnect = true; //to disconnect wifi => set it to true

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID)) {
      Serial.println("MQTT connected");
      client.subscribe("lights");
      client.subscribe("servo");
      client.subscribe("lights/neopixel");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String data = "";
  for (int i = 0; i < length; i++) {
    data += (char)payload[i];
  }

  if (String(topic) == "lights") {
    if (data == "ON") {
      digitalWrite(LED, HIGH); // Turn LED ON
    } else {
      digitalWrite(LED, LOW); // Turn LED OFF
    }
  } else if (String(topic) == "servo") {
    int degree = data.toInt(); 
    servo.write(degree);     
    Serial.print("Servo moved to: ");
    Serial.println(degree);
  } else if (String(topic) == "lights/neopixel") {
    int red, green, blue;
    sscanf(data.c_str(), "%d,%d,%d", &red, &green, &blue); // Parse RGB values
    fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));    // Update NeoPixel
    FastLED.show();
    Serial.print("NeoPixel color set to R:");
    Serial.print(red);
    Serial.print(" G:");
    Serial.print(green);
    Serial.print(" B:");
    Serial.println(blue);
  }
}

void setup() {
  Serial.begin(115200);

  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Initialize DHT sensor
  dht.begin();

  
  servo.attach(SERVO_PIN, 500, 2400);
  servo.write(90); 

  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  
  if (!simulateWiFiDisconnect && WiFi.status() == WL_CONNECTED && client.connected()) {
    digitalWrite(LED, HIGH); 
  } else {
    digitalWrite(LED, LOW);  
    if (!simulateWiFiDisconnect) reconnect(); 
  }

  client.loop();

  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    checkSensorDataAndPublish();
  }

  // Debugging: Change simulateWiFiDisconnect for testing
  // Uncomment the following lines to toggle disconnection after 10 seconds
  // if (millis() > 10000 && !simulateWiFiDisconnect) {
  //   simulateWiFiDisconnect = true;
  //   Serial.println("Simulated WiFi disconnection activated!");
  // }
}

void checkSensorDataAndPublish() {
  sensors_event_t event;
  float temp, hum;

  
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    temp = event.temperature;
    Serial.print("Temperature: ");
    Serial.println(temp);
  } else {
    temp = -1;
    Serial.println("Error reading temperature!");
  }

  
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    hum = event.relative_humidity;
    Serial.print("Humidity: ");
    Serial.println(hum);
  } else {
    hum = -1;
    Serial.println("Error reading humidity!");
  }

  
  String msg = String(temp) + "," + String(hum);
  client.publish(topic, msg.c_str());
  Serial.print("Published data: ");
  Serial.println(msg);
}
