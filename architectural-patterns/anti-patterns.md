# Anti-patterns и принципы

## Anti-patterns

- **Big Ball of Mud** — система без структуры. Признаки: нет модульности, жёсткая связанность.
- **God Object / God Service** — компонент, который знает/делает слишком много.
- **Golden Hammer** — использование знакомого инструмента для всех задач, даже когда он не подходит.
- **Stovepipe / Silos** — изолированные системы без интеграций.
- **Shotgun Surgery** — любое изменение требует правок в десятках мест.
- **Vendor Lock-in** — жёсткая зависимость от одного провайдера/технологии.
- **Spaghetti Code** — запутанные зависимости и flow.
- **Not-Invented-Here (NIH)** — нежелание использовать внешние решения.
- **Premature Optimization** — оптимизация без необходимости, усложнение архитектуры.
- **Architecture Astronaut** — стремление к сложным решениям без бизнеса.

## Принципы

- **SOLID** — набор принципов проектирования объектов/модулей.
- **DRY** — Don't Repeat Yourself.
- **KISS** — Keep It Simple Stupid.
- **YAGNI** — You Aren't Gonna Need It.
- **Separation of Concerns** — разделение ответственности.
- **12-Factor App** — принципы cloud-native (см. архитектура).
- **Design for Failure** — учитывайте возможность отказов (retries, graceful degradation).
- **Evolutionary Architecture** — архитектура, поддерживающая инкрементальные изменения.
- **Documentation as Code** — архитектурные решения фиксируются (ADR — Architecture Decision Records).
