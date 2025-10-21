# Устойчивость и обработка ошибок

Стратегии обработки ошибок, graceful degradation и интеграции с мониторингом в React.

## Error Boundary

- **Назначение**: перехват ошибок рендера в дочерних компонентах, предотвращение падения всего дерева.
- **Ограничения**: не ловит ошибки в обработчиках событий, асинхронном коде (Promise) и серверном рендеринге.
- **Практика**: реализуйте `componentDidCatch` и `getDerivedStateFromError`, выводите fallback, логируйте ошибку.

Классический boundary меняет состояние при первой ошибке и отображает запасной UI. Метод `componentDidCatch` — место, куда можно отправить ошибку в логирование (Sentry, Datadog).

```tsx
class ErrorBoundary extends React.Component<
  { fallback: React.ReactNode; onError?: (error: Error, info: React.ErrorInfo) => void },
  { hasError: boolean }
> {
  state = { hasError: false };

  static getDerivedStateFromError() {
    return { hasError: true };
  }

  componentDidCatch(error: Error, info: React.ErrorInfo) {
    this.props.onError?.(error, info);
  }

  render() {
    if (this.state.hasError) {
      return this.props.fallback;
    }
    return this.props.children;
  }
}
```

## Комбинация ErrorBoundary + Suspense

- **Паттерн**: оборачивайте асинхронный UI в `Suspense` и `ErrorBoundary`, чтобы показывать fallback во время загрузки и ошибки отдельно.
- **Советы**: обеспечьте разные fallback-компоненты для критичных и некритичных частей, добавьте retry, если ошибка временная.

Комбинация ниже даёт пользовательский опыт: сначала показывается скелетон, а при фатальной ошибке — экран ошибки. Error boundary также может логировать инцидент и предложить «Повторить».

```tsx
<ErrorBoundary fallback={<ErrorState />}>
  <Suspense fallback={<Skeleton />}>
    <UserProfile />
  </Suspense>
</ErrorBoundary>
```

## Retry UI и экстракция побочных эффектов

- **Задача**: предоставить пользователю возможность восстановиться после временной ошибки (сеть, API).
- **Инструменты**: `useQuery` с `retry`, собственные хуки с `retryFn`, UI с кнопкой «Повторить».
- **Практика**: храните последний успешный ответ, показывайте оптимистичный UI, различайте ошибки пользователя и системы.

`ErrorFallback` — минимальный компонент, который принимает обработчик повтора. Его удобно переиспользовать в разных местах, передавая `refetch` из React Query или собственный retry.

```tsx
function ErrorFallback({ onRetry }: { onRetry: () => void }) {
  return (
    <div role="alert">
      <p>Не удалось загрузить данные.</p>
      <button onClick={onRetry}>Повторить</button>
    </div>
  );
}
```

## Грейсфул деградация

- **Принципы**: показывайте skeletonы или упрощённую версию UI, сохраняйте возможность ключевых действий (например, офлайн-режим).
- **Паттерн Feature Toggle**: отключайте проблемные возможности через флаги; используйте remote-конфиг (LaunchDarkly, Unleash).
- **Сохраняйте контекст**: при ошибке формы оставляйте введённые пользователем данные, если это безопасно.

## Мониторинг и алертинг

- **Сбор ошибок**: интеграция с Sentry, LogRocket, Honeybadger; используйте `Sentry.wrap` или `ErrorBoundary` с `captureException`.
- **Дополнительные данные**: логируйте `componentStack`, `window.navigator.onLine`, версию приложения, `release`.
- **Алерты**: настройте пороговые значения (например, spike 5xx), связывайте события с релизами, отправляйте отчёты на Slack/Teams.

## Работа с асинхронными ошибками

- **Поймайте**: в обработчиках событий оборачивайте async в `try/catch`, используйте `window.addEventListener('unhandledrejection', ...)`.
- **Fallback Router**: в React Router 6.4+ используйте `errorElement`, `useRouteError` для страницы ошибки.
- **Server Actions**: в Next.js ловите ошибки на сервере, возвращайте `redirect` или `notFound`, логируйте через observability-платформы.
