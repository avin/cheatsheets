// ============================================
// 📌 REST Principles
// ============================================

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <functional>
#include <regex>

// REST (REpresentational State Transfer) - архитектурный стиль для создания веб-сервисов

// --- Richardson Maturity Model ---
// Level 0: Single URI, single method (RPC style)
// Level 1: Multiple URIs, single method
// Level 2: Multiple URIs, HTTP methods (это уже REST)
// Level 3: HATEOAS (Hypermedia As The Engine Of Application State)

// --- REST Constraints ---
// 1. Client-Server separation
// 2. Stateless - каждый запрос содержит всю необходимую информацию
// 3. Cacheable - ответы должны явно указывать кэшируемость
// 4. Uniform Interface - единообразный интерфейс
// 5. Layered System - клиент не знает, с каким слоем взаимодействует
// 6. Code on Demand (optional) - сервер может отправлять исполняемый код

// ============================================
// 📌 URL Design
// ============================================

// --- Resource Naming Best Practices ---
namespace RestUrlDesign {

// ✅ GOOD: Используй существительные, не глаголы
// GET /api/users           - получить пользователей
// POST /api/users          - создать пользователя
// GET /api/users/123       - получить пользователя 123

// ❌ BAD: Не используй глаголы в URL
// GET /api/getUsers
// POST /api/createUser
// POST /api/deleteUser/123

// ✅ GOOD: Множественное число для коллекций
// GET /api/users
// GET /api/posts
// GET /api/comments

// ✅ GOOD: Вложенные ресурсы
// GET /api/users/123/posts         - посты пользователя 123
// GET /api/users/123/posts/456     - пост 456 пользователя 123
// POST /api/users/123/posts        - создать пост для пользователя 123

// ⚠️ Избегай глубокой вложенности (> 2 уровней)
// ❌ BAD: /api/users/123/posts/456/comments/789/replies
// ✅ GOOD: /api/comments/789/replies

// Query параметры для фильтрации, сортировки, пагинации
struct QueryParams {
    // Pagination
    std::optional<int> page;      // ?page=2
    std::optional<int> limit;     // ?limit=20
    std::optional<std::string> cursor; // ?cursor=abc123
    
    // Filtering
    std::unordered_map<std::string, std::string> filters; // ?status=active&role=admin
    
    // Sorting
    std::vector<std::string> sort_fields; // ?sort=-created_at,name
    
    // Fields selection (sparse fieldsets)
    std::optional<std::string> fields; // ?fields=id,name,email
    
    // Includes (related resources)
    std::vector<std::string> includes; // ?include=posts,comments
};

// Парсинг query параметров
QueryParams parse_query(const std::string& query_string) {
    QueryParams params;
    
    std::istringstream stream(query_string);
    std::string pair;
    
    while (std::getline(stream, pair, '&')) {
        auto eq_pos = pair.find('=');
        if (eq_pos != std::string::npos) {
            auto key = pair.substr(0, eq_pos);
            auto value = pair.substr(eq_pos + 1);
            
            if (key == "page") {
                params.page = std::stoi(value);
            } else if (key == "limit") {
                params.limit = std::stoi(value);
            } else if (key == "cursor") {
                params.cursor = value;
            } else if (key == "sort") {
                // Разделение по запятой: sort=-created_at,name
                std::istringstream sort_stream(value);
                std::string field;
                while (std::getline(sort_stream, field, ',')) {
                    params.sort_fields.push_back(field);
                }
            } else if (key == "include") {
                std::istringstream inc_stream(value);
                std::string resource;
                while (std::getline(inc_stream, resource, ',')) {
                    params.includes.push_back(resource);
                }
            } else if (key == "fields") {
                params.fields = value;
            } else {
                params.filters[key] = value;
            }
        }
    }
    
    return params;
}

} // namespace RestUrlDesign

// ============================================
// 📌 HTTP Methods Semantics
// ============================================

namespace RestHttpMethods {

// --- GET: Получение ресурса ---
// • Safe (безопасный) - не изменяет состояние сервера
// • Idempotent (идемпотентный) - повторные запросы дают тот же результат
// • Кэшируемый

void handle_get_users() {
    // GET /api/users - получить список пользователей
    // Response: 200 OK
    // [
    //   {"id": 1, "name": "Alice"},
    //   {"id": 2, "name": "Bob"}
    // ]
}

void handle_get_user_by_id(int user_id) {
    // GET /api/users/123 - получить пользователя
    // Response: 200 OK
    // {"id": 123, "name": "Alice", "email": "alice@example.com"}
    
    // Если не найден:
    // Response: 404 Not Found
    // {"error": "User not found"}
}

// --- POST: Создание ресурса ---
// • Non-idempotent - повторные запросы создают новые ресурсы
// • Response: 201 Created
// • Location header с URL созданного ресурса

void handle_post_user(const std::string& json_body) {
    // POST /api/users
    // Request Body: {"name": "Charlie", "email": "charlie@example.com"}
    
    // Response: 201 Created
    // Location: /api/users/3
    // {"id": 3, "name": "Charlie", "email": "charlie@example.com"}
}

// --- PUT: Полная замена ресурса ---
// • Idempotent - повторные запросы с теми же данными дают тот же результат
// • Требует полное представление ресурса
// • Response: 200 OK или 204 No Content

void handle_put_user(int user_id, const std::string& json_body) {
    // PUT /api/users/123
    // Request Body: {"name": "Alice Updated", "email": "alice_new@example.com"}
    
    // Response: 200 OK
    // {"id": 123, "name": "Alice Updated", "email": "alice_new@example.com"}
    
    // Если ресурс не существует, можно создать:
    // Response: 201 Created (опционально)
}

// --- PATCH: Частичное обновление ---
// • Только измененные поля
// • Два формата: JSON Patch (RFC 6902) и Merge Patch (RFC 7386)

// JSON Patch format (более мощный)
void handle_patch_user_json_patch(int user_id, const std::string& json_patch) {
    // PATCH /api/users/123
    // Request Body (JSON Patch):
    // [
    //   {"op": "replace", "path": "/email", "value": "newemail@example.com"},
    //   {"op": "add", "path": "/phone", "value": "+1234567890"},
    //   {"op": "remove", "path": "/temporary_field"}
    // ]
    
    // Operations: add, remove, replace, move, copy, test
}

// Merge Patch format (проще, но менее гибкий)
void handle_patch_user_merge_patch(int user_id, const std::string& json_merge) {
    // PATCH /api/users/123
    // Content-Type: application/merge-patch+json
    // Request Body:
    // {"email": "newemail@example.com"}  // Только измененные поля
    
    // Response: 200 OK
    // {"id": 123, "name": "Alice", "email": "newemail@example.com"}
}

// --- DELETE: Удаление ресурса ---
// • Idempotent - повторные DELETE безопасны
// • Response: 204 No Content или 200 OK с телом

void handle_delete_user(int user_id) {
    // DELETE /api/users/123
    
    // Response: 204 No Content (без тела)
    // или
    // Response: 200 OK
    // {"message": "User deleted successfully"}
    
    // Повторный DELETE того же ресурса:
    // Response: 404 Not Found (или 204/200 если идемпотентность важнее)
}

// --- HEAD: Метаданные без тела ---
// • Идентичен GET, но без body
// • Используется для проверки существования ресурса

void handle_head_user(int user_id) {
    // HEAD /api/users/123
    // Response: 200 OK (только headers, без body)
    // Content-Length: 150
    // Last-Modified: Wed, 21 Oct 2024 07:28:00 GMT
}

// --- OPTIONS: Доступные методы ---
// • CORS preflight requests
// • Определение поддерживаемых методов

void handle_options_users() {
    // OPTIONS /api/users
    // Response: 200 OK
    // Allow: GET, POST, OPTIONS
    // Access-Control-Allow-Methods: GET, POST, PUT, DELETE
}

} // namespace RestHttpMethods

// ============================================
// 📌 Response Codes
// ============================================

// --- Success (2xx) ---
// • 200 OK
// • 201 Created
// • 204 No Content
// • 206 Partial Content

// --- Redirection (3xx) ---
// • 301 Moved Permanently
// • 302 Found
// • 304 Not Modified

// --- Client Errors (4xx) ---
// • 400 Bad Request
// • 401 Unauthorized
// • 403 Forbidden
// • 404 Not Found
// • 409 Conflict
// • 422 Unprocessable Entity
// • 429 Too Many Requests

// --- Server Errors (5xx) ---
// • 500 Internal Server Error
// • 502 Bad Gateway
// • 503 Service Unavailable

// ============================================
// 📌 Request/Response Format
// ============================================

// --- JSON API Specification ---
// • data, errors, meta structure
// • Resource objects
// • Relationships
// • Links
// • Included resources

// --- Error Responses ---
struct ErrorResponse {
    // • error code
    // • error message
    // • details/validation errors
    // • trace_id для debugging
};

// ============================================
// 📌 Pagination
// ============================================

// --- Offset-based ---
// • ?page=2&limit=20
// • Total count
// • Page metadata

// --- Cursor-based ---
// • ?after=cursor_id
// • Stable sorting
// • Large datasets

// --- Link headers ---
// • next, prev, first, last links
// • RFC 5988

// ============================================
// 📌 Filtering и Sorting
// ============================================
// • Query parameters (?filter[status]=active)
// • Multiple filters
// • Logical operators (AND, OR)
// • Sorting (?sort=-created_at)
// • Multi-field sorting

// ============================================
// 📌 Authentication
// ============================================

// --- API Keys ---
// • Header: X-API-Key
// • Query parameter
// • Rotation strategy

// --- JWT (JSON Web Tokens) ---
// • Header.Payload.Signature
// • Bearer token
// • Claims (sub, exp, iat)
// • Token validation
// • Refresh tokens

// --- OAuth 2.0 ---
// • Authorization Code flow
// • Client Credentials flow
// • Resource Owner Password flow
// • Token endpoint
// • Scopes

// ============================================
// 📌 Rate Limiting
// ============================================
// • Token bucket algorithm
// • Leaky bucket algorithm
// • Fixed window
// • Sliding window
// • Headers (X-RateLimit-*)
// • 429 Too Many Requests

// ============================================
// 📌 Caching
// ============================================
// • ETags
// • Last-Modified / If-Modified-Since
// • Cache-Control headers
// • Conditional requests
// • Validation
// • Invalidation strategies

// ============================================
// 📌 CORS (Cross-Origin Resource Sharing)
// ============================================
// • Preflight requests (OPTIONS)
// • Access-Control-Allow-Origin
// • Access-Control-Allow-Methods
// • Access-Control-Allow-Headers
// • Credentials handling

// ============================================
// 📌 API Versioning
// ============================================
// • URL versioning (/api/v1/)
// • Header versioning (Accept: application/vnd.api+json;version=1)
// • Query parameter (?version=1)
// • Deprecation strategy
// • Migration guides

// ============================================
// 📌 Documentation
// ============================================
// • OpenAPI/Swagger specification
// • API reference
// • Examples
// • Interactive documentation
// • SDK generation

// ============================================
// 📌 Testing
// ============================================
// • Unit tests для endpoints
// • Integration tests
// • Contract tests
// • Load tests
// • Mock servers