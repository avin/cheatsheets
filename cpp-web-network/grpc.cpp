// ============================================
// 📌 gRPC Basics
// ============================================
// • Protocol Buffers schema
// • Service definition
// • Code generation (protoc)
// • HTTP/2 transport
// • Binary protocol efficiency

// ============================================
// 📌 Service Definition (.proto)
// ============================================

/*
syntax = "proto3";

service UserService {
  rpc GetUser(GetUserRequest) returns (User);
  rpc ListUsers(ListUsersRequest) returns (stream User);
  rpc CreateUser(stream CreateUserRequest) returns (CreateUserResponse);
  rpc Chat(stream ChatMessage) returns (stream ChatMessage);
}
*/

// ============================================
// 📌 gRPC Communication Patterns
// ============================================

// --- Unary RPC ---
// • Simple request-response
// • Client sends one message
// • Server responds with one message
// • Example: GetUser

// --- Server Streaming ---
// • Client sends one request
// • Server sends stream of responses
// • Example: ListUsers, real-time updates

// --- Client Streaming ---
// • Client sends stream of requests
// • Server responds with one message
// • Example: File upload, batch processing

// --- Bidirectional Streaming ---
// • Both send streams
// • Independent read/write
// • Example: Chat, real-time collaboration

// ============================================
// 📌 C++ gRPC Server
// ============================================

// --- Synchronous Server ---
class UserServiceImpl : public UserService::Service {
    // • Override service methods
    // • Status return type
    // • Context parameter
    // • Request/Response parameters
};

// --- Asynchronous Server ---
// • CompletionQueue
// • Tag-based callbacks
// • Concurrent request handling
// • Better scalability

// ============================================
// 📌 C++ gRPC Client
// ============================================

// --- Blocking Client ---
// • Stub creation
// • Simple method calls
// • Deadline/Timeout
// • Metadata

// --- Async Client ---
// • CompletionQueue
// • Non-blocking calls
// • Callback-based
// • Better throughput

// ============================================
// 📌 Streaming Examples
// ============================================

// --- Server Streaming ---
Status ListUsers(ServerContext* context,
                 const ListUsersRequest* request,
                 ServerWriter<User>* writer) {
    // • writer->Write(user) для каждого
    // • Return Status::OK
}

// --- Client Streaming ---
Status CreateUsers(ServerContext* context,
                   ServerReader<CreateUserRequest>* reader,
                   CreateUsersResponse* response) {
    // • reader->Read(&request) в цикле
    // • Batch processing
}

// --- Bidirectional Streaming ---
Status Chat(ServerContext* context,
            ServerReaderWriter<ChatMessage, ChatMessage>* stream) {
    // • stream->Read() и stream->Write()
    // • Concurrent handling
}

// ============================================
// 📌 Metadata
// ============================================
// • Custom headers
// • Authentication tokens
// • Tracing IDs
// • Client context
// • Server context

// ============================================
// 📌 Interceptors
// ============================================
// • Client interceptors
// • Server interceptors
// • Logging
// • Authentication
// • Metrics collection
// • Request/response modification

// ============================================
// 📌 Error Handling
// ============================================
// • Status codes (OK, CANCELLED, NOT_FOUND, etc.)
// • Error details
// • Rich error model (google.rpc.Status)
// • Custom error handling

// ============================================
// 📌 Deadlines и Timeouts
// ============================================
// • Per-call deadlines
// • Propagation across services
// • Cancellation
// • Context management

// ============================================
// 📌 Authentication
// ============================================
// • SSL/TLS
// • Token-based auth
// • Mutual TLS (mTLS)
// • Metadata-based auth
// • OAuth integration

// ============================================
// 📌 Load Balancing
// ============================================
// • Client-side load balancing
// • Pick first, round robin
// • Resolver integration
// • Service discovery
// • Health checking

// ============================================
// 📌 Performance Optimization
// ============================================
// • HTTP/2 multiplexing
// • Connection pooling
// • Message compression
// • Arena allocation
// • Zero-copy optimization

// ============================================
// 📌 Advanced Features
// ============================================
// • Reflection API
// • Health checking
// • Server reflection
// • Custom credentials
// • Channel arguments
// • Keepalive

// ============================================
// 📌 Testing
// ============================================
// • Mock gRPC services
// • In-process testing
// • Integration tests
// • Load testing