#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <Arduino.h>
#include <LittleFS.h>

class Storage
{
public:
    static bool readFile(const String &path, File &file);
};

#endif