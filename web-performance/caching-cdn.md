# Кэширование, CDN и хранение

## HTTP caching
- Headers: `Cache-Control`, `Expires`, `ETag`, `Last-Modified`.
- Static assets: `Cache-Control: max-age=31536000, immutable` + hashed filenames.
- HTML/SSR: `Cache-Control: no-cache` + ETag (revalidate).
- Conditional requests: `If-None-Match`, `If-Modified-Since`.

## Service Worker caching
- Приложение shell → precache.
- Runtime caching (API responses, images).
- Manage versioning (remove old caches).

## CDN
- Использование CDN (Cloudflare, AWS CloudFront, Fastly).
- Geo distribution, TLS termination, HTTP/2.
- Purge cache (API), stale-if-error, stale-while-revalidate.

## Browser storage
- `localStorage`, `IndexedDB`, `Cache Storage`.
- Estimate storage (`navigator.storage.estimate()`).
- Лимиты: 5-10% диска (зависит от браузера).

## Compression
- Gzip, Brotli. Для изображения: WebP, AVIF.
- `Accept-Encoding` negotiation.
- Inline critical assets (HTML/CSS inline) ограниченно — следите за TTFB.

## Data API caching
- GraphQL/REST caching (HTTP caching, Apollo, SWR/React Query).
- Stale-While-Revalidate pattern (SWR).
- CDN caching for APIs (APIs on edge, Vercel Edge Functions, Cloudflare Workers).

