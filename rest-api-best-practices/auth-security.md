# Аутентификация и авторизация

## Auth модели
- **API Keys** — простой вариант для server-to-server. Добавлять в header (`X-API-Key`).
- **Basic Auth** — только для простых случаев (TLS обязательно).
- **OAuth 2.0** — стандарт для delegated auth (client_credentials, authorization_code, device_code).
- **OIDC** — надстройка над OAuth для аутентификации юзеров (ID Token).
- **JWT** — токены с клеймами (подпись). Управляйте сроком жизни и revoke.
- **mTLS** — взаимная аутентификация, полезна для внутренней инфраструктуры.

## Access control
- RBAC vs ABAC: проверка ролей/атрибутов.
- "Scopes" (`read:user`, `write:orders`).
- Логируйте изменения ролей/политик.

## Token handling
- Храните секреты в vault, rotate.
- Refresh токены: ограничивайте время жизни, revoke при компрометации.
- Правильно проверяйте `aud`, `iss`, `exp`, `nbf`.

## Безопасность транспортного уровня
- Обязательно HTTPS/TLS.
- Проводите регулярные пен-тесты и проверяйте актуальность ciphersuite.

## Rate limiting / throttling
- Обязателен для public API.
- Защита от brute force (login endpoint) — капчи, lockout после N попыток.

## Input validation & sanitization
- Лимит на размер (payload, headers).
- Очистка/escaping (SQL, XXE, injection).

## Content Security
- `Content-Security-Policy`, `X-Frame-Options`, `X-Content-Type-Options`, `Referrer-Policy`.

## Audit
- Логируйте успешные/неуспешные аутентификации, изменения данных, доступы к чувствительным ресурсам.
