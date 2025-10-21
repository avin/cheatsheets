# REST и fetch

## Fetch API
```js
const controller = new AbortController();
const timeout = setTimeout(() => controller.abort(), 5000);

try {
  const response = await fetch('/api/users', {
    method: 'GET',
    headers: { 'Accept': 'application/json' },
    signal: controller.signal
  });

  if (!response.ok) throw new Error(`HTTP ${response.status}`);

  const data = await response.json();
  console.log(data);
} catch (error) {
  if (error.name === 'AbortError') {
    console.log('Request aborted');
  } else {
    console.error('Request failed', error);
  }
} finally {
  clearTimeout(timeout);
}
```
- Используйте `AbortController` для отмены.
- Проверяйте `response.ok`, `status`. Не воспринимайте fetch errors как HTTP errors.

## JSON & FormData
- `response.json()`, `response.text()`, `response.blob()`.
- `FormData` для multipart:
```js
const formData = new FormData(formElement);
fetch('/upload', { method: 'POST', body: formData });
```

## Headers
- Content-Type (`application/json`), Accept.
- Authorization: Bearer токены (лучше header, не cookie для SPA). 

## Cache
- `Cache-Control` (сервер). Примеры: `no-cache`, `max-age`, `stale-while-revalidate`.
- Клиент: `fetch` по умолчанию использует условные запросы.
- `fetch('/api/data', { cache: 'reload' })` → bypass cache.

## Retry
- Реализация: exponential backoff (пример в errors-retry).
- Избегайте ретраев на неидемпотентных запросах (POST => подумайте идемпотентностью).

## axios / ky
```js
import ky from 'ky';
const api = ky.create({ prefixUrl: '/api', retry: { limit: 2 } });
const result = await api.get('users').json();
```
- axios: interceptors, cancellation tokens (`AbortController`).

## REST best practices
- Соблюдайте консистентные path/query (см. REST шпаргалка).
- Используйте HATEOAS по необходимости.
- Throttling/rate limiting — обрабатывайте `429`, `Retry-After`.

