# Запросы, ответы и форматы

## HTTP методы
- Используйте семантику:
  - `GET` — чтение (должен быть безопасным и идемпотентным).
  - `POST` — создание/действие.
  - `PUT` — полное обновление ресурса (идемпотентно).
  - `PATCH` — частичное обновление.
  - `DELETE` — удаление.
  - `HEAD`, `OPTIONS` — служебные.

## Request body
- JSON — де-факто стандарт (`Content-Type: application/json`).
- Для бинарных данных — `multipart/form-data` или S3 pre-signed URLs.
- Ограничивайте размер (`max body size`) и валидируйте вход.

## Responses
- Структура: `{ "data": ..., "meta": {...}, "links": {...} }` (JSON:API, но можно свой стандарт).
- Включайте метаданные: пагинация, фильтры.
- Даты → ISO 8601 (`2024-05-01T12:34:56Z`).
- Денежные суммы → integer (центы) или decimal с `currency`.

## Headers
- `Content-Type`, `Accept`, `Authorization` — обязательные согласно контексту.
- `ETag` / `If-None-Match` для кэширования.
- `Correlation-ID` / `X-Request-ID` для трассировки.
- `Retry-After` при rate limit/maintenance.

## Hypermedia / Links
- Опционально возвращайте `_links` (self, next, prev).

## Sorting, filtering, pagination
- Конвенции (snake_case vs camelCase). Документируйте.

## Partial responses
- Поддерживайте `fields`/`select` (`?fields=id,name`). Уменьшает payload.

## Batch
- В POST body список `{ items: [ {...}, {...} ] }`; проверка каждого элемента, возвращать статусы по item.

