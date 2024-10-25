/*
  DHCP-based IP printer

  This sketch uses the DHCP extensions to the Ethernet library
  to get an IP address via DHCP and print the address obtained.

  Circuit:
   STM32 board with Ethernet support

  created 12 April 2011
  modified 9 Apr 2012
  by Tom Igoe
  modified 02 Sept 2015
  by Arturo Guadalupi
  modified 23 Jun 2017
  by Wi6Labs
  modified 1 Jun 2018
  by sstaub
*/

#include <LwIP.h>
#include <STM32Ethernet.h>

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

// Last Logging Time (MilliSecond)
unsigned long lastLoggingTime = 0;

void printCoreVersion()
{
  Serial.print("My Core version: ");
  Serial.print(STM32_CORE_VERSION_MAJOR);
  Serial.print(".");
  Serial.print(STM32_CORE_VERSION_MINOR);
  Serial.print(".");
  Serial.print(STM32_CORE_VERSION_PATCH);
  Serial.println();
}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    if (thisByte < 3) Serial.print(".");
  }

  Serial.println();
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // this check is only needed on the Leonardo:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("start the Ethernet connection");
  if (Ethernet.begin() == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }

  // print my core version
  printCoreVersion();
  // print my local IP address
  printIPAddress();
  lastLoggingTime = millis();
}

void loop() {

  switch (Ethernet.maintain())
  {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");

    default:
      //nothing happened
      break;

  } // end swich

  unsigned long now = millis();
  if ( (now - lastLoggingTime) < 0) {
    lastLoggingTime = now;
  }
  if ( (now - lastLoggingTime) > 1000) {  
     lastLoggingTime = now;
     printIPAddress();
  }
}
