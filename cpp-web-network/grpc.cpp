// ============================================
// 📌 gRPC Basics
// ============================================

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <memory>
#include <string>

// gRPC - высокопроизводительный RPC framework от Google
// Преимущества:
// • HTTP/2 transport - мультиплексирование, server push
// • Protocol Buffers - эффективная бинарная сериализация
// • Поддержка streaming (unary, server, client, bidirectional)
// • Кросс-платформенность и кросс-язычность
// • Встроенная аутентификация и load balancing

// ============================================
// 📌 Service Definition (.proto)
// ============================================

// Файл: user_service.proto
/*
syntax = "proto3";

package userservice;

// Сообщения (Messages)
message User {
  int32 id = 1;
  string name = 2;
  string email = 3;
  repeated string tags = 4;
}

message GetUserRequest {
  int32 user_id = 1;
}

message ListUsersRequest {
  int32 page = 1;
  int32 page_size = 2;
}

message CreateUserRequest {
  string name = 1;
  string email = 2;
}

message CreateUserResponse {
  User user = 1;
  string message = 2;
}

message ChatMessage {
  string user = 1;
  string text = 2;
  int64 timestamp = 3;
}

// Сервис
service UserService {
  // Unary RPC - простой запрос-ответ
  rpc GetUser(GetUserRequest) returns (User);
  
  // Server Streaming - сервер отправляет поток ответов
  rpc ListUsers(ListUsersRequest) returns (stream User);
  
  // Client Streaming - клиент отправляет поток запросов
  rpc CreateUsers(stream CreateUserRequest) returns (CreateUserResponse);
  
  // Bidirectional Streaming - двусторонний поток
  rpc Chat(stream ChatMessage) returns (stream ChatMessage);
}
*/

// Компиляция:
// protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` user_service.proto
// protoc --cpp_out=. user_service.proto

// Генерируются файлы:
// • user_service.pb.h/cc - Protocol Buffers классы
// • user_service.grpc.pb.h/cc - gRPC service базовые классы

// ============================================
// 📌 gRPC Communication Patterns
// ============================================

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::ServerReaderWriter;

// --- Unary RPC - простейший паттерн ---
class UserServiceImpl final : public UserService::Service {
public:
    Status GetUser(ServerContext* context,
                  const GetUserRequest* request,
                  User* response) override {
        // Получение ID из запроса
        int user_id = request->user_id();
        
        // Поиск пользователя в БД
        if (user_id <= 0) {
            return Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid user ID");
        }
        
        // Заполнение ответа
        response->set_id(user_id);
        response->set_name("John Doe");
        response->set_email("john@example.com");
        
        return Status::OK;
    }
    
    // --- Server Streaming - сервер отправляет поток ---
    Status ListUsers(ServerContext* context,
                    const ListUsersRequest* request,
                    ServerWriter<User>* writer) override {
        int page = request->page();
        int page_size = request->page_size();
        
        // Отправка пользователей потоком
        for (int i = 0; i < page_size; ++i) {
            User user;
            user.set_id(page * page_size + i);
            user.set_name("User" + std::to_string(i));
            user.set_email("user" + std::to_string(i) + "@example.com");
            
            // Отправка каждого пользователя
            if (!writer->Write(user)) {
                // Клиент отключился
                break;
            }
        }
        
        return Status::OK;
    }
    
    // --- Client Streaming - клиент отправляет поток ---
    Status CreateUsers(ServerContext* context,
                      ServerReader<CreateUserRequest>* reader,
                      CreateUserResponse* response) override {
        CreateUserRequest req;
        int count = 0;
        
        // Чтение потока от клиента
        while (reader->Read(&req)) {
            // Создание пользователя
            std::cout << "Creating user: " << req.name() << std::endl;
            count++;
        }
        
        response->set_message("Created " + std::to_string(count) + " users");
        return Status::OK;
    }
    
    // --- Bidirectional Streaming - двусторонний поток ---
    Status Chat(ServerContext* context,
               ServerReaderWriter<ChatMessage, ChatMessage>* stream) override {
        ChatMessage msg;
        
        // Одновременное чтение и запись
        while (stream->Read(&msg)) {
            std::cout << msg.user() << ": " << msg.text() << std::endl;
            
            // Echo обратно
            ChatMessage response;
            response.set_user("Server");
            response.set_text("Echo: " + msg.text());
            response.set_timestamp(std::time(nullptr));
            
            stream->Write(response);
        }
        
        return Status::OK;
    }
};

// ============================================
// 📌 C++ gRPC Server
// ============================================

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    UserServiceImpl service;
    
    ServerBuilder builder;
    
    // Слушаем на порту без SSL
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    // Регистрация сервиса
    builder.RegisterService(&service);
    
    // Создание и запуск сервера
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    
    // Ожидание завершения
    server->Wait();
}

// ============================================
// 📌 C++ gRPC Client
// ============================================

class UserServiceClient {
private:
    std::unique_ptr<UserService::Stub> stub_;
    
public:
    UserServiceClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(UserService::NewStub(channel)) {}
    
    // Unary call
    User GetUser(int user_id) {
        GetUserRequest request;
        request.set_user_id(user_id);
        
        User response;
        grpc::ClientContext context;
        
        // Установка deadline (timeout)
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(5));
        
        // Вызов RPC
        Status status = stub_->GetUser(&context, request, &response);
        
        if (status.ok()) {
            return response;
        } else {
            std::cerr << "RPC failed: " << status.error_message() << std::endl;
            throw std::runtime_error(status.error_message());
        }
    }
    
    // Server streaming call
    std::vector<User> ListUsers(int page, int page_size) {
        ListUsersRequest request;
        request.set_page(page);
        request.set_page_size(page_size);
        
        grpc::ClientContext context;
        std::unique_ptr<grpc::ClientReader<User>> reader(
            stub_->ListUsers(&context, request));
        
        std::vector<User> users;
        User user;
        
        // Чтение потока от сервера
        while (reader->Read(&user)) {
            users.push_back(user);
        }
        
        Status status = reader->Finish();
        if (!status.ok()) {
            std::cerr << "ListUsers rpc failed." << std::endl;
        }
        
        return users;
    }
    
    // Client streaming call
    std::string CreateUsers(const std::vector<CreateUserRequest>& requests) {
        grpc::ClientContext context;
        CreateUserResponse response;
        
        std::unique_ptr<grpc::ClientWriter<CreateUserRequest>> writer(
            stub_->CreateUsers(&context, &response));
        
        // Отправка потока запросов
        for (const auto& req : requests) {
            if (!writer->Write(req)) {
                break;
            }
        }
        
        writer->WritesDone();
        Status status = writer->Finish();
        
        if (status.ok()) {
            return response.message();
        } else {
            return "RPC failed";
        }
    }
};

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