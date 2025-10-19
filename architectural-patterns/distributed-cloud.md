# Распределённые и облачные паттерны

## Microservices
- **Суть**: множество самостоятельных сервисов, каждый решает ограниченный домен.
- **Плюсы**: независимые релизы, масштабирование по компонентам, команды "по сервису".
- **Минусы**: сложная инфраструктура (CI/CD, observability), межсервисные коммуникации.

## Service Mesh
- **Суть**: дополнительный слой (Envoy + control plane), который управляет traffic management, security, observability.

## Serverless
- **FaaS (Function as a Service)**: AWS Lambda, Cloud Functions.
- **Плюсы**: платишь за выполнение, быстрый старт.
- **Минусы**: cold start, vendor lock-in, ограничения по ресурсам и времени.

## Cloud-native
- **12-Factor App, контейнеризация, declarative API.**
- **Паттерны**: Sidecar, Ambassador, Adapter.

## Edge Computing
- **Суть**: обработка на краю сети (приложения на CDN/IoT gateway).
- **Использование**: AR/VR, IoT, real-time analytics.

## Data Center/Region patterns
- **Active-Active**: несколько регионов обслуживают трафик.
- **Active-Passive**: основной DC + резерв.
- **Geo-Replication**: синхронная/асинхронная.

## Multitenancy
- **Суть**: одно приложение обслуживает несколько клиентов (тенантов).
- **Подходы**: isolated DB per tenant, shared DB with tenant_id, shared schema but row-level security.

## Saga / Orchestration** (см. integration).

## CAP theorem** (см. архитектура).

## Eventual consistency
- **Суть**: данные синхронизируются со временем, промежуточно могут расходиться.
- **Использование**: микросервисы, распределённые cache/DB.

## Idempotency / Message deduplication
- **Важно** для повторных вызовов в распределённых системах.

## Consul / etcd / Zookeeper
- **Службы**: сервис-дискавери, хранение конфигураций, распределённые блокировки.
