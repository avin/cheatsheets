# GraphQL, batching и caching

## Query basics
```js
const query = `
  query GetUser($id: ID!) {
    user(id: $id) {
      id
      name
      posts(limit: 10) {
        title
        createdAt
      }
    }
  }
`;

const response = await fetch('/graphql', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: JSON.stringify({ query, variables: { id: '123' } })
});
const { data, errors } = await response.json();
```
- Всегда обработайте `errors` (GraphQL возвращает 200 + errors array).
- Используйте client (Apollo, urql, Relay) для удобства.

## Apollo Client
- Cache (InMemoryCache), fetchPolicy (`cache-first`, `network-only`).
- Link: HTTP link, Batch HTTP link (batch multiple operations).
- Apollo DevTools.
- Error handling (onError link).

## Persisted Queries
- Передача hash вместо полного запроса (Apollo Persisted Queries).
- Уменьшение payload + защита от сложных запросов.

## GraphQL Subscriptions
- WebSocket (graphql-ws), SSE (GraphQL over SSE).
- Subscriptions transport (Apollo, urql). Поддержка в backend.

## DataLoader (server)
- Batching/ caching в resolver, предотвращает N+1 (см. серверную шпаргалку).

## Code Generation
- GraphQL Code Generator — типизированные hooks/clients.
- schema-first vs client-first.

## Best practices
- Limit depth/complexity (GraphQL Shield, max complexity).
- Input validation (GraphQL schema + custom directives).
- caching: `@cacheControl`, `APQ`. LaunchDarkly? (???). 
- Batching: `apollo-link-batch-http`, `defer`, `@stream` (GraphQL spec).

