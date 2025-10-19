# Пагинация и фильтры

Пагинация и фильтрация контролируют объем данных и нагрузку на бэкенд. Настройте единый контракт для всех сущностей.

## Offset-пагинация

- Простой вариант: аргументы `limit` и `offset`.
- Подходит для административных интерфейсов и неизменяемых списков.
- Неустойчив к вставкам/удалениям в середину — клиент может увидеть дубли или пропуски.

```graphql
type Query {
  projects(limit: Int!, offset: Int!): ProjectPage!
}

type ProjectPage {
  nodes: [Project!]!
  totalCount: Int!
}
```

## Cursor-пагинация

- Используйте курсоры (base64 или opaque) для устойчивости к изменению данных.
- Стандарт Relay Connection: `edges`, `pageInfo`, `node`.

```graphql
type Query {
  projects(first: Int, after: Cursor): ProjectConnection!
}

type ProjectConnection {
  edges: [ProjectEdge!]!
  pageInfo: PageInfo!
  totalCount: Int
}

type ProjectEdge {
  cursor: Cursor!
  node: Project!
}

type PageInfo {
  hasPreviousPage: Boolean!
  hasNextPage: Boolean!
  startCursor: Cursor
  endCursor: Cursor
}
```

## Курсоры в практике

- Храните курсор как `encode(base64, JSON.stringify({ id, createdAt }))`.
- Делайте пагинацию устойчивой к сортировке: `ORDER BY created_at, id`.
- Проверяйте `first` и `last` на верхний предел (например, не более 100 элементов).

## Фильтры и сортировки

- Структурируйте фильтры по полям: `equals`, `in`, `gte`, `lte`.
- Для сложных условий поддерживайте `AND`/`OR`.

```graphql
input ProjectFilter {
  name: StringFilter
  status: ProjectStatus
  createdAt: DateRange
  and: [ProjectFilter!]
  or: [ProjectFilter!]
}

input StringFilter {
  equals: String
  contains: String
  in: [String!]
}

input DateRange {
  gte: DateTime
  lte: DateTime
}

enum ProjectOrderField {
  CREATED_AT
  NAME
}

input ProjectOrder {
  field: ProjectOrderField!
  direction: OrderDirection! = ASC
}
```

## Соглашения

- Вводные фильтры и сортировки делайте опциональными, возвращайте дефолт на бэкенде.
- Документируйте ограничения (`maxLimit`, допустимые поля сортировки).
- При необходимости ограничьте сложность запроса (количество nested соединений/фильтров) через `graphql-depth-limit` или кастомный валидатор.
