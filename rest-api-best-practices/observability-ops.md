# Наблюдаемость и эксплуатация

## Логирование
- Логируйте каждый запрос: метод, путь, статус, `requestId`, пользователь, latency.
- Структурированный формат (JSON) — легче парсить.
- Не логируйте чувствительные данные (PII, токены).

## Tracing
- Используйте `X-Request-ID` / `traceparent` для распределённого трассирования (OpenTelemetry, Zipkin, Jaeger).
- Добавляйте span'ы на ключевые этапы (DB запросы, внешние вызовы).

## Метрики
- `requests_total`, `requests_duration`, `errors_total`, `active_users`.
- Лейблы: метод, endpoint шаблон, статус код.
- Экспорт в Prometheus, Datadog и т.д.

## Alerting
- SLO/SLA метрики (p95 latency, error rate).
- Алерты по rate spikes, проценту 5xx, росту 4xx.

## Feature flags & rollout
- Используйте флаги для постепенного включения функционала.
- Отдельные endpoint для проверки состояния (`/feature-flags`).

## Deployments
- Blue/green, canary, rollbacks (см. DevOps).
- Документируйте процедуру rollback.

## Инциденты
- Процесс IR: detection, triage, mitigation, resolution, postmortem.
- Храните runbooks (что делать при 5xx, отказе БД).

## Конфигурация
- Разделение env: prod, staging, dev.
- `Config-as-code` (env vars, Consul, Vault).
- Restart/stopping endpoint, graceful shutdown.

## API analytics
- Собирайте usage: какие endpoint самые популярные, кто потребители.
- Rate-limits, quotas: информируйте клиентов при близости к лимиту.

