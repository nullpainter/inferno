#ifndef _STORAGE_H_
#define _STORAGE_H_

class Storage
{
public:
    static bool readFile(const char *path, File &file);
};

#endif