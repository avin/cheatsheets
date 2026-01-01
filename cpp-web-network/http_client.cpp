// ============================================
// 📌 HTTP Protocol Basics
// ============================================
// • HTTP/1.1 structure
// • Request methods (GET, POST, PUT, DELETE, PATCH, etc.)
// • Headers
// • Status codes
// • Content-Type / Content-Length
// • Transfer-Encoding (chunked)
// • Keep-Alive connections

// ============================================
// 📌 Simple HTTP Client
// ============================================

// --- Synchronous HTTP GET ---
// • URL parsing
// • DNS resolution
// • TCP connection
// • Sending request
// • Receiving response
// • Header parsing
// • Body extraction

// --- HTTP POST with Body ---
// • application/x-www-form-urlencoded
// • application/json
// • multipart/form-data
// • Content-Length calculation

// ============================================
// 📌 HTTP Response Handling
// ============================================
// • Status code checking
// • Header parsing (case-insensitive)
// • Chunked transfer decoding
// • Content-Encoding (gzip, deflate)
// • Redirect handling (301, 302, 307, 308)
// • Error responses (4xx, 5xx)

// ============================================
// 📌 Modern HTTP Client Class
// ============================================

// --- HTTP Client with RAII ---
class HTTPClient {
    // • Connection pooling
    // • Keep-alive support
    // • Timeout configuration
    // • Retry logic
    // • Custom headers
    // • Cookie handling
    // • Authentication (Basic, Bearer)
};

// --- Fluent Interface ---
// • builder.get(url).header("X-Custom", "value").send()
// • Method chaining
// • std::expected<Response, Error> return

// ============================================
// 📌 Async HTTP Client
// ============================================
// • Coroutine-based requests
// • Future-based API
// • Callback-based API
// • Parallel requests
// • Request batching

// ============================================
// 📌 REST API Client Patterns
// ============================================

// --- JSON API Client ---
// • Automatic JSON serialization/deserialization
// • Type-safe requests
// • Response models
// • Error handling

// --- Example: GitHub API Client ---
// • Authentication (Personal Access Token)
// • Pagination handling
// • Rate limiting
// • ETags для caching

// ============================================
// 📌 HTTP/2 Support
// ============================================
// • Binary framing
// • Multiplexing
// • Server push
// • Header compression (HPACK)
// • Stream priorities
// • Flow control

// ============================================
// 📌 HTTPS / TLS
// ============================================

// --- OpenSSL Integration ---
// • SSL context initialization
// • Certificate verification
// • Hostname validation
// • TLS handshake
// • Encrypted send/recv

// --- Modern TLS Practices ---
// • TLS 1.2 / 1.3
// • Certificate pinning
// • ALPN (Application-Layer Protocol Negotiation)
// • SNI (Server Name Indication)

// ============================================
// 📌 Connection Pooling
// ============================================
// • Pool management
// • Connection reuse
// • Max connections per host
// • Idle connection timeout
// • Connection health checks

// ============================================
// 📌 Advanced Features
// ============================================
// • Streaming downloads
// • Upload progress tracking
// • Compression (gzip, brotli)
// • Proxy support (HTTP, SOCKS5)
// • Custom DNS resolver
// • IPv6 support

// ============================================
// 📌 Testing и Mocking
// ============================================
// • Mock HTTP server
// • Request recording/replay
// • Stubbing responses
// • Integration tests