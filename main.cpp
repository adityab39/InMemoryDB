#include<iostream>
#include<sstream>
#include "store.h"
#include "ttl_manager.h"


int main(){
    Store store;
    TTLManager ttlManager(store);
    ttlManager.start();

    std::string input;
    std::cout << "InMemoryDB – C++ Key-Value Store with TTL\n";
    std::cout << "Type commands like: SET key value | GET key | DEL key | EXPIRE key seconds | KEYS | EXIT\n";

    while (true){
        std ::cout << "> ";
        std::getline(std::cin, input);
        std::istringstream ss(input);
        std::string command;
        ss >> command;

        if(command == "SET"){
            std :: string key,value;
            ss >> key >> value;
            store.set(key,value);
        }else if (command == "GET"){
            std :: string key;
            ss >> key;
            std :: string result = store.get(key);
            if(result.empty()){
                std::cout << "NULL\n";
            }else{
                std::cout << result << "\n";
            }
        }else if (command == "DEL"){
            std :: string key;
            ss >> key;
            store.del(key); 
        }else if (command == "EXPIRE"){
            std :: string key;
            int seconds;
            ss >> key >> seconds;
            store.setTTL(key,seconds);
        }else if (command == "KEYS"){
            store.listKeys();
        }else if (command == "EXIT"){
            break;
        }else{
            std :: cout << "Unknown command.\n";
        }


        ttlManager.stop()
        return 0;
    }
}   