#include "../include/cache.hpp"

LRUCache::LRUCache(size_t cap) : capacity(cap) {}

void LRUCache::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = cache_map.find(key);

    if (it != cache_map.end()) {
        // Zaten varsa değer güncelle ve listenin en önüne taşı
        it->second.first = value;
        lru_list.erase(it->second.second);
        lru_list.push_front(key);
        it->second.second = lru_list.begin();
        return;
    }

    // Kapasite dolduysa en eski elemanı (en arkadaki) sil
    if (cache_map.size() >= capacity) {
        std::string oldest_key = lru_list.back();
        lru_list.pop_back();
        cache_map.erase(oldest_key);
    }

    // Yeni elemanı başa ekle
    lru_list.push_front(key);
    cache_map[key] = {value, lru_list.begin()};
}

std::optional<std::string> LRUCache::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = cache_map.find(key);
    if (it == cache_map.end()) {
        return std::nullopt;
    }

    // Erişildiği için listenin en önüne (en yeni) taşı
    lru_list.erase(it->second.second);
    lru_list.push_front(key);
    it->second.second = lru_list.begin();

    return it->second.first;
}

bool LRUCache::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = cache_map.find(key);
    if (it == cache_map.end()) {
        return false;
    }

    lru_list.erase(it->second.second);
    cache_map.erase(it);
    return true;
}

std::vector<std::string> LRUCache::keys() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::string> key_list;
    key_list.reserve(cache_map.size());
    for (const auto& pair : cache_map) {
        key_list.push_back(pair.first);
    }
    return key_list;
}

void LRUCache::flush() {
    std::lock_guard<std::mutex> lock(mtx);
    cache_map.clear();
    lru_list.clear();
}

size_t LRUCache::size() const {
    std::lock_guard<std::mutex> lock(mtx);
    return cache_map.size();
}