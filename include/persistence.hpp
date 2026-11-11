#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include <string>
#include <fstream>
#include "cache.hpp"

class PersistenceManager {
private:
    std::string filepath;
    std::ofstream aof_out;

public:
    explicit PersistenceManager(const std::string& path);
    ~PersistenceManager();

    void appendCommand(const std::string& command);
    void replay(LRUCache& cache);
};

#endif