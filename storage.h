#ifndef STORAGE_H
#define STORAGE_H

#include <string>

class Storage {
public:
    void set(const std::string &key, const std::string &value);
    bool get(const std::string &key, std::string&value);
    void show();
};

#endif // STORAGE_H
