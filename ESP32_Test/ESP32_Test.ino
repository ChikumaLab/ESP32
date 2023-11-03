#include <PubSubClient.h>

#include <WiFi.h>
char ssid[] = "AirPort11720";
char pass[] = "1388035971535";

const char* mqtt_server = "133.242.211.199";
const char* mqtt_topic = "Audio";
const char* mqtt_topic2 = "test";
 
WiFiClient mqttClient;
PubSubClient client(mqttClient);
char mqtt_clientid[32];
int Flag;

void callback(char* topic, byte* payload, unsigned int length) {
    char payload_ch[32];
    char topic_ch[32];
    String messa = "Audio";
    int chlen = min(31, (int)length);
    int tplen = min(31, (int)length);
    memcpy(payload_ch, payload, chlen);
    memcpy(topic_ch, topic, chlen);
    payload_ch[chlen] = 0;
    topic_ch[tplen] = 0;
    if(messa.compareTo(topic) == 0)
    {
      Serial2.print(payload_ch);
      Serial2.print("\r");
      Serial.println("Audio Loop");
      Serial.println(topic);
      Serial.println(payload_ch);
      client.publish(mqtt_topic2, "Audio Send"); //送信はこう
    }
    else
    {
      Serial.println(topic);
      Serial.println(payload_ch);
      
    }

}

void wait_mqtt() {
    if (!client.connected()) {
        Serial.print("Waiting MQTT connection...");
        while (!client.connected()) { // 非接続のあいだ繰り返す
            if (client.connect(mqtt_clientid)) {
                client.subscribe(mqtt_topic);
                client.subscribe(mqtt_topic2);
            } else {
                delay(2000);
            }
        }
        Serial.println("connected");
    }
}
 
void setup() {
    Serial.begin(115200);
    Serial2.begin(9600);


    Serial.print("connecting wifi");
    WiFi.mode(WIFI_STA);
    if (String(WiFi.SSID()) != String(ssid)) {
        WiFi.begin(ssid, pass);
    }
    WiFi.setSleep(false);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("connected");
    delay(1000);

    byte mac_addr[6];
    WiFi.macAddress(mac_addr);
    sprintf(mqtt_clientid, "ESP32CLI_%02x%02x%02x%02x%02x%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
       Serial.print("Audio ");
    for(int i = 0;i < 16;i++)
    {
      Serial2.write('?');
      if(Serial2.read() == '>')
      {
        Serial.print("Audio OK \n");
        break;
      }
       Serial.print(".");
       delay(1000);
    }
    Serial.print("Audio Pass \n");

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}
void loop() {
    wait_mqtt();
    client.loop();
}
