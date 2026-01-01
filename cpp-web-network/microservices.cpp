// ============================================
// 📌 Service Communication
// ============================================

#include <string>
#include <memory>
#include <chrono>
#include <atomic>

// Микросервисная архитектура - разбиение системы на независимые сервисы

// Способы взаимодействия:
// 1. Synchronous - HTTP/gRPC (просто, но coupling)
// 2. Asynchronous - Message Queues (RabbitMQ, Kafka)
// 3. Event-driven - Event Bus
// 4. Service Mesh - Istio, Linkerd

// ============================================
// 📌 Circuit Breaker Pattern
// ============================================

enum class CircuitState {
    CLOSED,      // Нормальное состояние, запросы проходят
    OPEN,        // Открыт, запросы блокируются
    HALF_OPEN    // Полуоткрыт, пробные запросы
};

class CircuitBreaker {
private:
    CircuitState state = CircuitState::CLOSED;
    int failure_count = 0;
    int success_count = 0;
    int failure_threshold = 5;  // После 5 ошибок -> OPEN
    int success_threshold = 2;  // После 2 успехов в HALF_OPEN -> CLOSED
    std::chrono::seconds open_timeout{60}; // Через 60с OPEN -> HALF_OPEN
    std::chrono::steady_clock::time_point last_failure_time;
    
public:
    template<typename Func>
    auto execute(Func&& func) -> decltype(func()) {
        if (state == CircuitState::OPEN) {
            // Проверка timeout
            auto now = std::chrono::steady_clock::now();
            if (now - last_failure_time > open_timeout) {
                state = CircuitState::HALF_OPEN;
                success_count = 0;
            } else {
                throw std::runtime_error("Circuit breaker is OPEN");
            }
        }
        
        try {
            auto result = func();
            on_success();
            return result;
        } catch (...) {
            on_failure();
            throw;
        }
    }
    
private:
    void on_success() {
        failure_count = 0;
        
        if (state == CircuitState::HALF_OPEN) {
            success_count++;
            if (success_count >= success_threshold) {
                state = CircuitState::CLOSED;
            }
        }
    }
    
    void on_failure() {
        failure_count++;
        last_failure_time = std::chrono::steady_clock::now();
        
        if (failure_count >= failure_threshold) {
            state = CircuitState::OPEN;
        }
    }
};

// ============================================
// 📌 Retry Pattern with Exponential Backoff
// ============================================

class RetryPolicy {
private:
    int max_retries = 3;
    std::chrono::milliseconds initial_delay{100};
    double backoff_multiplier = 2.0;
    std::chrono::milliseconds max_delay{10000};
    
public:
    template<typename Func>
    auto execute(Func&& func) -> decltype(func()) {
        int attempt = 0;
        auto delay = initial_delay;
        
        while (true) {
            try {
                return func();
            } catch (const std::exception& e) {
                attempt++;
                
                if (attempt >= max_retries) {
                    throw; // Превышен лимит попыток
                }
                
                // Exponential backoff + jitter
                auto jitter = std::chrono::milliseconds(rand() % 100);
                std::this_thread::sleep_for(delay + jitter);
                
                delay = std::min(std::chrono::milliseconds(
                    static_cast<long long>(delay.count() * backoff_multiplier)
                ), max_delay);
            }
        }
    }
};

// ============================================
// 📌 Service Discovery
// ============================================

struct ServiceInstance {
    std::string id;
    std::string host;
    int port;
    std::unordered_map<std::string, std::string> metadata;
    bool healthy = true;
};

class ServiceRegistry {
private:
    std::unordered_map<std::string, std::vector<ServiceInstance>> services;
    std::mutex mutex;
    
public:
    // Регистрация сервиса
    void register_service(const std::string& service_name, const ServiceInstance& instance) {
        std::lock_guard lock(mutex);
        services[service_name].push_back(instance);
    }
    
    // Получение здоровых инстансов
    std::vector<ServiceInstance> get_instances(const std::string& service_name) {
        std::lock_guard lock(mutex);
        
        std::vector<ServiceInstance> healthy_instances;
        auto it = services.find(service_name);
        
        if (it != services.end()) {
            for (const auto& instance : it->second) {
                if (instance.healthy) {
                    healthy_instances.push_back(instance);
                }
            }
        }
        
        return healthy_instances;
    }
    
    // Load balancing - round robin
    std::optional<ServiceInstance> get_next_instance(const std::string& service_name) {
        static std::unordered_map<std::string, std::atomic<int>> counters;
        
        auto instances = get_instances(service_name);
        if (instances.empty()) return std::nullopt;
        
        int index = counters[service_name]++ % instances.size();
        return instances[index];
    }
};
// • Idempotency

// --- Saga Pattern ---
// • Choreography
// • Orchestration
// • Compensating transactions
// • Distributed transactions

// --- Event Sourcing ---
// • Event store
// • Event replay
// • Projections
// • CQRS

// ============================================
// 📌 Distributed Tracing
// ============================================
// • OpenTelemetry
// • Jaeger / Zipkin
// • Trace context propagation
// • Span creation
// • Sampling

// ============================================
// 📌 Logging и Monitoring
// ============================================
// • Structured logging (JSON)
// • Log aggregation
// • Metrics (Prometheus)
// • Alerts
// • Health checks

// ============================================
// 📌 Configuration Management
// ============================================
// • Environment variables
// • Configuration server
// • Feature flags
// • Secret management (Vault)