#include "storage.h"
#include <unordered_map>
#include <mutex>
#include <iostream>

static std::unordered_map<std::string, std::string> store;
static std::mutex store_mutex;

void Storage::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(store_mutex);
    store[key] = value;
}

bool Storage::get(const std::string& key, std::string& value) {
    std::lock_guard<std::mutex> lock(store_mutex);
    auto it = store.find(key);
    if (it != store.end()) {
        value = it->second;
        return true;
    }
    return false;
}

void Storage::show(){
    for (const auto& pair : store) {
    std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

}