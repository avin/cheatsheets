# Метрики и измерение

## Core Web Vitals
- **Largest Contentful Paint (LCP)** — время загрузки основного контента (<2.5s).
- **First Input Delay (FID)** — задержка первое взаимодействие (<100ms). Будет заменён на INP.
- **Interaction to Next Paint (INP)** — единый показатель отзывчивости (<200ms).
- **Cumulative Layout Shift (CLS)** — стабильность layout (<0.1).

## Дополнительные метрики
- TTFB (Time To First Byte).
- FCP (First Contentful Paint).
- TTI (Time To Interactive).
- TBT (Total Blocking Time).
- Speed Index.

## RUM vs Synthetic
- RUM (Real User Monitoring): реальные пользователи (Web Vitals API, Google Analytics, Sentry, NewRelic).
- Synthetic: Lighthouse, WebPageTest — лабораторные условия.

## Измерение
- Chrome DevTools → Performance tab → Web Vitals overlay.
- Lighthouse (CI/CD, bundler integration).
- Web Vitals JS library (`web-vitals`).
- Boomerang, Calibre, SpeedCurve.

## Performance budgets
- Установите бюджеты (bundle size, LCP, CLS).
- Используйте `lighthouse-ci`, `webpack-bundle-analyzer`.
- В CI: fail если превышено.

## Logging
- `navigator.sendBeacon`, `PerformanceObserver` для кастомных метрик.
- В SPA: `Performance.mark` / `measure`. Собирайте TTI, long tasks.

