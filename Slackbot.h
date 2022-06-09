#ifndef _SLACKBOT_H_
#define _SLACKBOT_H_

#include <WebSocketsClient.h>

class Slackbot
{
public:
    static void update();

private:
    static bool connect();
    static void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
    static void processPayload(String payload);
    static bool readConfig();
    static bool commandReceived(String payload, String command);

    static bool _connected;
    static WebSocketsClient _webSocket;
    static String _botId;
    static String _oauthToken;
};
#endif