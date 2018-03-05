
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // Allows us to connect to, and publish to the Mosquitto MQTT broker

//const char* host = "192.168.211.10";
//const int httpPort = 80;
const char* ssid = "PipeDN";
const char* wifi_password = "p!p3d0uun_20170104";
const char* mqtt_server = "192.168.211.10";
//const char* mqtt_server = "10.0.0.9";
const char* mqtt_topic = "noiseLevel";
const char* mqtt_username = "super";
const char* mqtt_password = "super";
const char* clientID = "Client 1";
//  The analog input for sound device
const int POTPIN = A0;
int analogInputValue = 0;

WiFiClient WiFiClient;

// Create MQTT client to connect to broker
PubSubClient client(mqtt_server, 1883, WiFiClient); // 1883 is the listener port for the Broker

//  Setup method --------------------------------------- //
void setup() {

  pinMode(POTPIN,INPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  Serial.begin(115200);

//  setup network connection to server
//  connect to WIFI API
//  Connect and subscribe to MQTT broker
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifi_password); 

  while (WiFi.status() != WL_CONNECTED) {
                                               
    delay(500);
    Serial.print("!");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

}

void Publish(int level){
  char pubMsg[50];
  String str = "1,";
  String msg = "Level noise: ";
  str += level;
  msg += level;
  
  //  Convert to char array
  str.toCharArray(pubMsg,50);

  //  MQTT handles
  //  Try to publish noiseLevel to broker, else send failed publish to console
  //  and try again
  if (client.publish(mqtt_topic, pubMsg)) {
      Serial.println(msg);
  } else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(mqtt_topic, pubMsg);
    Serial.println(msg);
  }
}

//  turn ON LED and Publish the noise level to the MQTT broker ----------------//
void turnONLEDatThisLevel (int level){

  if (level < 250){
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    //Publish(level);
  }
    
  if (level > 250  && level < 600) {
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
  }
    
  if( level > 600){
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
  }

  Publish(level);
}

//  The main loop -------------------------------- //
void loop() {
  analogInputValue = analogRead(POTPIN);
  turnONLEDatThisLevel(analogInputValue);
  
}
