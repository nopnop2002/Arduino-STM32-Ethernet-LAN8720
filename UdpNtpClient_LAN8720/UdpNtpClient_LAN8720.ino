/*

 Udp NTP Client

 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 modified 23 Jun 2017
 by Wi6Labs
 modified 1 Jun 2018
 by sstaub
 This code is in the public domain.

 */

#include <LwIP.h>
#include <STM32Ethernet.h>
#include <EthernetUdp.h>
#include <TimeLib.h>     // https://github.com/PaulStoffregen/Time

// Your NTP Server
#define NTP_SERVER      "pool.ntp.org"
// Your local time zone
#define TIME_ZONE       9            
// local port to listen for UDP packets
#define LOCAL_PORT      8888

// NTP time stamp is in the first 48 bytes of the message
#define NTP_PACKET_SIZE 48
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// Get Day of the week string [Sun,Mon....]
char * dow2char(byte days) {
  char *dayOfWeek[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  return dayOfWeek[days];
}

// Get Day of the week [0-Sunday, 1-Monday etc.]
uint8_t get_dow(unsigned long t) {
    return ((t / 86400) + 4) % 7;
}

void showTime(char * title, time_t timet, char * dow) {
   Serial.print(title);
   Serial.print(year(timet));
   Serial.print("/");
   Serial.print(month(timet));
   Serial.print("/");
   Serial.print(day(timet));
   Serial.print(" ");
   Serial.print(hour(timet));
   Serial.print(":");
   Serial.print(minute(timet));
   Serial.print(":");
   Serial.print(second(timet));
   Serial.print(" [");
   Serial.print(dow);
   Serial.println("]");
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
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());
  
  Udp.begin(LOCAL_PORT);
}

void loop() {
  sendNTPpacket(NTP_SERVER); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(1000);
  if (Udp.parsePacket()) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = ");
    //Serial.println(secsSince1900);

    // now convert NTP time into UNIX time:
    // UNIX time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears; 
    Serial.print("Unix time = ");
    Serial.println(epoch);

    // Greenwich Mean Time
    uint8_t DayOfWeek = get_dow(epoch); 
    showTime("The UTC time is ", (time_t)epoch, dow2char(DayOfWeek));

    // Local Time
    if (TIME_ZONE != 0) {
      unsigned long epochLocal = epoch + (TIME_ZONE * 60 * 60);
      DayOfWeek = get_dow(epochLocal);
      showTime("The LOCAL time is ", (time_t)epochLocal, dow2char(DayOfWeek));
    }
   
  }
  // wait ten seconds before asking for the time again
  delay(10000);
  Ethernet.maintain();
}

// send an NTP request to the time server at the given address
void sendNTPpacket(char* address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
