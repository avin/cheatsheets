# 🌐 C++23 Web & Network Programming Guide

> Полное руководство по созданию современных сетевых и веб-приложений на C++

## 🎯 Core Topics

### Network Fundamentals
- [network_basics.cpp](network_basics.cpp) - TCP/UDP, сокеты, RAII wrappers
- [async_io.cpp](async_io.cpp) - Event loops, coroutines, epoll/io_uring
- [ssl_tls.cpp](ssl_tls.cpp) - Безопасность, TLS, криптография

### HTTP & Web
- [http_client.cpp](http_client.cpp) - REST клиент, HTTP/2, connection pooling
- [http_server.cpp](http_server.cpp) - Web framework, routing, middleware
- [websocket.cpp](websocket.cpp) - Real-time коммуникация, WebSocket protocol

### APIs & Protocols
- [rest_api.cpp](rest_api.cpp) - REST design, authentication, versioning
- [grpc.cpp](grpc.cpp) - gRPC, Protocol Buffers, streaming RPC
- [graphql.cpp](graphql.cpp) - GraphQL schema, resolvers, subscriptions

### Data & Infrastructure
- [serialization.cpp](serialization.cpp) - JSON, Protobuf, MessagePack
- [microservices.cpp](microservices.cpp) - Service mesh, patterns, tracing
- [performance_web.cpp](performance_web.cpp) - Caching, optimization, scaling

## 🚀 Use Cases

- **REST API Backend** → http_server.cpp + rest_api.cpp + serialization.cpp
- **gRPC Microservice** → grpc.cpp + async_io.cpp + microservices.cpp
- **WebSocket Chat** → websocket.cpp + http_server.cpp
- **HTTP Client Tool** → http_client.cpp + async_io.cpp + ssl_tls.cpp

## 📚 Requirements

- C++23 compiler (GCC 13+, Clang 16+, MSVC 19.36+)
- OpenSSL 3.0+ (для SSL/TLS)
- Optional: Boost.Asio, gRPC, Protocol Buffers

## 🎓 Learning Path

1. **Начало**: network_basics.cpp → tcp/udp basics
2. **Async**: async_io.cpp → event loops, coroutines
3. **Web**: http_server.cpp → создай свой web framework
4. **APIs**: rest_api.cpp или grpc.cpp → выбери стиль
5. **Production**: microservices.cpp + performance_web.cpp

## 🔗 Related Guides

- [C++23 Quick Reference](../cpp-quick-reference/) - Базовый синтаксис
- [C++23 Complete Guide](../cpp-complete-guide/) - Глубокие концепции
- [C++23 GameDev Guide](../cpp-gamedev/) - Разработка игр