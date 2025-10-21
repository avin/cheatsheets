# Оптимизация загрузки и сети

## HTTP оптимизация
- Используйте HTTP/2 или HTTP/3 (multiplexing, header compression).
- Объединяйте запросы (но не переусердствуйте — HTTP/2 справляется с множеством).
- Уменьшайте вес: минификация, обрезка комментов, tree shaking.
- Gzip/Brotli (Brotli для HTML/JS/CSS).

## Resource hints
- `<link rel="preload" href="/fonts/font.woff2" as="font" type="font/woff2" crossorigin>`.
- `<link rel="prefetch" href="/next-page" as="document">` (низкий приоритет).
- `<link rel="dns-prefetch" href="//cdn.example.com">`.
- `<link rel="preconnect" href="https://api.example.com" crossorigin>`.

## Lazy loading
- `loading="lazy"` для изображений/iframe.
- `IntersectionObserver` для кастомной lazy загрузки.
- Код-сплиттинг: динамические `import()`.

## Шрифты
- `font-display: swap` в @font-face.
- Preload critical fonts, `unicode-range` для subset.
- Включайте системные fallback.

## JS загрузка
- `defer` для скриптов, `async` (только если порядок не важен).
- Inline критические JS, postpone heavy scripts (split).
- Webpack/Vite code splitting, split by route/component.
- Prefetch chunk'и для next screen (`React.lazy`, `Suspense`).

## Сторонние скрипты
- Оцените «стоимость» (send to webpagetest / request map).
- Загружайте async, defer, dynamic import.
- Планируйте удаление/отложенную загрузку (tag manager). Используйте Consent manager.

## Network
- Keep connection reuse, avoid blocking requests.
- HTTP caching headers (see caching section).
- для SPA: prefetch data (Query cache, prefetch routers).

