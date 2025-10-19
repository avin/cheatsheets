# Проверки, тесты и инструменты

Надёжная схема GraphQL требует статического анализа, тестов и мониторинга.

## Валидация схемы

- Используйте `graphql-js` `validate` + кастомные правила (depth-limit, complexity).
- GraphQL Inspector / Apollo Rover: сравнение схем, предотвращение breaking changes.
- Автоматизируйте проверку SDL в CI (линтер + проверка на циклические зависимости).

## Code Generation

- GraphQL Code Generator: генерирует типы, хуки, SDK для клиентов (TypeScript, Swift, Kotlin, C#).
- Relay Compiler: генерирует артефакты для Relay Modern.
- Сервера: генерируйте типы для контракта (например, Nexus, Pothos, type-graphql).

## Тестирование резолверов

- Юнит-тесты: вызывайте резолвер как обычную функцию, подставляя контекст и mock-зависимости.
- Интеграционные тесты: исполняйте реальный GraphQL-запрос через `graphql()` (graphql-js) или HTTP-слой.
- Snapshot-тесты имеют смысл для стабильных схем; следите за обновлением `__typename`.

```ts
import { graphql } from 'graphql';
import { schema } from '../schema';

it('returns account info for the viewer', async () => {
  const result = await graphql({
    schema,
    source: `query { viewer { id email } }`,
    contextValue: { user: { id: 'u1', email: 'a@example.com' } },
  });
  expect(result.errors).toBeUndefined();
  expect(result.data?.viewer.email).toBe('a@example.com');
});
```

## Нагрузочное тестирование

- k6 + `graphql` extension, Artillery, Gatling.
- Измеряйте время выполнения, загрузку CPU, количество открытых соединений (особенно для подписок).
- Ограничивайте глубину и сложность запроса до теста, чтобы атаки не блокировали сервер.

## Security checklist

- Depth/complexity limit, query cost analysis.
- Persisted queries + allow-list (Apollo Persisted Queries, Relay Persisted Queries).
- Валидация переменных (например, максимальная длина строк).
- Защита от introspection в продакшене (скрывайте схему или ограничивайте авторизованным).

## Мониторинг и алерты

- Подключите tracing (Apollo Studio, Honeycomb, OpenTelemetry + Grafana Tempo).
- Метрики: средняя продолжительность резолвера, количество ошибок, размер ответа.
- Логи: запрос, переменные (обезличенные), user-id, correlation-id, код ошибки.
