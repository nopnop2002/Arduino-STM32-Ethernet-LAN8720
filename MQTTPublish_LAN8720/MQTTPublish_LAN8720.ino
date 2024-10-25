#include <LwIP.h>
#include <STM32Ethernet.h>
#include <PubSubClient.h>

#define MQTT_SERVER     "broker.emqx.io"
#define MQTT_PORT       1883
#define MQTT_TOPIC      "/test/stm32" // You can change
#define MQTT_WILL_TOPIC MQTT_TOPIC
#define MQTT_WILL_MSG   "I am leaving..." // You can change
#define MQTT_INTERVAL   2000

EthernetClient EthClient;
PubSubClient client(EthClient);
unsigned long lastMillis;

void errorDisplay(char* buff) {
  Serial.print("Error:");
  Serial.println(buff);
  while(1);
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start Ethernet and UDP
  Serial.println("Begin Ethernet");
  if (Ethernet.begin() == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  Serial.println("Success to configure Ethernet using DHCP");

  Serial.print("localIP: ");
  Serial.println(Ethernet.localIP());
  IPAddress ip = Ethernet.localIP();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  char clientid[20];
  sprintf(clientid,"STM32-%03d",ip[3]);
  Serial.print("clientid=");
  Serial.println(clientid);
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (!client.connect(clientid, MQTT_WILL_TOPIC, 0, 0, MQTT_WILL_MSG)) {
    errorDisplay("connect Fail");
  }
  Serial.println("connected");
  lastMillis = millis();
}

void loop() {
  char msg[50];

  if (!client.connected()) {
    errorDisplay("not connect Broker");
  }
  client.loop();

  long now = millis();
  if (now < lastMillis) lastMillis = now;
  if (now - lastMillis > MQTT_INTERVAL) {
    lastMillis = now;
    snprintf (msg, 75, "hello world #%lu", now);
    Serial.print("Publish message: ");
    Serial.println(msg);
    if (!client.publish(MQTT_TOPIC, msg)) {
      errorDisplay("publish fail");
    }
  }
  Ethernet.maintain();
}
