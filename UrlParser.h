#ifndef _URLPARSER_H_
#define _URLPARSER_H_

#include <Regexp.h>

struct Url
{
    char host[128];
    char path[256];
};

/**
 * @brief Basic regular expression based URL parser.
 */
class UrlParser
{
public:
    /**
     * @brief Parses a URL string, extracting the host and path.
     * @remark ports are not handled; these wil be returned as part of the host.
     *
     * @param text text to parse
     * @param url url to populate
     * @return true if parsing succeded, false otherwise.
     */
    static bool parse(char *text, Url &url);
};

#endif