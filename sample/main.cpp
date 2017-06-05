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
  client = new RaspberryOSIOClient((char *) "gizz", (char *) "80", (char *) "7E4ZHOQJ", onMessage);

  cout << "Client started. When \"exit\" message is received, the program will publish test message to topic and finish its work." << endl;

  // Subscribe for topic.
  bool result = client->subscribe((char * ) "/users/gizz/test");

  cout << "Subscribing result: " << (result == true ? "success" : "error") << endl;

  // Main communication loop to process messages.
  do
  {
    // Save loop iteration state (TRUE if all ok).
    result = client->loop();
    // Just show that we are alive.
    cout << ".\r\n";
    // Wait 1 second.
    sleep(1);
  }
  while(result == true); // Break if loop returned FALSE.

  cout << "Publishing \"hi!\" message: ";
  result = client->publish((char *) "/users/gizz/test", (char *) "hi!");

  cout << (result == true ? "success" : "error") << endl;

  delete client;

  cout << "Bye!" << endl;

  return 0;
}
