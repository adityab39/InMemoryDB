#include <iostream>
#include <sstream>
#include <thread>
#include "store.h"
#include <unistd.h>

class TTLManager {
    Store& store;
    std::thread worker;
public:
    TTLManager(Store& s) : store(s) {}
    void start() {
        worker = std::thread([this]() { store.ttlWatcher(); });
    }
    void stop() {
        if (worker.joinable()) worker.detach();
    }
};

int main() {
    Store store;
    TTLManager ttl(store);
    ttl.start();

    std::string input;
    std::cout << "InMemoryDB – TTL, LRU, Versioned Key Store\n";
    std::cout << "Available Commands:\n";
    std::cout << "  SET <key> <value>      → Store a value\n";
    std::cout << "  GET <key>              → Retrieve a value\n";
    std::cout << "  DEL <key>              → Delete a key\n";
    std::cout << "  EXPIRE <key> <seconds> → Auto-delete key after TTL\n";
    std::cout << "  HISTORY <key>          → View all previous values\n";
    std::cout << "  KEYS                   → List all active keys\n";
    std::cout << "  EXIT                   → Quit the program\n";

    while (true) {

        if (isatty(fileno(stdin))) {  
            std::cout << "> ";
        }

        if (!std::getline(std::cin, input)) {
            break;  
        }

        std::istringstream ss(input);
        std::string command;
        ss >> command;

        if (command == "SET") {
            std::string key, value;
            ss >> key >> value;
            store.set(key, value);
        } else if (command == "GET") {
            std::string key;
            ss >> key;
            std::string result = store.get(key);
            if (result.empty()) std::cout << "(null)\n";
            else std::cout << result << "\n";
        } else if (command == "DEL") {
            std::string key;
            ss >> key;
            store.del(key);
        } else if (command == "EXPIRE") {
            std::string key;
            int seconds;
            ss >> key >> seconds;
            store.setTTL(key, seconds);
        } else if (command == "KEYS") {
            store.listKeys();
        } else if (command == "HISTORY") {
            std::string key;
            ss >> key;
            store.printHistory(key);
        } else if (command == "EXIT") {
            std::cout << "Exiting InMemoryDB...\n";
            break; 
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    ttl.stop();
    return 0;
}