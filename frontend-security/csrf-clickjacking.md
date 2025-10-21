# CSRF, clickjacking и session

## CSRF (Cross-Site Request Forgery)
- Защита: CSRF token (пер-генерация на сервере, вёрстка `<meta name="csrf-token">`).
- Двухступенчатая защита: токен + `SameSite` cookie.
- Для API: `SameSite=strict` или `lax`, JWT → используйте `Authorization` header (не cookie).
- Double Submit Cookie: токен в cookie + в header.
- Реализуйте перезапросы (hedged requests) когда нужно.

## Clickjacking
- Защита: `X-Frame-Options: DENY` или `frame-ancestors` в CSP.
- UI overlay detection (JS, но не надёжно).
- Для форм — желательно confirmation.

## Session fixation
- Регулярно обновляйте сессии (rotating session ID) после login.
- Храните минимальные данные в session storage.

## Login CSRF
- Применяйте CSRF token на login форму.
- CAPTCHA? (с осторожностью, юзабилити).
- Rate limiting, lockout, MFA.

## CORS
- Настройте `Access-Control-Allow-Origin` (specific origin, не `*`).
- Credentials: `Access-Control-Allow-Credentials: true` + `Access-Control-Allow-Origin` не `*`.
- Preflight (`OPTIONS`) — проверяйте методы/headers.

## Form anti-automation
- Honeypot поля (hidden inputs) — детект ботов.
- reCAPTCHA, hCaptcha, Cloudflare Turnstile.

