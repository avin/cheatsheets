# Runtime защита и мониторинг

## Runtime security
- CSP violation reporting: `report-uri`/`report-to` endpoint.
- Subresource Integrity (SRI) + `crossorigin`).
- Trusted Types диагностика (`window.trustedTypes.createPolicy`).
- Content-Security-Policy-Report-Only для gradual rollout.

## Error reporting
- Sentry, Bugsnag, Rollbar — отправка ошибок и breadcrumbs.
- Monitor console logs (`console.error`), unhandledrejection.
- Логируйте пользователю понятные сообщения, не выдавайте stack traces.

## Tamper detection
- Проверка `document.currentScript.integrity`.
- Manifest integrity (для SPA) — application shell.

## Monitoring
- Network tab: отладка запросов.
- RUM (Real User Monitoring) — измерение Web Vitals.
- Security headers monitoring (Mozilla Observatory, SecurityHeaders.com).
- CT (Certificate Transparency) мониторинг для TLS.

## postMessage / iframe security
- Проверяйте `event.origin` перед обработкой сообщений.
- Используйте конкретные targetOrigin в `postMessage` (`window.postMessage(data, 'https://example.com')`).
- Sandbox `<iframe>` `sandbox="allow-scripts allow-same-origin"`. Не включайте `allow-top-navigation` без необходимости.

## Polyfills & dynamic imports
- Загружайте только необходимые polyfills (`@vitejs/plugin-legacy`, Polyfill.io). Избегайте untrusted sources.
- Code splitting: lazy load/feature flag.

