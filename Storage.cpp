#include <LittleFS.h>
#include "Storage.h"

bool Storage::readFile(const char *path, File &file)
{
    file = LittleFS.open(path, "r");
    if (file)
    {
        return true;
    }

    Serial.printf("File %s not found\n", path);

    return false;
}
