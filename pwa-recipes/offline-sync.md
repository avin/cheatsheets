# Работа офлайн и data sync

## Offline UX
- Уведомляйте пользователя о состоянии (online/offline).
  ```js
  window.addEventListener('offline', () => showToast('You are offline'));
  window.addEventListener('online', () => showToast('Back online'));
  ```
- Предоставляйте офлайн fallback (контент, сохранённый cache + IndexedDB).
- Кешируйте критические API ответы (`Cache Storage` + `IndexedDB`).

## IndexedDB
- Места хранение данных (workbox, idb library).
- Синхронизация: сохраняйте изменения в очередь, отправляйте при `online`.

## Background Sync
- Используйте `Background Sync` (гарантированная доставка запросов при восстановлении сети).
- Fallback: Polling при `online`.
- Поддержка: Chrome/Edge; Safari (пока нет). Падение → manual sync.

## Offline-first
- Precache shell (HTML/CSS/JS), dynamic caching для данных.
- Use `NetworkFirst` для HTML, `StaleWhileRevalidate` для API.
- Offline analytics (Google Analytics offline mode).

## Notifications & push
- Push API (с разрешением пользователя) для обновлений (в магазине/offline).
- Web push → payload = JSON, service worker → показать уведомление (`self.registration.showNotification`).

## Error handling
- Graceful degradation: показывайте, что изменения будут синхронизированы позже.
- Retry logic (exponential backoff).
- Сохраняйте timestamp/разрешения для отладки.

