#include "UrlParser.h"

void UrlParser::parseUrl(String urlString, Url *url)
{

    // Assume a valid URL
    enum URLParseState
    {
        PROTOCOL,
        SEPERATOR,
        HOST,
        PORT,
        PATH
    } state = PROTOCOL;

    url->protocol = "";
    url->host = "";
    url->port = "";
    url->path = "/";

    for (int i = 0; i < urlString.length(); i++)
    {
        switch (state)
        {
        case PROTOCOL:
            if (urlString[i] == ':')
                state = SEPERATOR;
            else
                url->protocol += urlString[i];
            break;
        case SEPERATOR:
            if (urlString[i] != '/')
            {
                state = HOST;
                url->host += urlString[i];
            }
            break;
        case HOST:
            if (urlString[i] == ':')
                state = PORT;
            else
            {
                if (urlString[i] == '/')
                    state = PATH;
                else
                    url->host += urlString[i];
            }
            break;
        case PORT:
            if (urlString[i] == '/')
                state = PATH;
            else
                url->port += urlString[i];
            break;
        case PATH:
            url->path += urlString[i];
        }
    }
}