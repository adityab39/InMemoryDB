#include "ttl_manager.h"
#include <chrono>
#include <thread>

TTLManager::TTLManager(Store& store) : store(store), running(false) {}

void TTLManager::start() {
    running = true;
    worker = std::thread([this]() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            store.removeExpiredKeys();
        }
    });
}

void TTLManager::stop() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
}