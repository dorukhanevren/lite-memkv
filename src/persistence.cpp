// src/persistence.cpp
#include "../include/persistence.hpp"
#include <sstream>
#include <iostream>

PersistenceManager::PersistenceManager(const std::string& path) : filepath(path) {
    aof_out.open(filepath, std::ios::app);
}

PersistenceManager::~PersistenceManager() {
    if (aof_out.is_open()) {
        aof_out.close();
    }
}

void PersistenceManager::appendCommand(const std::string& command) {
    if (aof_out.is_open()) {
        aof_out << command << "\n";
        aof_out.flush();
    }
}

void PersistenceManager::replay(LRUCache& cache) {
    std::ifstream in(filepath);
    if (!in.is_open()) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string action, key, val;
        iss >> action >> key;
        if (action == "SET") {
            iss >> val;
            cache.set(key, val);
        } else if (action == "DEL") {
            cache.del(key);
        } else if (action == "FLUSH") {
            cache.flush();
        }
    }
}