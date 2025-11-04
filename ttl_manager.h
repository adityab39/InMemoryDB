#pragma once
#include <thread>
#include <atomic>
#include "store.h"

class TTLManager {
private:
    Store& store;
    std::thread worker;
    std::atomic<bool> running;

public:
    TTLManager(Store& store);
    void start();
    void stop();
};