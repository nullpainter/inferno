#include "Storage.h"

bool Storage::readFile(const String &path, File &file)
{
    file = LittleFS.open(path, "r");
    if (file)
    {
        return true;
    }

    Serial.print("File ");
    Serial.print(path);
    Serial.println(" not found");

    return false;
}
