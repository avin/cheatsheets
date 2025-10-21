# Наблюдаемость и метрики

Как измерять качество и стабильность React-приложений в продакшене.

## Web Vitals и пользовательские метрики

- **Паттерн**: импортируйте `reportWebVitals` (CRA) или `web-vitals`, отправляйте метрики на бекенд или аналитические сервисы.
- **Ключевые показатели**: LCP, INP, CLS, TTFB, FID. Дополнительно наблюдайте TTI, TBT, custom events.
- **Советы**: используйте секцию `navigator.connection` для сегментации по сети, связывайте метрику с пользователем (`id`).

```tsx
import { onCLS, onLCP, onINP } from 'web-vitals';

function sendToAnalytics(metric: Metric) {
  fetch('/analytics', {
    method: 'POST',
    body: JSON.stringify(metric),
    keepalive: true,
  });
}

onCLS(sendToAnalytics);
onLCP(sendToAnalytics);
onINP(sendToAnalytics);
```

## Мониторинг ошибок и логирование

- **Инструменты**: Sentry, Bugsnag, LogRocket, Datadog RUM.
- **Паттерн**: оборачивайте приложение провайдером, используйте `ErrorBoundary` для ручного захвата ошибок.
- **Советы**: добавляйте `tags` (версия, регион), `breadcrumbs`, передавайте sourcemaps в CI.

```tsx
import * as Sentry from '@sentry/react';

Sentry.init({
  dsn: process.env.SENTRY_DSN,
  integrations: [new Sentry.BrowserTracing(), new Sentry.Replay()],
  tracesSampleRate: 0.1,
  replaysSessionSampleRate: 0.05,
});
```

## Метрики производительности в runtime

- **API**: `PerformanceObserver`, `performance.mark/measure`, `requestIdleCallback`.
- **Паттерн**: измеряйте критические пользовательские действия (анимации, открытие модалок) и отправляйте метки на бекенд/аналитику.
- **Советы**: используйте идентификаторы сценариев, агрегируйте данные на сервере для дашбордов.

```tsx
function usePerfMeasure(label: string) {
  useEffect(() => {
    performance.mark(`${label}-start`);
    return () => {
      performance.mark(`${label}-end`);
      performance.measure(label, `${label}-start`, `${label}-end`);
      const [entry] = performance.getEntriesByName(label);
      analytics.track('perf:measure', { label, duration: entry.duration });
      performance.clearMarks(`${label}-start`);
      performance.clearMarks(`${label}-end`);
      performance.clearMeasures(label);
    };
  }, [label]);
}
```

## Инциденты и алертинг

- **Сигналы**: ошибки (кол-во/мин, spike), Web Vitals (LCP > 4s), отказ в авторизации.
- **Интеграции**: PagerDuty, Opsgenie, Slack-боты, Github status pages.
- **Практика**: определите SLO (например, 95% LCP < 2.5s), подключите алерты при дрейфе.

## Observability стек

- **Логи**: структурированные (JSON), включают `traceId`, `sessionId`.
- **Трейсинг**: OpenTelemetry → OTLP коллектор → Jaeger/Tempo. Используйте `@opentelemetry/api` + custom инжекторы.
- **Метрики**: Prometheus для server-side, InfluxDB/StatsD для дополнительного контекста.

```ts
// telemetry.ts
import { WebTracerProvider } from '@opentelemetry/sdk-trace-web';
import { ConsoleSpanExporter, SimpleSpanProcessor } from '@opentelemetry/sdk-trace-base';

const provider = new WebTracerProvider();
provider.addSpanProcessor(new SimpleSpanProcessor(new ConsoleSpanExporter()));
provider.register();
```

## Дистрибуция и релизные метки

- **Паттерн**: передавайте версию приложения (Git SHA) в запросы и отчёты, чтобы быстро найти деградации.
- **Инструменты**: `import.meta.env.VITE_APP_VERSION`, `process.env.NEXT_PUBLIC_RELEASE`.
- **Советы**: используйте feature flags для постепенного rollout, сравнивайте метрики по cohorts (control vs treatment).
