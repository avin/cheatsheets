# Архитектурные паттерны

Справочник по ключевым архитектурным паттернам и подходам. Каждый раздел описывает суть, типовые сценарии использования и связанные риски/компромиссы.

### 🧱 [Структурные и слойные](./structural-layered.md)

- Layered Architecture (n-tier) — слойная монолитная архитектура
- Hexagonal Architecture — ports & adapters
- Clean Architecture — concentric layers
- Modular Monolith — модули в монолите
- Plugin/Microkernel — ядро+плагины
- MVC/MVP/MVVM — UI паттерны
- Pipe and Filter — pipeline трансформаций
- Blackboard — общая доска

### 🔄 [Интеграционные и коммуникационные](./integration-communication.md)

- ESB — enterprise service bus
- Event-Driven Architecture — pub/sub событий
- Message Queue — очереди задач
- Event Streaming — Kafka, Pulsar
- API Gateway / BFF — единая точка входа
- Webhooks — обратные вызовы
- Choreography vs Orchestration — координация процессов
- Circuit Breaker — защита от cascade failures
- API Composition / Aggregator — агрегация
- Retry/Backoff/DLQ — устойчивость

### ☁️ [Распределённые и облачные](./distributed-cloud.md)

- Microservices — независимые сервисы
- Service Mesh — traffic management
- Serverless — FaaS
- Cloud-native — 12-Factor, sidecar
- Edge Computing — обработка на краю
- Data Center/Region patterns — active-active
- Multitenancy — multi tenant
- Eventual consistency — сходимость
- Consul/etcd/Zookeeper — service discovery

### 🖥️ [UI и клиентские](./ui-client.md)

- MVC/MVP/MVVM — разделение UI
- Flux/Redux — однонаправленный поток
- Observer / Pub-Sub — подписки
- Reactive programming — rxjs
- PWA — веб как нативный
- Micro Frontends — фрагментация UI
- Atomic Design — дизайн-системы
- Responsive design — адаптация
- Accessibility — a11y

### 🗃️ [Data-centric и аналитика](./data-centric.md)

- Data Lake — сырые данные
- Data Warehouse — аналитическое хранилище
- Lambda Architecture — batch + speed
- Kappa Architecture — stream-only
- ETL/ELT — данные
- Star/Snowflake schema — моделирование DWH
- OLTP vs OLAP — type workloads
- CDC — change data capture
- Data Mesh — domain ownership

### ⚠️ [Anti-patterns и принципы](./anti-patterns.md)

- Anti-patterns — Big Ball of Mud, God object
- SOLID/DRY/KISS — принципы
- Design for Failure — resilience
- ADR — Architecture Decision Records
