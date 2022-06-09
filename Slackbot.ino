
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "LedControl.h"
#include "StateManager.h"
#include "UrlParser.h"
#include "Slackbot.h"
#include "Storage.h"

bool Slackbot::_connected = false;
String Slackbot::_botId;
String Slackbot::_oauthToken;

WebSocketsClient Slackbot::_webSocket;

// TODO include arduino.h, rename to cpp across the board

// Identifies whether a @Inferno command has been received from a user
bool Slackbot::commandReceived(String payload, String command) 
{
    return payload.indexOf("<@" + _botId + "> " + command) > -1;
}

// Processes a Slack mesage
void Slackbot::processPayload(String payload)
{
    // Only process messages. Note that we're not deserializing JSON here as 
    // payloads are too large for the heap on ESP8266.
    if (payload.indexOf("\"type\":\"message\"") == -1)
    {
        return;
    }

    if (
        StateManager::getState() == State::Monitor &&
        (payload.indexOf("[Alerting]") > -1 || payload.indexOf("Triggered: ") > -1))
    {
        LedControl::setIntensity(255);
        LedControl::on(); // TODO would be nice to fade in here

        LedControl::fadeOut(FADE_DURATION, UNHEALTHY_DURATION);
    }
    else if (commandReceived(payload, "on"))
    {
        StateManager::setState(State::On);
    }
    else if (commandReceived(payload, "off"))
    {
        // An "off" command is expected either after Inferno is in "on" state,
        // or in "monitoring" state but the monitoring incident has passed.
        // We should therefore fade out, but reset the state to monitor.
        LedControl::fadeOut();
        StateManager::setState(State::Monitor);
    }
}

// Called on web socket event. Handles disconnection, and incoming messages from Slack.
void Slackbot::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WebSocket] Disconnected \n");
        _connected = false;
        break;

    case WStype_CONNECTED:
        Serial.printf("[WebSocket] Connected to: %s\n", payload);
        // sendReply();
        break;

    case WStype_TEXT:
        Serial.printf("[WebSocket] Message: %s\n", payload);
        // send message to
        //_webSocket.sendTXT("message here");

        String payloadString;
        for (int i = 0; i < length; i++)
        {
            payloadString += (char)payload[i];
        }

        processPayload(payloadString);
        break;
    }
}

bool Slackbot::readConfig()
{
    // Read OAuth token from file
    File tokenFile;
    if (!Storage::readFile("secrets/slackToken.txt", tokenFile))
    {
        Serial.println("Unable to read Slack token");
        return false;
    }

    _oauthToken = tokenFile.readString();
    tokenFile.close();

    // Read Slack bot id from file
    File slackBotIdFile;
    if (!Storage::readFile("secrets/slackBotId.txt", slackBotIdFile))
    {
        Serial.println("Unable to read Slack token");
        return false;
    }

    _botId = slackBotIdFile.readString();
    slackBotIdFile.close();

    return true;
}

/*
  Establishes a bot connection to Slack:
  1. Performs a REST call to get the WebSocket URL
  2. Conencts the WebSocket
  Returns true if the connection was established successfully.
*/

bool Slackbot::connect()
{
    if (!readConfig())
    {
        return false;
    }

    WiFiClientSecure client;

    // Avoid having to manage certificates or thumbprints
    client.setInsecure();

    // Get websocket address
    HTTPClient http;
    http.begin(client, "https://slack.com/api/rtm.connect");
    http.addHeader("Authorization", "Bearer " + _oauthToken);

    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("HTTP GET failed with code %d\n", httpCode);
        return false;
    }

    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    String ok = doc["ok"];
    String error = doc["error"];

    // Verify authorisation was successful
    if (ok == "false")
    {
        Serial.print("Error connecting to Slack: ");
        Serial.println(error);

        return false;
    }

    Url url;
    UrlParser::parseUrl(doc["url"], &url);

    // Open websocket connection and register event handler
    Serial.println("WebSocket Host=" + url.host + " Path=" + url.path);
    _webSocket.beginSSL(url.host.c_str(), 443, url.path.c_str());
    _webSocket.onEvent(webSocketEvent);

    _webSocket.setReconnectInterval(20000);
    _webSocket.enableHeartbeat(15000, 3000, 2);

    return true;
}

void Slackbot::update()
{
    _webSocket.loop();

    if (!_connected)
    {
        _connected = Slackbot::connect();
    }
}