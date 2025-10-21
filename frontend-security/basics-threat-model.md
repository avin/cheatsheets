# Основы и модель угроз

## Принципы
- **Defense in depth** — комбинируйте меры: CSP, sanitization, CSRF, security headers.
- **Least privilege** — минимизируйте доступ к данным и API.
- **Zero trust** — не доверяйте клиенту, проверяйте сервером.

## Типичные угрозы
- XSS (cross-site scripting) → крадёт cookies, токены.
- CSRF → выполняет запросы от имени пользователя.
- Clickjacking → пользователь кликает "под iframe".
- Supply chain → вредоносные пакеты зависимостей.
- Leaks (sensitive data exposure) → API ключи, токены в JS.
- DOM-based уязвимости (client-side template injection, postMessage).

## Threat modeling
- Asset: токены, PII, доступ к API.
- Adversary: злоумышленник, скрипт, несознательный пользователь.
- Entry points: формы, query params, localStorage, third-party scripts.
- Mitigation: sanitize, escape, validate, политика доступа, SRI.

## Политика безопасности контента (CSP)
- CSP — декларация доверенных источников (script-src, style-src, object-src).
- Настраивайте отчёты (report-uri/report-to) для мониторинга нарушений.
- Используйте nonce/hashed scripts вместо `unsafe-inline`.

## Headers
- `Content-Security-Policy`
- `Strict-Transport-Security`
- `X-Frame-Options` или `frame-ancestors`
- `X-Content-Type-Options: nosniff`
- `Referrer-Policy`, `Permissions-Policy`
- `Cross-Origin-Embedder-Policy`, `Cross-Origin-Opener-Policy`, `Cross-Origin-Resource-Policy` для защиты от Spectre.

