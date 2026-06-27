#include <DHT.h>
#include <WiFiS3.h>
#include <PubSubClient.h>

#define DHTPIN 2
#define DHTTYPE DHT11

// WIFI
const char* password = "12345678";
const char* ssid = "iPhoneAndrei";

// MQTT192.168.1.223
const char* mqtt_server = "172.20.10.6";
const int mqtt_port = 1883;

// declarare client mqtt
WiFiClient wifiClient;
PubSubClient client(wifiClient);

DHT dht(DHTPIN, DHTTYPE);

const int RPWM = 5;
const int LPWM = 6;
bool open = true;
bool autoMode = true;

unsigned long lastMsg = 0;

// ====================== Functii ============================ //

void stopActuator() {
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
}

void openActuator() {
  analogWrite(RPWM, 200);
  analogWrite(LPWM, 0);
}

void closeActuator() {
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 200);
}

// functie callback pentru mod AUTO/MANUAL
void callback(char* topic, byte* payload, unsigned int length) 
{
  String msg = "";

  for (unsigned int i = 0; i < length; i++) 
  {
    msg += (char)payload[i];
  }

  Serial.print("MQTT: ");
  Serial.println(msg);

  if (msg == "AUTO") 
  {
    autoMode = true;
    Serial.println("Mod AUTO");
  }
  else if (msg == "MANUAL") 
  {
    autoMode = false;
    Serial.println("Mod MANUAL");
  }
  else if (autoMode == false && msg == "OPEN") 
  {
    openActuator();
    delay(16000);
    stopActuator();
    open = true;
  }
  else if (autoMode == false && msg == "CLOSE") 
  {
    closeActuator();
    delay(16000);
    stopActuator();
    open = false;
  }
}

void connectWiFi() 
{
  while (WiFi.begin(ssid, password) != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(2000);
  }
  Serial.println("\nWiFi conectat");
}

void reconnectMQTT() 
{
  while (!client.connected()) 
  {
    Serial.println("Incerc conectare MQTT...");
    if (client.connect("arduinoClient")) 
    {
      Serial.println("MQTT conectat");
      client.subscribe("fereastra/actuator/cmd");
    } 
    else 
    {
      Serial.print("Eroare MQTT, rc = ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ======================================================== //

void setup() 
{
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  dht.begin();

  stopActuator();
  Serial.begin(115200);
  delay(300);

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() 
{

  if (!client.connected()) 
  {
    reconnectMQTT();
  }
  client.loop();

  float t = dht.readTemperature();

  if (isnan(t)) 
  {
    Serial.println("Eroare la citirea temperaturii");
    delay(500);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.println(t);

  // trimitere date prin mqtt
  if (millis() - lastMsg > 2000) 
  {
    lastMsg = millis();
    char tempString[8];
    dtostrf(t, 1, 2, tempString);
    client.publish("proiect/temperatura", tempString);
  }

  // logica pentru modul AUTO
  if (autoMode == true) 
  {
    if (t > 29 && open == false) 
    { 
      openActuator();
      delay(13000);
      stopActuator();
      open = true;
    }
    else if (t < 28 && open == true) 
    {
      closeActuator();
      delay(16000);
      stopActuator();
      open = false;
    }
  }

  delay(500);
}