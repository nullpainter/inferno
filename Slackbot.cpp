
#include <ESP8266WiFi.h>

#include "LedControl.h"
#include "StateManager.h"
#include "Slackbot.h"
#include "Storage.h"

char Slackbot::_botId[64];
char Slackbot::_oauthToken[128];
char Slackbot::_authorisationHeader[256];

WebSocketsClient Slackbot::_webSocket;
bool Slackbot::_connected = false;

WiFiClientSecure Slackbot::_wifiClient;

bool Slackbot::commandReceived(const char *text, const char *command)
{
    // Create text to search for
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%s%s%s%s", "<@", _botId, "> ", command);

    return strcasestr(text, buffer) ? true : false;
}

void Slackbot::processPayload(char *payload)
{
    // Only process messages. Note that we're not deserializing JSON here as large payloads are too 
    // large for the heap on ESP8266.
    if (!strstr(payload, "\"type\":\"message\""))
    {
        return;
    }

    // Alert if an alerting indicator is present in the message and if Inferno's in a monitor state
    if (StateManager::getState() == State::Monitor &&
        (strstr(payload, slackAlertIndicator) || strstr(payload, dataDogIndicator)))
    {
        LedControl::on();
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
        StateManager::setState(State::Monitor, true);
    }
}

void Slackbot::webSocketEvent(WStype_t type, unsigned char *payload, unsigned long length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println(F("[WebSocket] Disconnected"));
        _connected = false;
        break;

    case WStype_CONNECTED:
        Serial.println(F("[WebSocket] Connected"));
        _connected = true;
        break;

    case WStype_TEXT:
        Serial.printf("[WebSocket] %s\n", payload);
        processPayload((char *)payload);
        break;
    }
}

bool Slackbot::readConfig()
{
    // OAuth token
    File tokenFile;
    if (!Storage::readFile("secrets/slackToken.txt", tokenFile))
    {
        Serial.println(F("Unable to read Slack token"));
        return false;
    }

    int bytes = tokenFile.readBytes(_oauthToken, sizeof(_oauthToken));
    _oauthToken[bytes] = NULL;

    tokenFile.close();

    // Slack bot id
    File slackBotIdFile;
    if (!Storage::readFile("secrets/slackBotId.txt", slackBotIdFile))
    {
        Serial.println(F("Unable to read Slack token"));
        return false;
    }

    bytes = slackBotIdFile.readBytes(_botId, sizeof(_botId));
    _botId[bytes] = NULL;

    slackBotIdFile.close();

    return true;
}

bool Slackbot::authorise(Url &url)
{
    // Avoid having to manage certificates or thumbprints
    _wifiClient.setInsecure();

    // Authorise
    HTTPClient http;
    http.begin(_wifiClient, F("https://slack.com/api/rtm.connect"));

    char header[512];
    snprintf(header, sizeof(header), "Bearer %s", _oauthToken);
    http.addHeader(F("Authorization"), header);

    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("HTTP GET failed with code %d\n", httpCode);
        return false;
    }

    // Parse connect response
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    const char *ok = doc["ok"];
    const char *error = doc["error"];
    const char *urlString = (doc["url"]).as<char *>();

    // Verify authorisation was successful
    if (ok == "false")
    {
        Serial.printf("Error connecting to Slack: %s\n", error);
        return false;
    }

    // Parse websocket string so we can identify the host and path
    if (!UrlParser::parse((char *)urlString, url))
    {
        Serial.printf("Unable to parse Slack websocket URL: %s", urlString);
        return false;
    }

    return true;
}

bool Slackbot::connect()
{
    if (!readConfig())
    {
        return false;
    }

    Url url;
    if (!authorise(url))
    {
        return false;
    }

    // Open websocket connection and register event handler
    _webSocket.beginSSL(url.host, 443, url.path);
    _webSocket.onEvent(webSocketEvent);

    _webSocket.setReconnectInterval(20000);
    _webSocket.enableHeartbeat(15000, 3000, 2);

    return true;
}

void Slackbot::update()
{
    // Authorise to Slack if required
    if (!_connected)
    {
        _connected = connect();
    }

    // Process websocket messages if we have a connection to Slack
    if (_connected)
    {
        _webSocket.loop();
    }
}