#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Parser {
public:
    static std::vector<std::string> parseCommands(std::string &buffer);
};

#endif // PARSER_H
