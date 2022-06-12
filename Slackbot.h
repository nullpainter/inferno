#ifndef _SLACKBOT_H_
#define _SLACKBOT_H_

#include <WebSocketsClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "UrlParser.h"

/**
 * @brief
 * @remark Code adapted from https://github.com/alexcorvis84/SlackBot/
 *
 */
class Slackbot
{
public:
    static void update();

private:
    /**
     * @brief Establishes a bot connection to Slack
     *
     * @return true if the connection was established successfully.
     */
    static bool connect();
    static bool authorise(Url &url);

    /**
     * @brief Called on websocket events. Handles connection and incoming Slack messages
     *
     * @param type type of websocket event
     * @param payload payload
     * @param length length of payload
     */
    static void webSocketEvent(WStype_t type, unsigned char *payload, unsigned long length);

    /**
     * @brief Processes a Slack message
     *
     * @param payload JSON payload to process
     */
    static void processPayload(char *payload);

    /**
     * @brief Reads Slack bot configuration from files in flash.
     *
     * @return whether files could be read
     */
    static bool readConfig();

    /**
     * @brief Identifies whether a @Inferno command has been received from a user.
     * @remark This matching is pretty basic, is case-sensitive and requires a single space.
     *
     * @param text Slack message text
     * @param command the command to test
     * @return true whether the payload matched the command.
     */
    static bool commandReceived(const char *text, const char *command);

    /**
     * @brief Initialises Slack message properties that are deserialized.
     */
    static void initialisePayloadFilters();

    /**
     * @brief whether the Slackbot is connected to Slack and can send and receive messages.
     */
    static bool _connected;
    static WebSocketsClient _webSocket;

    static char _botId[64];
    static char _oauthToken[128];

    static WiFiClientSecure _wifiClient;
    static StaticJsonDocument<200> _payloadFilter;

    static char _authorisationHeader[256];

    static const inline char *slackAlertIndicator = "[Alerting]";
    static const inline char *dataDogIndicator = "Triggered: ";
};
#endif