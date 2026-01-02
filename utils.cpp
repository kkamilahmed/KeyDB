#include <string>
    
void trimCRLF(std::string &s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
}

void toUpper(std::string &s) {
    for (auto &c : s) c = toupper(c);
 }