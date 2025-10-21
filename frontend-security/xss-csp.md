# XSS, CSP и Trusted Types

## Типы XSS
- **Reflected XSS** — скрипт возвращается сразу (фильтруйте вход).
- **Stored XSS** — скрипт сохранён в БД/контенте (markdown, комментарии).
- **DOM-based XSS** — на клиенте (document.write, innerHTML, `location`/`hash`).

## Предотвращение XSS
- Escape output (контекстно): HTML, attribute, URL, CSS.
- Не используйте `innerHTML` / `document.write` с пользовательскими данными. Вместо этого `textContent`, DOM API.
- Sanitize input (DOMPurify, Sanitizer API).
- Content Security Policy (CSP).
- Trusted Types (Chrome) — контроль sink'ов (`innerHTML`, `eval`).

## CSP пример
```http
Content-Security-Policy: default-src 'self'; script-src 'self' 'nonce-abc123' https://apis.google.com; object-src 'none'; base-uri 'self'; frame-ancestors 'none';
```
- `default-src 'self'` — по умолчанию разрешены только текущий origin.
- `script-src 'nonce-...'` — для inline скриптов используйте nonce или hash.
- `object-src 'none'` — запрет Flash/plug-ins.
- `frame-ancestors` — защита от clickjacking.

## Trusted Types
```html
<meta http-equiv="Content-Security-Policy" content="trusted-types default; require-trusted-types-for 'script'">
```
- Используйте DOMPurify с Trusted Types (`DOMPurify.sanitize(..., { RETURN_TRUSTED_TYPE: true })`).
- Запрещает напрямую присваивать строки в опасные sink'и.

## React/Frameworks
- React: `dangerouslySetInnerHTML` — избегать (sanitize перед использованием).
- Vue/Angular — escape по умолчанию, но beware `v-html`, `innerHTML` binding.
- SSR → XSS через шаблоны (escape server-side).

## Third-party scripts
- Устанавливайте Subresource Integrity (SRI):
```html
<script src="https://cdn.com/library.js" integrity="sha384-..." crossorigin="anonymous"></script>
```
- Минимизируйте количество сторонних скриптов, ограничивайте разрешения (CSP). 

## DOMPurify пример
```js
import DOMPurify from 'dompurify';
const sanitized = DOMPurify.sanitize(userInput);
content.innerHTML = sanitized;
```

