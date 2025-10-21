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
- Проверяет: оффлайн, manifest, SW registration, best practices.
- Check Progressive Web App Checklist (web.dev/pwa-checklist) для подробного списка требований.

## Progressive Enhancement
- Базовая функциональность без JS (где возможно).
- Храните данные на сервере, синхронизируйте при онлайн.

## Browser support
- Chromium/Firefox/Safari поддерживают PWA (Safari 16+ имеет ограничения).
- iOS: offline ограничен (quota около 50MB, service worker живёт ~30 сек). Web Push Notifications доступны с iOS 16.4+ (только по разрешению пользователя).
- Нет Background Sync, ограниченный доступ к Web Push API (без periodic sync).

## SPA vs MPA
- PWA может быть и SPA, и MPA. Важен service worker.

## PWA capabilities
- Web App Manifest (icons, display mode, start_url).
- Service Worker (cache, background sync, push notifications).
- Web APIs: Notifications, Background Sync, Web Share, Workbox.

