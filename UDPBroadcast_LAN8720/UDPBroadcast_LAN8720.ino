#include <LwIP.h>
#include <STM32Ethernet.h>
#include <EthernetUdp.h>

unsigned int localPort = 9800;     // local port to listen for UDP packets
unsigned int remotePort = 9876;    // remote port to listen for UDP packets
unsigned long lastMillis;
IPAddress broadcastIp(255, 255, 255, 255);

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

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
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());

  Serial.print("broadcastIP: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(broadcastIp[thisByte], DEC);
    if (thisByte < 3) Serial.print(".");
  }
  Serial.println();

  Udp.begin(localPort);
  lastMillis = millis();
}

void loop() {
  unsigned long now = millis();
  if (now < lastMillis) lastMillis = now; 
  if (now - lastMillis > 10000) {
    Serial.print("Broadcast to ");
    Serial.println(remotePort);
    lastMillis = now;
    Udp.beginPacket(broadcastIp, remotePort);
    char sbuf[128];
    sprintf(sbuf, "Hello STM32 %lu", now);
    Udp.write(sbuf);
    //Udp.write("\r\n");
    Udp.endPacket();
  }
  Ethernet.maintain();
}
