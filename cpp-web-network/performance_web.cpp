// ============================================
// 📌 Connection Pooling
// ============================================

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <unordered_map>

// Connection pooling - переиспользование соединений для снижения overhead

class DatabaseConnectionPool {
private:
    struct Connection {
        int conn_id;
        bool in_use = false;
        std::chrono::steady_clock::time_point last_used;
    };
    
    std::vector<Connection> connections;
    std::mutex mutex;
    std::condition_variable cv;
    int max_connections = 10;
    
public:
    DatabaseConnectionPool(int max_conn) : max_connections(max_conn) {
        for (int i = 0; i < max_connections; ++i) {
            connections.push_back({i, false, std::chrono::steady_clock::now()});
        }
    }
    
    int acquire() {
        std::unique_lock lock(mutex);
        cv.wait(lock, [this] {
            return std::any_of(connections.begin(), connections.end(),
                [](const Connection& c) { return !c.in_use; });
        });
        
        for (auto& conn : connections) {
            if (!conn.in_use) {
                conn.in_use = true;
                conn.last_used = std::chrono::steady_clock::now();
                return conn.conn_id;
            }
        }
        return -1;
    }
    
    void release(int conn_id) {
        std::lock_guard lock(mutex);
        for (auto& conn : connections) {
            if (conn.conn_id == conn_id) {
                conn.in_use = false;
                break;
            }
        }
        cv.notify_one();
    }
};

// ============================================
// 📌 Caching Strategies
// ============================================

// LRU Cache (Least Recently Used)
template<typename K, typename V>
class LRUCache {
private:
    size_t capacity;
    std::list<std::pair<K, V>> items;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cache;
    std::mutex mutex;
    
public:
    LRUCache(size_t cap) : capacity(cap) {}
    
    std::optional<V> get(const K& key) {
        std::lock_guard lock(mutex);
        auto it = cache.find(key);
        if (it == cache.end()) return std::nullopt;
        
        items.splice(items.begin(), items, it->second);
        return it->second->second;
    }
    
    void put(const K& key, const V& value) {
        std::lock_guard lock(mutex);
        auto it = cache.find(key);
        
        if (it != cache.end()) {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }
        
        if (items.size() >= capacity) {
            cache.erase(items.back().first);
            items.pop_back();
        }
        
        items.emplace_front(key, value);
        cache[key] = items.begin();
    }
};

// ============================================
// 📌 HTTP Optimization
// ============================================

#include <zlib.h>

class GzipCompressor {
public:
    static std::vector<uint8_t> compress(const std::string& data) {
        z_stream stream{};
        deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
        
        stream.avail_in = data.size();
        stream.next_in = (Bytef*)data.data();
        
        std::vector<uint8_t> compressed;
        compressed.resize(deflateBound(&stream, data.size()));
        
        stream.avail_out = compressed.size();
        stream.next_out = compressed.data();
        
        deflate(&stream, Z_FINISH);
        compressed.resize(stream.total_out);
        deflateEnd(&stream);
        
        return compressed;
    }
};

// ============================================
// 📌 Database Optimization
// ============================================
// • Connection pooling
// • Prepared statements
// • Batch operations
// • Read replicas
// • Sharding
// • Indexing strategies

// ============================================
// 📌 Async Processing
// ============================================
// • Background jobs
// • Task queues
// • Worker pools
// • Job scheduling
// • Priority queues

// ============================================
// 📌 Load Testing
// ============================================
// • Benchmarking tools
// • Stress testing
// • Load patterns
// • Bottleneck identification
// • Performance profiling

// ============================================
// 📌 Scalability
// ============================================
// • Horizontal scaling
// • Vertical scaling
// • Stateless services
// • Session management
// • Database scaling
// • Caching layers