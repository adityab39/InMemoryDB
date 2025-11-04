#include "store.h"
#include <iostream>

void Store::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex);
    data[key] = value;
    expiry.erase(key);
    std::cout << "OK\n";
}

std::string Store::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = data.find(key);
    if (it == data.end()) return "";
    auto exp = expiry.find(key);
    if (exp != expiry.end() && std::chrono::system_clock::now() > exp->second) {
        data.erase(key);
        expiry.erase(key);
        return "";
    }
    return it->second;
}


void Store::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex);
    data.erase(key);
    expiry.erase(key);
    std::cout << "OK\n";
}


void Store::setTTL(const std::string& key, int seconds) {
    std::lock_guard<std::mutex> lock(mutex);
    if (data.find(key) != data.end()) {
        expiry[key] = std::chrono::system_clock::now() + std::chrono::seconds(seconds);
        std::cout << "OK\n";
    } else {
        std::cout << "Key does not exist.\n";
    }
}


void Store::removeExpiredKeys() {
    std::lock_guard<std::mutex> lock(mutex);
    auto now = std::chrono::system_clock::now();
    for (auto it = expiry.begin(); it != expiry.end(); ) {
        if (now > it->second) {
            data.erase(it->first);
            it = expiry.erase(it);
        } else {
            ++it;
        }
    }
}


void Store::listKeys() {
    std::lock_guard<std::mutex> lock(mutex);
    for (const auto& pair : data) {
        std::cout << pair.first << "\n";
    }
}

std::mutex& Store::getMutex() {
    return mutex;
}