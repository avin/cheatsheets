# PWA (Progressive Web Apps) шпаргалка

Руководство по построению прогрессивных веб-приложений: базовые принципы, web manifest, service workers, кэширование, офлайн-режим, публикация и лучшие практики.

### 🧱 [Базовые принципы и требования](./basics-requirements.md)

- HTTPS, service worker, manifest
- Lighthouse PWA audit
- Progressive enhancement, browser support

### 📄 [Web App Manifest](./manifest.md)

- name/short_name, start_url, scope
- display modes, theme/background colors
- icons (purpose maskable), shortcuts
- Подключение manifest и meta

### 🔁 [Service Worker и кэширование](./service-worker-cache.md)

- Регистрация SW, install/activate
- Caching стратегии (cache-first, network-first)
- Offline fallback, background sync
- Push notifications, storage quotas

### 📶 [Работа офлайн и data sync](./offline-sync.md)

- Offline UX, online/offline events
- IndexedDB для оффлайн данных
- Background Sync и ограничения
- Error handling, retry

### 📱 [Установка и UX](./installation-ux.md)

- beforeinstallprompt, install flow
- display modes и splash screen
- Icons, Apple Touch, shortcuts
- Web Share/API, ограничения iOS

### 🚀 [Публикация и best practices](./deployment-best-practices.md)

- SW updates, skipWaiting/clients.claim
- Testing через Lighthouse/DevTools
- Distribution (Web, Microsoft Store, TWA)
- Monitoring, analytics, best practices
