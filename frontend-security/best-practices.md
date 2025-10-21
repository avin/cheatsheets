# Чек-лист и best practices

## XSS
- Использовать CSP (`default-src 'self'; script-src 'nonce-...'`).
- Sanitize пользовательский ввод (DOMPurify).
- Escape контент по контексту.
- Избегать `eval`, `Function`, `new Function`, динамического HTML.
- Trusted Types (если поддерживается).

## CSRF/State
- `SameSite` cookie, CSRF tokens, Double Submit.
- Проверка Origin/Referer (POST/PUT/DELETE).
- Logout → invalidate session/token.
- Rate limiting и MFA.

## Storage
- Не храните secrets в `localStorage`. Access token → header.
- refresh token → `HttpOnly` cookie (кластер).
- secure cookies (`Secure`, `HttpOnly`).

## Third-party scripts
- SRI (`integrity`), ограничить `script-src` (CSP).
- Отслеживайте обновления/релизы.
- Privacy: обработка пользовательских данных (GDPR).

## Supply chain
- Pin versions (lockfiles).
- Проверьте maintainer/team. Используйте `npm audit`, `yarn audit`, GitHub Dependabot.
- `npm ci`, `npm config set ignore-scripts true` (если можно).
- Подписанные релизы (Sigstore).

## Deployment
- HTTPS/TLS ONLY `Strict-Transport-Security`.
- Security headers (CSP, X-Content-Type-Options, X-Frame-Options/Permissions-Policy).
- subresource integrity.
- Security scans (OWASP ZAP, Burp), SPA-specific tests (DOM XSS).

## Monitoring
- Error reporting + Sentry/Bugsnag.
- CSP report endpoint.
- Alerts на изменения security headers.
- Regular pentest.

