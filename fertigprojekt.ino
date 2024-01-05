#include <WiFi.h>

#include <Adafruit_AHTX0.h>

#include <PubSubClient.h>
#define LDR 36

char mac[17];

const char* ssid = "Mywifis";
const char* pass = "happiness";

const char* mqtt_server = "ioe.noseryoung.ch";

WiFiClient espClient;

PubSubClient client(espClient);


void setup_wifi() {

  Serial.print("Connecting to ");

  Serial.print(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println("done!");

  byte m[6];

  WiFi.macAddress(m);

  sprintf(mac, "%02X:02X:02X:02X:02X:02X", m[0], m[1], m[2], m[3], m[4], m[5]);
}
void callback(char* topic, byte* payload, unsigned int length) {



  char message[length];

  for (int i = 0; i < length; i++) {

    message[i] = (char)payload[i];



    Serial.print("Received Message: ");

    Serial.println(message);
  }
}
void reconnect() {

  Serial.print("Attempting MQTT connection...");

  while (!client.connected()) {

    if (client.connect(mac)) {

      Serial.println("done!");

    } else {

      delay(500);

      Serial.print(".");
    }
  }
}

bool isPublished = 0;


void setup() {

  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop() {

  int LDR_val = analogRead(LDR);
  char LDR_valString[8];

  dtostrf(LDR_val, 1, 2, LDR_valString);
  Serial.println(LDR_valString);

  delay(2000);

  if (!client.connected()) {

    reconnect();
  }

  // const char* tempBuffer = LDR_val;


  isPublished = client.publish("worblaufen/poststatus/busu", LDR_valString);



  client.loop();

  return;
}