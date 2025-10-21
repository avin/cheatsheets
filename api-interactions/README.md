# API взаимодействие шпаргалка

Сводка по взаимодействию с API на фронтенде: REST и fetch, GraphQL, streaming и real-time, обработка ошибок, инструменты и best practices.

### 📡 [REST и fetch](./rest-fetch.md)

- fetch basics, AbortController
- JSON/FormData, headers, cache options
- Retry logic, axios/ky usage

### 🔁 [GraphQL, batching и caching](./graphql-batching.md)

- GraphQL queries/mutations
- Apollo Client, caching, persisted queries
- Subscriptions, DataLoader

### 📶 [Streaming, SSE и WebSockets](./streaming-realtime.md)

- Streaming fetch, NDJSON
- SSE (EventSource) базовая реализация
- WebSockets (WS/WSS), reconnect
- WebTransport, keepalive

### 🛡️ [Ошибки, retry и устойчивость](./errors-retry.md)

- Error handling, HTTP status check
- Exponential backoff retry example
- Timeouts, fallback UI

### 🧰 [Инструменты и тестирование](./tools-testing.md)

- REST/GraphQL clients (axios, Apollo)
- Mocking tools (моки через Service Worker, Mirage, fake-api-middleware для дев-сервера webpack/vite)
- Testing frameworks, contract tests
- Monitoring/observability

### ✅ [Best practices и безопасность](./best-practices.md)

- API design, pagination, rate limit
- Security (auth, CSRF)
- Performance (cache, batching)
- Offline/resilience, documentation
