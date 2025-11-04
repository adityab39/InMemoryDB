#include "store.h"
#include <iostream>

void Store::set(const std::string& key, const std::string& value) {
    std::unique_lock<std::mutex> lock(mutex);

    if (data.find(key) == data.end() && data.size() >= maxSize) {
        std::string lru = usageOrder.back();
        usageOrder.pop_back();
        keyPosition.erase(lru);
        data.erase(lru);
        expiry.erase(lru);
        history.erase(lru);
    }

    data[key] = value;
    history[key].push_back(value);

    if (keyPosition.find(key) != keyPosition.end()) {
        usageOrder.erase(keyPosition[key]);
    }
    usageOrder.push_front(key);
    keyPosition[key] = usageOrder.begin();

    std::cout << "OK\n";
}

std::string Store::get(const std::string& key) {
    std::unique_lock<std::mutex> lock(mutex);
    if (data.find(key) == data.end()) return "";

    if (expiry.count(key) && Clock::now() > expiry[key]) {
        del(key);
        return "";
    }

    usageOrder.erase(keyPosition[key]);
    usageOrder.push_front(key);
    keyPosition[key] = usageOrder.begin();

    return data[key];
}


void Store::del(const std::string& key, bool silent) {
    std::unique_lock<std::mutex> lock(mutex);
    data.erase(key);
    expiry.erase(key);
    history.erase(key);

    if (keyPosition.find(key) != keyPosition.end()) {
        usageOrder.erase(keyPosition[key]);
        keyPosition.erase(key);
    }

    if (!silent) {
        std::cout << "OK\n";
    }
}


void Store::setTTL(const std::string& key, int seconds) {
    std::unique_lock<std::mutex> lock(mutex);
    if (data.find(key) == data.end()) {
        std::cout << "Key does not exist.\n";
        return;
    }

    TimePoint expiryTime = Clock::now() + std::chrono::seconds(seconds);
    expiry[key] = expiryTime;
    expiryQueue.push({expiryTime, key});
    cv.notify_one();
    std::cout << "OK\n";
}

void Store::listKeys() {
    std::unique_lock<std::mutex> lock(mutex);
    if (data.empty()) {
        std::cout << "No keys found.\n";
        return;
    }
    for (const auto& [k, _] : data) {
        std::cout << k << std::endl;  
    }
}

void Store::printHistory(const std::string& key) {
    std::unique_lock<std::mutex> lock(mutex);
    if (!history.count(key)) {
        std::cout << "No history found.\n";
        return;
    }
    for (const auto& value : history[key]) {
        std::cout << value << "\n";
    }
}



void Store::ttlWatcher() {
    while (running) {  
        std::unique_lock<std::mutex> lock(mutex);
        if (expiryQueue.empty()) {
            cv.wait_for(lock, std::chrono::seconds(1));  
        } else {
            auto next = expiryQueue.top();
            auto now = Clock::now();

            if (now >= next.expireAt) {
                expiryQueue.pop();
                lock.unlock();
                del(next.key, true);
            } else {
                cv.wait_until(lock, next.expireAt);
            }
        }
    }
}
