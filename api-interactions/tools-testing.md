# Инструменты и тестирование

## Клиенты
- REST: fetch, axios, ky, superagent.
- GraphQL: Apollo Client, urql, Relay.
- WebSockets/SSE: Socket.IO, ws, EventSource API.

## Mocking
- Service Worker mock (например, указать кастомный SW, который перехватывает fetch/XHR).
- MirageJS, json-server для тестовых API.
- fake-api-middleware (Webpack dev server / Vite) для моков на уровне дев-сервера.
- Playwright/Cypress intercept requests.

## Тестирование
- Jest + fetch mock (например, jest-fetch-mock или кастомный Service Worker для браузерных тестов) для unit.
- Integration tests: Cypress, Playwright (через intercept).
- Contract tests: Pact (consumer-driven contracts), Dredd.
- API testing: Postman/Newman, Hoppscotch.

## Monitoring
- Logging: Sentry (http spans), Datadog, NewRelic Browser.
- Network error tracking.
- Live profiling: Chrome DevTools (Network tab).

## Developer utilities
- REST clients: Postman, Insomnia, Bruno.
- GraphQL Playground, GraphiQL, Apollo Sandbox.
- RequestBin/Hookbin для вебхуков.
- Fiddler/Charles Proxy для отладки.
