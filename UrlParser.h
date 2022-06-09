#ifndef _URLPARSER_H_
#define _URLPARSER_H_

#include <Arduino.h>

struct Url
{
  String protocol = "";
  String host = "";
  String port = "";
  String path = "";
};

class UrlParser
{
public:
  static void parseUrl(String urlString, Url *url);
};

#endif
