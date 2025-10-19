# Подписки и realtime

GraphQL-подписки позволяют получать события в реальном времени. Важно учитывать транспорт, масштабирование и безопасность.

## Протоколы и транспорт

- WebSocket (`graphql-ws`, `subscriptions-transport-ws`) — стандарт для браузеров.
- Server-Sent Events (SSE) — подходит для одностороннего канала, но не все клиенты поддерживают.
- MQTT/AMQP — для IoT и внутренних сервисов (через шлюзы).

## Схема подписок

```graphql
type Subscription {
  messageAdded(roomId: ID!): Message!
  orderStatusChanged(orderId: ID!): OrderStatusEvent!
}

type Message {
  id: ID!
  roomId: ID!
  author: User!
  content: String!
  createdAt: DateTime!
}
```

- Всегда указывайте фильтры (roomId/orderId), чтобы ограничить поток событий.
- Возвращайте полноценные типы — подписка должна быть консистентна с остальными запросами.

## Реализация на сервере

- Apollo Server: `graphql-ws`, `graphql-subscriptions`, pub/sub адаптер (Redis, Kafka, MQTT).
- Mercurius (Fastify): встроенный pub/sub с плагинами для Redis, NATS.
- Yoga / Helix: используют `@graphql-tools/executor`, подходящи для serverless.

```ts
// Apollo Server (TypeScript)
const pubsub = new RedisPubSub();

const resolvers = {
  Subscription: {
    messageAdded: {
      subscribe: withFilter(
        () => pubsub.asyncIterator('MESSAGE_ADDED'),
        (payload, variables) => payload.roomId === variables.roomId,
      ),
    },
  },
};
```

- В serverless окружениях подписки требуют внешнего брокера (например, GraphQL over WebSocket + AWS AppSync, Hasura Event Triggers).
- Планируйте время жизни соединения и heartbeats (ping/pong).

## Авторизация и безопасность

- Проверяйте токен при установке соединения и обновляйте его на каждом событии (если токен истекает).
- Ограничивайте количество одновременных подписок на пользователя.
- Добавляйте метрики по количеству подписок, времени жизни подключений, ошибкам.

## Отладка и мониторинг

- Логируйте handshake, reconnect, ошибки сериализации.
- Используйте `graphql-ws` server events или middleware для трассировки.
- При тестировании подписок используйте сценарии: подключение, потеря соединения, повторное подключение, хаотичный поток событий.
