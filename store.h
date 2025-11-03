#pragma once
#include <unordered_map>
#include <string>
#include <chrono>
#include <mutex>

class Store {
private:
    std::unordered_map<std::string, std::string> data;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> expiry;
    std::mutex mutex;

public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    void del(const std::string& key);
    void setTTL(const std::string& key, int seconds);
    void removeExpiredKeys();
    void listKeys();
    std::mutex& getMutex();  
};