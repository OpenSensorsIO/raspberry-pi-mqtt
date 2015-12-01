#ifndef raspberry_osio_client_h
#define raspberry_osio_client_h

#include <mosquitto.h>

// Default MQTT port
#define MQTT_PORT 1883

// Default server name
#define OSIO_SERVERNAME "mqtt.opensensors.io"

class RaspberryOSIOClient 
{
private:

  mosquitto * _data;
  char * _userName;
  char * _deviceId;
  char * _devicePassword;
  char * _serverName;
  bool _authenticatedInServer;
  bool connectIfNecessary();
  void initialize(char * userName, char * deviceId, char * devicePassword, char * serverName, void (*callback)(char*,char*,unsigned int));
  
public:

  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword); 
  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword, char * serverName);
  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword, void (*callback)(char*,char*,unsigned int));
  RaspberryOSIOClient(char * userName, char * deviceId, char * devicePassword, char * serverName, void (*callback)(char*,char*,unsigned int));
  ~RaspberryOSIOClient();
  bool publish(char * topic, char * payload);
  bool subscribe(char * topic);
  bool loop();
  bool disconnect();

  // We don't recommend use these functions directly.
  // They are for internal purposes.
  void (*onMessage)(char*,char*,unsigned int);
  void (*onDisconnect)(void*);
  void resetConnectedState();
};

#endif
