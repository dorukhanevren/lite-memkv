#include <iostream>
#include <sstream>
#include <vector>
#include "../include/cache.hpp"
#include "../include/persistence.hpp"

void printBanner() {
    std::cout << "\033[1;36m";
    std::cout << "  _ _ _                            _           \n";
    std::cout << " | (_) | |_ ___ _____ ___ _____   | |___ _ _   \n";
    std::cout << " | | |  _| -_|___|     | -_|     |_| |_ / | |  \n";
    std::cout << " |_|_|_| |___|   |_|_|_|___|_|_|_|___|___|___| \n";
    std::cout << " High-Performance In-Memory Key-Value Engine   \n";
    std::cout << "\033[0m" << std::endl;
    std::cout << "Commands: SET <k> <v> | GET <k> | DEL <k> | KEYS | SIZE | FLUSH | EXIT\n\n";
}

int main() {
    printBanner();
    LRUCache kvStore(5000); // 5000 eleman kapasiteli LRU
    PersistenceManager persistence("data.aof");

    // Açılışta önceki verileri diskten geri yükle
    persistence.replay(kvStore);
    std::cout << "\033[32m[+] Engine ready. Restored " << kvStore.size() << " records from disk.\033[0m\n\n";

    std::string line;
    while (true) {
        std::cout << "\033[1;33mlite-memkv>\033[0m ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        for (auto &c : cmd) c = toupper(c);

        if (cmd == "EXIT" || cmd == "QUIT") {
            break;
        } else if (cmd == "SET") {
            std::string key, val;
            if (iss >> key >> val) {
                kvStore.set(key, val);
                persistence.appendCommand("SET " + key + " " + val);
                std::cout << "OK\n";
            } else {
                std::cout << "(error) ERR wrong number of arguments for 'set' command\n";
            }
        } else if (cmd == "GET") {
            std::string key;
            if (iss >> key) {
                auto val = kvStore.get(key);
                if (val.has_value()) {
                    std::cout << "\"" << *val << "\"\n";
                } else {
                    std::cout << "(nil)\n";
                }
            } else {
                std::cout << "(error) ERR wrong number of arguments for 'get' command\n";
            }
        } else if (cmd == "DEL") {
            std::string key;
            if (iss >> key) {
                if (kvStore.del(key)) {
                    persistence.appendCommand("DEL " + key);
                    std::cout << "(integer) 1\n";
                } else {
                    std::cout << "(integer) 0\n";
                }
            }
        } else if (cmd == "KEYS") {
            auto allKeys = kvStore.keys();
            for (size_t i = 0; i < allKeys.size(); ++i) {
                std::cout << i + 1 << ") \"" << allKeys[i] << "\"\n";
            }
        } else if (cmd == "SIZE") {
            std::cout << "(integer) " << kvStore.size() << "\n";
        } else if (cmd == "FLUSH") {
            kvStore.flush();
            persistence.appendCommand("FLUSH");
            std::cout << "OK\n";
        } else {
            std::cout << "(error) ERR unknown command '" << cmd << "'\n";
        }
    }

    return 0;
}