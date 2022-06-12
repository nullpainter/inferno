#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Webserver.h"
#include "StateManager.h"
#include "Storage.h"

ESP8266WebServer Webserver::_server(LISTENING_PORT);

void Webserver::handleClient()
{
  _server.handleClient();
}

void Webserver::setup()
{
  _server.on("/", help);
  _server.on("/off", off);
  _server.on("/on", on);
  _server.on("/monitor", monitor);
  _server.on("/status", status);
  _server.on("/fire", fire);
  _server.onNotFound(help);

  _server.begin();

  Serial.printf("HTTP server started on port %d\n", LISTENING_PORT);
}

void Webserver::status()
{
  File file;
  if (!Storage::readFile("/status.txt", file))
  {
    return;
  }

  // Get status text
  char buffer[64];
  switch (StateManager::getState())
  {
  case On:
    strcpy(buffer, "on");
    break;
  case Off:
    strcpy(buffer, "off");
    break;
  case Monitor:
    strcpy(buffer, "monitoring");
    break;
  }

  // Append free heap text
  long freeMemoryBytes = ESP.getFreeHeap();

  char memoryBuffer[255];
  sprintf(memoryBuffer, ", free heap: %dKB", (freeMemoryBytes / 1024));
  strcat(buffer, memoryBuffer);

  String response = file.readString();
  response.replace("{status}", buffer);

  // Send response
  _server.send(200, "text/plain", response);
}

void Webserver::help()
{
  writeResponse("/help.txt");
}

void Webserver::fire()
{
  writeResponse("/fire.txt");
}

void Webserver::off()
{
  if (!Webserver::verifyPost())
  {
    help();
    return;
  }

  StateManager::setState(Off);
  status();
}

void Webserver::on()
{
  if (!Webserver::verifyPost())
  {
    help();
    return;
  }

  StateManager::setState(On);
  status();
}

void Webserver::monitor()
{
  if (!Webserver::verifyPost())
  {
    help();
    return;
  }

  StateManager::setState(Monitor);
  status();
}

bool Webserver::verifyPost()
{
  if (_server.method() == HTTP_POST)
  {
    return true;
  }

  _server.send(405, "text/plain", "Method not allowed; GET / for help.\n\n");
  return false;
}

void Webserver::writeResponse(const char *path)
{
  File file;
  if (!Storage::readFile(path, file))
  {
    return;
  }

  String response = file.readString();
  _server.send(200, "text/plain", response);
}