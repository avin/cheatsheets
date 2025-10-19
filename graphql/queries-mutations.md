# Запросы и мутации

GraphQL-запросы описывают форму данных, которую ожидает клиент. Мутации изменяют состояние и возвращают свежие сущности.

## Структура запроса

```graphql
query GetDashboard($limit: Int!, $includeArchived: Boolean = false) {
  viewer {
    id
    name
    teams(first: $limit) {
      edges {
        node {
          id
          name
          inbox @skip(if: $includeArchived) {
            unreadCount
          }
        }
      }
    }
  }
}
```

- `operationName` помогает трейсингу и кешу.
- Алиасы позволяют исполнять один и тот же резолвер с разными аргументами.
- Директивы `@include` и `@skip` работают на уровне запроса.

## Фрагменты и переиспользование

```graphql
fragment UserAvatar on User {
  id
  name
  profile {
    avatarUrl
  }
}

query TeamMembers($teamId: ID!) {
  team(id: $teamId) {
    id
    name
    members {
      ...UserAvatar
      role
    }
  }
}
```

- Фрагменты помогают синхронизировать поля между компонентами и снижают дублирование.
- Инлайн-фрагменты (`... on Type`) применяются для union/interface.

## Аргументы и input-типы

- Сложные фильтры оформляйте через `input`-типы, чтобы избежать множества опциональных аргументов.
- Для пагинации комбинируйте `first/after`, `last/before`, фильтры и сортировки.

```graphql
mutation UpdateProfile($input: UpdateProfileInput!) {
  updateProfile(input: $input) {
    user {
      id
      bio
    }
    warnings
  }
}

input UpdateProfileInput {
  id: ID!
  bio: String
  socials: [SocialInput!]
}
```

## Батчинг и кеширование

- При обращении к REST/БД избегайте N+1 — используйте DataLoader или аналогичный слой.
- Кеш CANONICAL: ключ = `operationName + variables + selected fields`.
- На клиентах (Apollo/Relay/urql) настраивайте нормализованный кеш; на сервере — short-term response cache (Apollo Cache, InMemoryLRU).

## Обработка ошибок

- Сервер должен возвращать `errors[]` с путями (`path`) и кодами (`extensions.code`).
- Для частичных ошибок оставляйте данные в `data`, чтобы клиент мог отобразить успешную часть.
- Добавляйте correlation-id в `extensions`, чтобы легче дебажить проблему.
