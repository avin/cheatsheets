# REST API best practices

Рекомендации по проектированию, документированию и эксплуатации REST API.

### 🧠 [Общие принципы дизайна](./design-principles.md)

- Resource-oriented и idempotency
- Stateless и кэширование
- Consistency в структуре
- Security by design и rate limiting

### 🌐 [URL, ресурсы и версии](./urls-versioning.md)

- Именование ресурсов, субресурсы
- Версионирование (URL, header)
- Пагинация и фильтрация
- Health-check и bulk операции

### ✉️ [Запросы, ответы и форматы](./requests-responses.md)

- HTTP методы и семантика
- Формат запросов/ответов (JSON)
- Хедеры (ETag, Correlation-ID)
- Partial responses и batch

### 🔑 [Аутентификация и авторизация](./auth-security.md)

- OAuth2, API keys, JWT, mTLS
- Access control (RBAC/ABAC, scopes)
- Возврат и проверка токенов
- Защита входа, rate limiting

### ⚠️ [Ошибки и статус-коды](./errors-status.md)

- HTTP коды (2xx/4xx/5xx)
- Структура ошибки (code, message, details)
- Validation и concurrency
- Maintenance и локализация

### 📄 [Документация и тестирование](./docs-testing.md)

- OpenAPI/Swagger спецификация
- Документация и SDK
- Автотесты API (unit/integration/contract)
- Sandbox окружения

### 🔍 [Наблюдаемость и эксплуатация](./observability-ops.md)

- Логи и структурированное логирование
- Tracing и метрики (Prometheus)
- Alerting и SLO
- Runbooks, environment separation
