#include "store.h"
#include <iostream>

void Store::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex);
    data[key] = value;
    expiry.erase(key);
    std::cout << "OK\n";
}

