# URL, ресурсы и версии

## Naming
- Используйте иерархию `/resources/{id}/sub-resource`. Пример: `/users/{userId}/orders/{orderId}`.
- Пользуйтесь множественным числом (`/users`), избегайте "verbs" (`/getUsers`).
- Для фильтров и пагинации — query-параметры (`?status=active&limit=20`).

## Версионирование
- **URL версия**: `/v1/`, `/v2/`. Простое понимание для клиентов.
- **Header версия**: `Accept: application/vnd.api+json;version=2`. Гибко, но сложнее.
- **SemVer**: `v1.2.0` только при необходимости. Планируйте снятие старых версий.
- Уведомляйте о deprecation заранее (headers `Deprecation`, `Sunset`).

## Pagination
- Offset/limit (`?offset=0&limit=50`) — простота, но стоимость при больших offset.
- Cursor-based (`?cursor=abc123&limit=20`) — устойчиво к изменениям набора.
- Возвращайте метаданные: `total`, `hasNext`, `nextCursor`.

## Filtering & Sorting
- Query-параметры (`?status=active&createdAfter=2024-01-01`).
- Сортировка (`?sort=+name,-createdAt`) — определите соглашение (например, `+`/`-` или `asc`/`desc`).
- Для сложных фильтров лучше POST `/search` с body (JSON DSL).
  ```json
  POST /orders/search
  {
    "filters": {
      "status": ["pending", "processing"],
      "createdAt": { "from": "2024-01-01", "to": "2024-02-01" },
      "total": { "gte": 100 }
    },
    "sort": ["-createdAt"],
    "pagination": { "cursor": "abc123", "limit": 50 }
  }
  ```
  Возвращайте `meta` с информацией о запросе и `nextCursor`.
  ```json
  {
    "data": [...],
    "meta": {
      "limit": 50,
      "nextCursor": "eyJpZCI6MTIzNH0="
    }
  }
  ```
  При отсутствии следующей страницы `nextCursor` можно опустить или вернуть null.

- Для bulk (batch) операций создайте отдельный endpoint:
  ```json
  POST /inventory/batch
  {
    "operations": [
      { "op": "update", "sku": "SKU-1", "quantity": 10 },
      { "op": "delete", "sku": "SKU-2"}
    ]
  }
  ```
  В ответе указывайте результат по каждому элементу.
  ```json
  {
    "results": [
      { "sku": "SKU-1", "status": "updated" },
      { "sku": "SKU-2", "status": "deleted" }
    ],
    "errors": []
  }
  ```

## Relationships
- Связанные ресурсы: `/orders/{id}/items`, `/users/{id}/roles`.
- Опционально `?include=items,customer` или `?expand=` для совместного ответа.

## Специальные endpoint
- Health check: `/healthz`, `/readyz`, `/livez`.
- Метаданные API: `/version`, `/docs`, `/.openapi.json`.
- Bulk операции: `POST /orders/batch` с массивом элементов.

