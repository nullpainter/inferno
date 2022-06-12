#include <cstring>
#include "UrlParser.h"

bool UrlParser::parse(char *text, Url &url)
{
    MatchState matchState;
    matchState.Target(text);

    // Match on host and path only
    char result = matchState.Match("[a-z]+:\/\/([^/]+)(.*)");

    if (result != REGEXP_MATCHED)
    {
        return false;
    }
    
    char buffer[512];

    // Extract host
    matchState.GetCapture(buffer, 0);
    strcpy(url.host, buffer);

    // Extract path
    matchState.GetCapture(buffer, 1);
    strcpy(url.path, buffer);

    return true;
}