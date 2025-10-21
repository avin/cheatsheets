# Базовые принципы и требования

## Что такое PWA
- Веб-приложение, которое ощущается как нативное: оффлайн, installable, быстрый старт.
- "Progressive" — работает везде, использует доступные возможности.

## Core требования
1. **Secure** — сайт должен работать по HTTPS.
2. **Service Worker** — зарегистрирован, контролирует страницу.
3. **Web App Manifest** — `manifest.json` с иконками/названием.
4. **Responsive** — адаптивный UI.
5. **Performance** — fast load (Lighthouse checklist).

## Lighthouse PWA аудит
- Chrome DevTools → Lighthouse → PWA.
- Проверяет: offline, manifest, service worker, best practices.

## Progressive Enhancement
- Базовая функциональность без JS (где возможно).
- Храните данные на сервере, синхронизируйте при онлайн.

## Browser support
- Chromium/Firefox/Safari поддерживают PWA (Safari 16+ имеет ограничения).
- iOS: поддержка offline limited (квоты IndexedDB, push — только web push iOS 16.4+).

## SPA vs MPA
- PWA может быть и SPA, и MPA. Важен service worker.

## PWA capabilities
- Web App Manifest (icons, display mode, start_url).
- Service Worker (cache, background sync, push notifications).
- Web APIs: Notifications, Background Sync, Web Share, Workbox.

