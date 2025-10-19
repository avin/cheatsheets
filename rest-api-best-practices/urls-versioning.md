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

## Relationships
- Связанные ресурсы: `/orders/{id}/items`, `/users/{id}/roles`.
- Опционально `?include=items,customer` или `?expand=` для совместного ответа.

## Специальные endpoint
- Health check: `/healthz`, `/readyz`, `/livez`.
- Метаданные API: `/version`, `/docs`, `/.openapi.json`.
- Bulk операции: `POST /orders/batch` с массивом элементов.

