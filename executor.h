#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include "storage.h"

class Executor {
public:
    Executor(Storage& storage) : storage_(storage) {}
    std::string execute(const std::string& cmdLine);

private:
    Storage& storage_;
};

#endif // EXECUTOR_H