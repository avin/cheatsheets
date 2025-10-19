# Ошибки и статус-коды

## HTTP статус-коды
- `200 OK` — успешная операция.
- `201 Created` — ресурс создан (возвращайте `Location`).
- `204 No Content` — успешный запрос без тела.
- `400 Bad Request` — неверные параметры.
- `401 Unauthorized` — требуется аутентификация.
- `403 Forbidden` — нет прав.
- `404 Not Found` — ресурс отсутствует.
- `409 Conflict` — конфликт (дубликат, версия).
- `422 Unprocessable Entity` — валидация не прошла.
- `429 Too Many Requests` — превышен лимит.
- `500` / `502` / `503` — серверные ошибки.

## Структура ошибки

```json
{
  "error": {
    "code": "validation_failed",
    "message": "Invalid email",
    "details": [
      { "field": "email", "issue": "invalid_format" }
    ],
    "traceId": "abc123"
  }
}
```

- `code` — машинно-читаемый код.
- `message` — человеку читаемый текст.
- `details` — массив ошибок полей.
- `traceId` или `requestId` для корреляции.

## Validation errors
- Указывайте, какое поле не прошло проверку.
- При нескольких ошибках верните все.

## Обработка concurrency
- Используйте `409 Conflict` или `412 Precondition Failed` (если есть ETag / If-Match).

## Maintenance / плановые работы
- `503 Service Unavailable` + `Retry-After`.

## Internationalization
- По необходимости поддерживайте `Accept-Language` в сообщениях.

