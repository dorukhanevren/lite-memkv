#ifndef CACHE_HPP
#define CACHE_HPP

#include <string>
#include <unordered_map>
#include <list>
#include <optional>
#include <mutex>
#include <vector>

class LRUCache {
private:
    size_t capacity;
    // LRU takibi için bağlı liste (key tutar)
    std::list<std::string> lru_list;
    // Hızlı erişim için map: key -> {value, list_iterator}
    std::unordered_map<std::string, std::pair<std::string, std::list<std::string>::iterator>> cache_map;
    mutable std::mutex mtx;

public:
    explicit LRUCache(size_t cap = 1000);

    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    bool del(const std::string& key);
    std::vector<std::string> keys() const;
    void flush();
    size_t size() const;
};

#endif