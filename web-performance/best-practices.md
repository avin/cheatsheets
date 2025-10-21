# Best practices и чек-лист

## Загрузка
- Минимизируйте bundle (code splitting, remove dead code, tree shaking).
- Prefetch/Preload критических ресурсов.
- Lazy load изображений, компонентов, third-party скриптов.
- Используйте HTTP/2, Brotli.

## Рендеринг
- Уменьшайте CLS: резервируйте место для изображений/iframe.
- Убирайте long tasks (>50ms), разбивайте на chunks (`requestIdleCallback`).
- CSS критический inline, остальное async.
- JS → `defer`, `async`, `module`.

## Network
- CDN для статики, API edge caching.
- Cache-Control: `max-age`, `stale-while-revalidate`, `immutable`.
- SPA → prefetch routes (React Router, Next.js).

## Images & Fonts
- WebP/AVIF, SVG icons, `<picture>` + srcset/sizes.
- `font-display: swap`, preload критических шрифтов.

## Monitoring & budgets
- Установите performance budget (LCP < 2.5s, CLS < 0.1, INP < 200ms).
- RUM + Synthetic (Lighthouse, WebPageTest). Alerts при превышении.
- Интеграция с CI (Lighthouse CI) → падение build при нарушении.

## Progressive enhancement
- Не ломайтесь без JS (но degrade gracefully).
- Feature detection (`@supports`, `if ('serviceWorker' in navigator)`).

## Third-party audits
- Оценивайте scripts (tag manager, analytics). Lazy load, consent.
- Удаляйте неиспользуемые теги/SDK.
- CSP / security headers (см. Security шпаргалку).

## QA Checklist
- Lighthouse score > 90 (Performance).
- Web Vitals удовлетворяют (LCP/FID/CLS/INP).
- Offline fallback (если PWA).
- Testing на 3G/CPU 4x slowdown.

