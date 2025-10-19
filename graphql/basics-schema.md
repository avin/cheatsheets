# Базовые понятия и схема

Основа GraphQL — декларативное описание типов и операций через Schema Definition Language (SDL).

## Типы в SDL

```graphql
type User {
  id: ID!
  email: String!
  name: String
  role: Role! @default(value: "USER")
  profile: Profile
  createdAt: DateTime!
}

enum Role {
  USER
  ADMIN
}

interface Node {
  id: ID!
}

type Profile implements Node {
  id: ID!
  bio: String
  avatarUrl: String
}

union SearchResult = User | Profile
```

- `!` — обязательное поле; массивы записываются как `[Type!]!`.
- Используйте `enum` для дискретных значений, `interface` и `union` для полиморфизма.
- Пользовательские scalar типы (`DateTime`, `UUID`) описываются в SDL и реализуются в резолверах.

## Модуляризация схемы

- Разносите типы и операции по модулям: `user.graphql`, `post.graphql`, `shared.graphql`.
- Используйте `extend` для расширения базовых типов и корневых операций.

```graphql
# base.graphql
schema {
  query: Query
  mutation: Mutation
  subscription: Subscription
}

type Query {
  _service: ServiceInfo!
}

# user.graphql
extend type Query {
  me: User
  user(id: ID!): User
  users(filter: UserFilter, pagination: CursorInput): UserConnection!
}

extend type Mutation {
  updateUser(input: UpdateUserInput!): User!
}
```

## Директивы и описания

- Встроенные директивы (`@include`, `@skip`, `@deprecated`, `@specifiedBy`).
- Пользовательские директивы добавляют метаданные (например, для RBAC, кеширования, валидации).
- Документируйте типы через string-literal перед объявлением или с помощью `"""description"""`.

```graphql
"""
Информация о текущем сервисе (версия, поддержка фич).
"""
type ServiceInfo {
  version: String!
  features: [String!]!
}

directive @auth(requires: Role!) on FIELD_DEFINITION
```

## Query / Mutation / Subscription

- Корневые типы отделяют операции чтения, изменения и realtime.
- Вводные параметры оформляйте через `input` типы (immutable).
- Возвращайте полезные типы обёртки (`Payload`, `Result`) с полями `errors`, `warnings`, `record`.

```graphql
input UpdateUserInput {
  id: ID!
  name: String
  profile: ProfileInput
}

type UpdateUserPayload {
  user: User!
  warnings: [String!]
}

type Subscription {
  userUpdated(id: ID!): User!
}
```

## SDL best practices

- Старайтесь держать имена типов в PascalCase, полей — в camelCase.
- Типы для коллекций называйте по сущности (`UserConnection`, `TransferEdge`).
- Добавляйте комментарии к каждому публичному типу и полю — они попадут в introspection и документацию.
