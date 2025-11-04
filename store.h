#pragma once
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

using Clock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<Clock>;

class Store {
private:
    size_t maxSize = 100;

    std::unordered_map<std::string, std::string> data;
    std::unordered_map<std::string, TimePoint> expiry;
    std::unordered_map<std::string, std::vector<std::string>> history;

    std::list<std::string> usageOrder;
    std::unordered_map<std::string, std::list<std::string>::iterator> keyPosition;

    std::mutex mutex;
    std::condition_variable cv;

    struct ExpiryEntry {
        TimePoint expireAt;
        std::string key;
        bool operator>(const ExpiryEntry& other) const {
            return expireAt > other.expireAt;
        }
    };

    std::priority_queue<ExpiryEntry, std::vector<ExpiryEntry>, std::greater<>> expiryQueue;

public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    void del(const std::string& key);
    void setTTL(const std::string& key, int seconds);
    void listKeys();
    void printHistory(const std::string& key);
    void ttlWatcher();
};