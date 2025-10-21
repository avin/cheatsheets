# Service Worker и кэширование

## Регистрация
```js
if ('serviceWorker' in navigator) {
  window.addEventListener('load', () => {
    navigator.serviceWorker.register('/sw.js')
      .then(reg => console.log('SW registered', reg))
      .catch(err => console.error('SW registration failed', err));
  });
}
```

## Базовый SW
```js
const CACHE_NAME = 'app-cache-v1';
const PRECACHE_URLS = [
  '/',
  '/index.html',
  '/styles.css',
  '/app.js',
  '/offline.html'
];

self.addEventListener('install', event => {
  event.waitUntil(
    caches.open(CACHE_NAME).then(cache => cache.addAll(PRECACHE_URLS))
  );
});

self.addEventListener('activate', event => {
  event.waitUntil(
    caches.keys().then(keys => Promise.all(
      keys.filter(key => key !== CACHE_NAME).map(key => caches.delete(key))
    ))
  );
});

self.addEventListener('fetch', event => {
  event.respondWith(
    caches.match(event.request).then(response => response || fetch(event.request))
  );
});
```

## Caching стратегии
- **Cache First** — отдаёт из кеша → fallback к сети (статические ресурсы).
- **Network First** — сначала сеть, fallback cache (HTML/API).
- **Stale-While-Revalidate** — отдаёт кеш + параллельно обновляет (аватары).
- **Cache Only**, **Network Only**, **Cache then Network**.
- Используйте Workbox (Google) для декларативных стратегий.

## Offline fallback
- Кешируйте `/offline.html` или `/offline.json`.
- В SW: если network fetch fail → отдайте fallback страницу.

## Background sync
```js
self.addEventListener('sync', event => {
  if (event.tag === 'sync-queue') {
    event.waitUntil(sendPendingRequests());
  }
});
```
- Требует регистрацию `sync.register('sync-queue')` на клиенте.
- Используйте `Background Sync` для офлайн форм.

## Push notifications
- `self.addEventListener('push', event => { ... })`
- Require permission (`Notification.requestPermission`).
- Web Push (VAPID) для отправки.

## Storage quotas
- Cache Storage, IndexedDB, localStorage — ограничение по origin.
- `navigator.storage.estimate()`.

