# Best practices и безопасность

## API design considerations
- Консистентная структура (см. REST/GraphQL шпаргалки).
- Версионирование / feature flags.
- Pagination (cursor, limit), filtering.
- Rate limiting: 429 + `Retry-After`.

## Security
- HTTPS всегда.
- Token-based auth: JWT (short-lived), rotate refresh tokens, OAuth2.
- Защита от CSRF (SameSite, tokens) — см. Security шпаргалка.
- Input validation (client + server).
- Avoid leaking secrets в bundle (`process.env.REACT_APP_...` ≠ secret).

## Performance
- Cache-инвалидация (ETag, Last-Modified).
- Batching (GraphQL, API batching), deduplicate requests.
- CDN/edge caching для public API.
- Lazy load/ prefetch (React Query/SWR).

## Offline & resilience
- Fallback UI, offline modes (PWA стратегии).
- Store state в IndexedDB/local storage (sanitize).
- Background sync (Workbox).

## Observability
- Логи (structured), monitoring (Sentry, Datadog, Prometheus).
- Настройка `window.addEventListener('error')`, `unhandledrejection`.
- SLA/SLO нотификации.

## Documentation
- OpenAPI/Swagger для REST, GraphQL schema docs.
- README/Playground, collection in Postman.
- Change logs, version docs.

