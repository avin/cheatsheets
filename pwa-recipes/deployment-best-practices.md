# Публикация и best practices

## Deployment
- HTTPS обязательный. Используйте Let's Encrypt / managed certs.
- Версионирование service worker: update caches, `skipWaiting()` осторожно.
- Кэш контролируйте (`cache busting`, `precache manifest`).
- Monitor SW updates (промпт пользователю обновить).

## Updates
- SW update flow: `install` → `waiting` → `activate`.
- Prompt user: "New version available. Refresh?".
- `clients.claim()` для takeover (после activate).

## Testing
- Lighthouse (PWA category).
- DevTools → Application tab: manifest, service workers, storage.
- `chrome://inspect/#service-workers`.
- Work offline toggle.

## Distribution
- Web: `Add to Home Screen` (Android/iOS). Desktop Chrome.
- Microsoft Store → PWABuilder → generate package.
- Google Play → Trusted Web Activity.

## Analytics/Monitoring
- Track install prompt usage, `beforeinstallprompt` acceptance rate.
- Log service worker errors (`self.addEventListener('error')`).
- RUM: Web Vitals.
- Error reporting (Sentry).

## Best practices
- Minimize bundle, prefetch critical assets.
- Offline fallback (при ошибке — понятная страница).
- Background sync/push responsibly (ask permission contextually).
- Пересмотрите privacy (ядро PWA — access to device features).

