extern "C" {
    #include <user_interface.h>
}

#include <Milkcocoa.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define MILKCOCOA_APP_ID    ""
#define MILKCOCOA_DATASTORE "espr"
#define MILKCOCOA_SERVERPORT    1883

const char* ssid = "";
const char* password = "";
const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;
const int ledPin = 14;
const int sensorPin = 16;

WiFiServer server(80);
WiFiClient client;
Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(sensorPin, INPUT);

    // Connect to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.println(WiFi.localIP());
}

void loop() {
    if (digitalRead(sensorPin) == HIGH) {
        digitalWrite(ledPin, HIGH);
        sendData();
    } else {
        digitalWrite(ledPin, LOW);
    }
    delay(500);
}

void sendData() {
    milkcocoa.loop();
    DataElement elem = DataElement();
    elem.setValue("status", "detected!");
    milkcocoa.push(MILKCOCOA_DATASTORE, &elem);
}
