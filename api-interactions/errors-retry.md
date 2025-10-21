# Ошибки, retry и устойчивость

## Обработка ошибок
- Всегда проверяйте HTTP статус (`response.ok`). Логируйте тело ошибки (`response.json()`).
- Catch network errors (fetch -> network failure, AbortError, CORS issues).
- Используйте централизованный error handler.
- Показывайте пользователю понятные сообщения.

## Retry
```js
async function fetchWithRetry(url, options = {}, retries = 3, backoff = 500) {
  try {
    const res = await fetch(url, options);
    if (!res.ok && retries) {
      if (res.status >= 500) {
        await new Promise(r => setTimeout(r, backoff));
        return fetchWithRetry(url, options, retries - 1, backoff * 2);
      }
    }
    return res;
  } catch (error) {
    if (retries > 0) {
      await new Promise(r => setTimeout(r, backoff));
      return fetchWithRetry(url, options, retries - 1, backoff * 2);
    }
    throw error;
  }
}
```
- Exponential backoff (`backoff * 2`).
- Circuit breaker (resilience libraries).
- Retry only идемпотентные операции.

## Timeout
- `AbortController` для timeout (см. REST).
- Set per request/timeouts (axios timeout, ky, react-query).

## Fallbacks
- Cache fallback (offline).
- Rehydrate data from local store.
- Для read → fallback UI (skeleton, cached data).

## Progressive enhancement
- Понижение качества (low-res images, partial data) при медленном connection.
- Lazy load heavy components.

## Logging & monitoring
- Отправляйте ошибки в Sentry/Bugsnag.
- Отслеживайте rate ограничений (429), 5xx spikes.
- Аггрегируйте error logs на backend.

