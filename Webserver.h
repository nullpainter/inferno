#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>

#define LISTENING_PORT 80

class Webserver
{
public:
    void setup();
    void update();
    void handleClient();

private:
    static ESP8266WebServer _server;

    static void status();
    static void fire();
    static void help();
    static void off();
    static void on();
    static void monitor();
    static bool verifyPost();
    static void writeResponse(const String &path);
};
#endif