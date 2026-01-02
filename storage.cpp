#include "storage.h"
#include "utils.h" 
#include <thread>
#include <fstream>
#include <iostream>

std::unordered_map<std::string, std::string> Storage::store;
std::unordered_map<std::string, std::chrono::steady_clock::time_point> Storage::last_access;
std::mutex Storage::store_mutex;

void Storage::load() {
    std::lock_guard<std::mutex> lock(store_mutex);

    std::ifstream file("dump.txt");
    if (file.is_open()) {
        file.close();
        importMap(store, "dump.txt");
        std::cout << "Loaded store from dump.txt\n";

   
        auto now = std::chrono::steady_clock::now();
        for (auto& kv : store)
            last_access[kv.first] = now;
    } else {
        std::cout << "No dump.txt found, starting with empty store\n";
    }
}

void Storage::save() {
    std::lock_guard<std::mutex> lock(store_mutex);
    exportMap(store, "dump.txt");
}

void Storage::updateUsage(const std::string& key) {
    last_access[key] = std::chrono::steady_clock::now();
}

void Storage::evictLRU() {
    if (store.empty()) return;

    auto lru = last_access.begin();
    for (auto it = last_access.begin(); it != last_access.end(); ++it) {
        if (it->second < lru->second) lru = it;
    }

    std::cout << "[Evicting LRU key: " << lru->first << "]\n";
    store.erase(lru->first);
    last_access.erase(lru->first);
}

void Storage::set(const std::string& key, const std::string& value, int ttl_seconds) {
    std::lock_guard<std::mutex> lock(store_mutex);

   
    if (store.size() >= max_keys && store.find(key) == store.end()) {
        if (policy == "noeviction") {
            std::cerr << "MEMORY LIMIT REACHED: noeviction policy, cannot add key '" << key << "'\n";
            return;
        } else if (policy == "allkeys-lru") {
            evictLRU();
        }
    }

    store[key] = value;
    updateUsage(key);


    if (ttl_seconds > 0) {
        std::thread([key, ttl_seconds]() {
            std::this_thread::sleep_for(std::chrono::seconds(ttl_seconds));
            std::lock_guard<std::mutex> lock(Storage::store_mutex);
            Storage::store.erase(key);
            Storage::last_access.erase(key);
            std::cout << "[TTL expired: " << key << "]\n";
        }).detach();
    }
}

bool Storage::get(const std::string& key, std::string& value) {
    std::lock_guard<std::mutex> lock(store_mutex);
    auto it = store.find(key);
    if (it != store.end()) {
        value = it->second;
        updateUsage(key); 
        return true;
    }
    return false;
}

void Storage::show() {
    std::lock_guard<std::mutex> lock(store_mutex);
    std::cout << "Current store contents:\n";
    for (const auto& pair : store) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
    }
}
