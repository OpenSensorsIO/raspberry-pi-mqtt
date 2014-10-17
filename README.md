raspberry-pi-mqtt
=================

This is an OpenSensors client for [Raspberry PI board](http://www.raspberrypi.org/).

## Hardware prerequisites

You need a [Raspberry PI board](http://www.raspberrypi.org/) to start using this library.

## Software prerequisites

First you need to install operating system on your Raspbery PI board. We have tested our library on [Raspbian](http://raspbian.org/). You can install any other system what you want. There is [nice instalation guide](http://www.raspberrypi.org/documentation/installation/installing-images/README.md). Secondly you should keep in mind that you must build your application on operating system installed on your Raspberry. You should do so because lined C/C++ program depends on processor architecture. So to use program on Ubuntu 14.04 (for example) link it on Ubuntu. To run it on Raspbian link it on Raspbian. In our library we use [mosquitto MQTT client library](http://mosquitto.org/api/files/mosquitto-h.html). Therefore to link your future program you need install libmosquitto0-dev first (example listed below working for Ubuntu):

``` code
apt-get install libmosquitto0-dev
```

This will install necessary binaries and mosquitto.h files on your system.

## Getting Started

To use library in your first client program include [raspberry_osio_client.h](./raspberry_osio_client/raspberry_osio_client.h) in your program:

``` code
#include "raspberry_osio_client.h"
```

## Library reference

This client library realized as C++ class named RaspberryOSIOClient. There are such public methods to work with it:

Constructors which initialize necessary connection parameters. There are some overrides of it ("server name" and "callback" parameters are not mandatory):

``` code
  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword); 
  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword, char * serverName);
  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword, void (*callback)(char*,char*,unsigned int));
  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword, char * serverName, void (*callback)(char*,char*,unsigned int));
```

Method to process iteration of receiving data (callback supplied in constructor (if necessary)):

``` code
int RaspberryOSIOClient::loop();
```

Method to publish message to topic:

``` code
int RaspberryOSIOClient::publish(char* topic, char* payload);
```

Method to subscribe for topic:

``` code
int RaspberryOSIOClient::subscribe(char* topic);
```

## Subscribe to a Message

``` code
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "raspberry_osio_client.h"

using namespace std;

RaspberryOSIOClient * client = 0;


/*
 * Handler for incoming messages.
 */	
void onMessage(char* topic, char* payload, unsigned int length) 
{
  char* clearMessage = new char[length + 1];
  memset(clearMessage, 0, length + 1);
  memcpy(clearMessage, payload, length);
  
  cout << "Topic: " << topic << ", message: " << clearMessage;

  // Break communication cycle when receive "exit".
  if (strcmp(clearMessage, "exit") == 0)
  {
    client->disconnect();
  }
}

int main() 
{
  // Our raspberry MQTT client instance.
  client = new RaspberryOSIOClient("username", "deviceod", "7E4fHOQJ", onMessage);

  cout << "Client started. When \"exit\" message is received, the program will finish its work." << endl;

  // Subscribe for topic.
  int result = client->subscribe("/users/username/test");

  cout << "Subscribing result: " << (result == OSIO_ERROR_SUCCESS ? "success" : "error") << endl;

  // Main communication loop to process messages.
  do
  {
    // Save loop iteration state (OSIO_ERROR_SUCCESS if all ok).
    result = client->loop();
    // Just show that we are alive.
    cout << ".\r\n";
    // Wait 1 second.
    sleep(1);
  }
  while(result == OSIO_ERROR_SUCCESS); // Break if loop not returned OSIO_ERROR_SUCCESS.

  delete client;
  return 0;
}
```

## Publish a Message

``` code
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "raspberry_osio_client.h"

using namespace std;

RaspberryOSIOClient * client = 0;


int main() 
{
  // Our raspberry MQTT client instance.
  client = new RaspberryOSIOClient("username", "deviceid", "7s4ZHOQJ");

  cout << "Client started." << endl;
  cout << "Publishing \"hi!\" message: ";

  result = client->publish("/users/username/test", "hi!");

  cout << (result == OSIO_ERROR_SUCCESS ? "success" : "error") << endl;

  delete client;
  return 0;
}
```

## Run example

You can build [sample](./sample) program using [makefile](makefile) and run it on your operating system. This sample combines publishing and sunscribing to topic.
