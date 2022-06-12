
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
StaticJsonDocument<200> Slackbot::_payloadFilter;

bool Slackbot::commandReceived(const char *text, const char *command)
{
    // Create text to search for
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%s%s%s%s", "<@", _botId, "> ", command);

    return strstr(text, buffer) ? true : false;
}

void Slackbot::processPayload(char *payload)
{
    // Deserialize the document
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(_payloadFilter));

    if (error)
    {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
    }

    // Only process messages
    if (doc[F("type")] != "message")
    {
        return;
    }

    serializeJsonPretty(doc, Serial);
    Serial.println();

    const char *text = (doc[F("text")]).as<char *>();
    char buffer[256];
    StaticJsonDocument<256> response;

    // Alert if an alerting indicator is present in the message and if Inferno's in a monitor state
    if (StateManager::getState() == State::Monitor &&
        (strstr(text, slackAlertIndicator) || strstr(text, dataDogIndicator)))
    {
        LedControl::on();
        LedControl::fadeOut(FADE_DURATION, UNHEALTHY_DURATION);
    }
    else if (commandReceived(text, "on"))
    {
        StateManager::setState(State::On);
    }
    else if (commandReceived(text, "off"))
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

void Slackbot::initialisePayloadFilters()
{
    _payloadFilter["type"] = true;
    _payloadFilter["channel"] = true;
    _payloadFilter["text"] = true;
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
    initialisePayloadFilters();

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