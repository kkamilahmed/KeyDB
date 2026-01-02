#ifndef STORAGE_H
#define STORAGE_H

#include <unordered_map>
#include <mutex>
#include <string>
#include <chrono>

class Storage {
public:
    Storage(size_t maxKeys = 1000, const std::string& evictionPolicy = "noeviction")
        : max_keys(maxKeys), policy(evictionPolicy) {}

    void load();
    void save();

    void set(const std::string& key, const std::string& value, int ttl_seconds = 0);
    bool get(const std::string& key, std::string& value);
    void show();

private:
    void evictLRU();
    void updateUsage(const std::string& key);

    size_t max_keys;
    std::string policy; // "noeviction" or "allkeys-lru"

    // Shared storage
    static std::unordered_map<std::string, std::string> store;
    static std::unordered_map<std::string, std::chrono::steady_clock::time_point> last_access;
    static std::mutex store_mutex;
};

#endif // STORAGE_H
