/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "ZerOne_YZ"
#define STAPSK  "112233445566"
#endif
const int RECONNECT = 0;
const int RECEIVE        = 1;
const int UPDATA          = 2;
const int SEND              = 3;

const char* ssid = STASSID;
const char* password = STAPSK;

auto led = 2;
int brightness = 100;
int bordStatus = 1;
unsigned long timeout = 0;
int recieveBytesCount = 0;
const char* host = "192.168.1.14";
const uint16_t port = 8266;
WiFiClient client;


void setup() {
    Serial.begin(115200);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
       would try to act as both a client and an access-point and could cause
       network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());

    Serial.print("connecting to ");
    Serial.print(host);
    Serial.print(':');
    Serial.println(port);

    // Use WiFiClient class to create TCP connections
    
    while (!client.connect(host, port)) {
        Serial.println("connection failed , Reboot ! ! !");
        delay(5000);
        return;
    }

    analogWrite(led, brightness);
}

void loop() {

    switch (bordStatus)
    {
    case RECEIVE:
        if (!client.connected())  bordStatus = RECONNECT;

        timeout = millis();     //ÉèÖÃ³¬Ê±
        recieveBytesCount = client.available();

        while (client.available() == 0) {
            if (millis() - timeout > 50000) {
                Serial.println(">>> Client Timeout !");
                client.stop();
                bordStatus = RECONNECT;
                delay(5000);
                return;
            }
        }

        while (client.available())
        {
            char ch = static_cast<char>(client.read());
            Serial.print(ch);
            if (ch=='a')
            {
                brightness = 40;
            }
            else if(ch=='o')
            {
                brightness = 60;
            }
            else
            {
                brightness += 20;
            }
        }
        break;
    case RECONNECT:
        while (!client.connect(host, port)) {
            Serial.println("connection failed£¬ Reboot and connnect");
            delay(1000);
            ESP.restart();
            return;
        }
        break;
    case SEND:
        if (!client.connected())  bordStatus = RECONNECT;
        client.println("hello from ESP8266");
        break;
    case UPDATA:
        break;
    default:
        Serial.println("Aha ? ......");
        break;
    }
    analogWrite(led, brightness);
}