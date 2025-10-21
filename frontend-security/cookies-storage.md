# Cookies, storage и токены

## Cookies
- Используйте `Secure`, `HttpOnly`, `SameSite` (Lax/Strict) для защищённых cookie.
- Session cookie: `Set-Cookie: session=...; Secure; HttpOnly; SameSite=Lax`.
- Не храните чувствительные данные в cookie (только ID/токен).
- Ротация cookie, избежание "everlasting sessions".

## LocalStorage/SessionStorage
- Избегайте хранения access tokens в `localStorage/sessionStorage` (XSS → theft).
- Если всё же: шифруйте (но JS требует ключ) → лучше cookie + SameSite.
- Очистка при logout, expiration logic.

## IndexedDB
- Подходит для больших данных (offline-first). Но защищайте от XSS (JS доступен).

## JWT/Token storage
- Access token в `Authorization: Bearer` header; refresh token в `HttpOnly` cookie.
- Short-lived tokens + rotate refresh tokens (revoke on compromise).
- Подписывайте (RS256/ES256), проверяйте `aud`, `iss`, `exp`, `nbf`, `iat`.

## Secret management
- Не committing secrets в repo (git-secrets, pre-commit hooks).
- Используйте `.env` + vault (Doppler, Vault, Parameter Store).
- Frontend: секреты должны быть доступными только backend (proxy API). Public env (NEXT_PUBLIC_*) — не секрет.

## Storage events
- Используйте `storage` event (JS) для синхронизации вкладок (logout). Учитывайте XSS — sanitize.

