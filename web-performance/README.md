# Web Performance шпаргалка

Рекомендации по оптимизации скорости веб-приложений: метрики, загрузка ресурсов, рендеринг, кэширование, инструменты мониторинга и best practices.

### 📊 [Метрики и измерение](./metrics-measurement.md)

- Core Web Vitals (LCP, CLS, FID/INP)
- RUM vs Synthetic (Lighthouse, WebPageTest)
- Performance budgets
- PerformanceObserver & custom metrics

### ⚡ [Оптимизация загрузки и сети](./loading-network.md)

- HTTP/2/3, preload/prefetch
- Lazy loading (images, code)
- Optimизация шрифтов и JS
- Third-party scripts, consent

### 🎨 [Рендеринг и пайплайн](./rendering-pipeline.md)

- Critical rendering path
- CSS/JS impact, long tasks
- CLS mitigation, transforms
- Workers, requestIdleCallback

### 🗃️ [Кэширование, CDN и хранение](./caching-cdn.md)

- HTTP caching headers
- Service Worker caching
- CDN (Cloudflare, Fastly)
- Compression, API caching

### 🛠️ [Инструменты и мониторинг](./tools-monitoring.md)

- DevTools, Lighthouse, WebPageTest
- RUM tools (web-vitals, Sentry)
- Bundle analyzers, automation
- Monitoring dashboards & alerts

### ✅ [Best practices и чек-лист](./best-practices.md)

- Загрузка, рендеринг, network
- Images & fonts оптимизация
- Budgets + CI integration
- Third-party audits, QA checklist
