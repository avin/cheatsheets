// ============================================
// 📌 REST Principles
// ============================================
// • Resources и URIs
// • HTTP methods (GET, POST, PUT, DELETE, PATCH)
// • Stateless communication
// • HATEOAS (Hypermedia)
// • Richardson Maturity Model

// ============================================
// 📌 URL Design
// ============================================
// • Resource naming (/users, /posts)
// • Singular vs plural
// • Nested resources (/users/1/posts)
// • Query parameters (?page=1&limit=10)
// • Filtering, sorting, pagination
// • Versioning (/api/v1/)

// ============================================
// 📌 HTTP Methods Semantics
// ============================================

// --- GET ---
// • Idempotent
// • Safe
// • Caching
// • Query parameters

// --- POST ---
// • Create resource
// • Non-idempotent
// • 201 Created response
// • Location header

// --- PUT ---
// • Update (replace) resource
// • Idempotent
// • Whole resource update
// • 200 OK or 204 No Content

// --- PATCH ---
// • Partial update
// • JSON Patch format
// • Merge Patch format

// --- DELETE ---
// • Remove resource
// • Idempotent
// • 204 No Content or 200 OK

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