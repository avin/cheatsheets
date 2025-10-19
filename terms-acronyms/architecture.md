# Архитектура и проектирование

**ACID** — набор гарантий транзакций в СУБД: atomicity, consistency, isolation, durability.

**BASE** — мягкие гарантии для распределённых систем: basically available, soft state, eventual consistency.

**CAP theorem** — в условиях сетевого разбиения система может гарантировать лишь два из трёх свойств: consistency, availability, partition tolerance.

**CQRS** — (Command Query Responsibility Segregation) разделение операций чтения и записи по разным моделям/службам.

**DDD** — Domain-Driven Design, подход к проектированию софтовых систем, который ставит бизнес-домен в центр архитектуры.

**Event Sourcing** — хранение состояния как последовательности событий; текущее состояние вычисляется путём их применения.

**Hexagonal Architecture** (Ports and Adapters) — архитектура, где ядро (domain) отделено от инфраструктуры и взаимосвязей, взаимодействует через порты.

**Microservices** — архитектурный стиль, предполагающий независимые сервисы с собственными БД, оркестрируемые через сеть.

**Monolith** — монолитная архитектура, в которой все компоненты приложения разворачиваются как единое целое.

**Service Mesh** — инфраструктурный слой для сервис-2-сервис коммуникаций (Envoy, Istio, Linkerd), реализующий observable, secure, controllable service-to-service traffic.

**SOA** — Service-Oriented Architecture, предшественник микросервисов, фокус на reusable services.

**Saga pattern** — управление длительными транзакциями в распределённых системах с помощью последовательности локальных транзакций и компенсирующих действий.

**Twelve-Factor App** — набор принципов для создания cloud-native приложений (конфигурация через env, stateless процессы и т.д.).

**Zero-downtime deployment** — техники обновления без простоя (blue/green, canary).

**Kappa/Lambda architecture** — архитектурные подходы к обработке данных (batch + stream).
