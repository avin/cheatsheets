# Code splitting и загрузка

Стратегии разделения кода и оптимизации загрузки для крупных React-приложений.

## React.lazy и Suspense

- **Когда**: компоненты используются не всегда (модалки, графики, маршруты).
- **Плюсы**: автоматически создаёт отдельный бандл, ленивый импорт с обработкой состояния загрузки.
- **Минусы**: требуется `Suspense`-обёртка и fallback, поддержка SSR ограничена (нужен `@loadable/component` или ручная интеграция).

В примере `Dashboard` лениво загружает компонент графика и показывает skeleton пока модуль не подтянулся.

```tsx
const Chart = React.lazy(() => import('./Chart'));

function Dashboard() {
  return (
    <Suspense fallback={<Skeleton />}>
      <Chart />
    </Suspense>
  );
}
```

## Динамические импорты с условиями

- **Паттерн**: грузите тяжёлые библиотеки только по требованию (например, редактор, карты).
- **Инструменты**: `import()` внутри обработчиков, `prefetch` заранее (Webpack magic comments, Vite `import.meta.glob`).
- **Советы**: используйте `await` внутри `startTransition`, чтобы не блокировать UI.

Функция `loadEditor` подгружает Markdown-редактор только тогда, когда он действительно нужен, и сохраняет компонент в локальном состоянии.

```tsx
async function loadEditor() {
  const { MarkdownEditor } = await import(/* webpackChunkName: "md-editor" */ './MarkdownEditor');
  setEditor(() => MarkdownEditor);
}
```

## Splitting по маршрутам

- **React Router**: `lazy` в конфигурации маршрутов, `defer` для данных.
- **Next.js App Router**: папка `app/segment` автоматически создаёт чанки, `loading.tsx` для fallback.
- **Практика**: группируйте связанные страницы, используйте bundle анализатор, чтобы избегать дублирования.

Конфигурация маршрутизатора ниже делит приложение на чанки по разделам — каждая ветка `reports` загружает свой компонент при первом входе.

```tsx
const router = createBrowserRouter([
  {
    path: '/',
    async lazy() {
      const { Home } = await import('./routes/Home');
      return { Component: Home };
    },
  },
  {
    path: '/reports',
    async lazy() {
      const { ReportsLayout } = await import('./routes/reports/Layout');
      return { Component: ReportsLayout };
    },
    children: [
      {
        path: 'sales',
        async lazy() {
          const { SalesReport } = await import('./routes/reports/Sales');
          return { Component: SalesReport };
        },
      },
    ],
  },
]);
```

## Prefetch/Preload ресурсов

- **Цель**: загрузить скрипты/данные заблаговременно, пока пользователь читает страницу.
- **Инструменты**: `<link rel="prefetch">`, `<link rel="preload">`, Next.js `next/script`, Vite `preload`.
- **Советы**: префетчите вероятные маршруты, очищайте устаревшие чанки через версионирование.
- **Риски**: избыточный prefetch ухудшает UX на медленных сетях; проверяйте `navigator.connection.saveData` и пропускайте подсказку, если пользователь просит экономию трафика.

`PrefetchLink` создаёт `<link rel="prefetch">`, чтобы браузер заранее скачал страницу, на которую пользователь, скорее всего, перейдёт.

```tsx
function PrefetchLink({ to, children }: { to: string; children: React.ReactNode }) {
  const linkRef = useRef<HTMLAnchorElement>(null);

  useEffect(() => {
    const el = document.createElement('link');
    el.rel = 'prefetch';
    el.href = to;
    document.head.appendChild(el);
    return () => document.head.removeChild(el);
  }, [to]);

  return (
    <a ref={linkRef} href={to}>
      {children}
    </a>
  );
}
```

## Бандлер-специфичные оптимизации

- **Webpack**: `SplitChunksPlugin`, `dynamic imports`, `module federation`, `bundle analyzer`.
- **Vite/Rollup**: `optimizeDeps`, `manualChunks`, `import.meta.glob`.
- **Turbopack/rspack**: автоматическое prefetch/prefetch hints, поддержка импортов из npm.

Конфиг Vite вручную выделяет общие зависимости (`vendor`) и «тяжёлый» блок графиков, чтобы браузер кэшировал их раздельно.

```js
// vite.config.ts
export default defineConfig({
  build: {
    rollupOptions: {
      output: {
        manualChunks: {
          vendor: ['react', 'react-dom', 'react-router-dom'],
          charts: ['chart.js', 'react-chartjs-2'],
        },
      },
    },
  },
});
```

## Критический рендер и серверные подсказки

- **Паттерн**: выделяйте критические ресурсы (CSS, минимальный JS) в `<head>`, остальное загружайте асинхронно.
- **Инструменты**: Next.js `app/head.tsx`, `link rel="modulepreload"`, HTTP/2 push (заменён на 103 Early Hints).
- **Советы**: используйте `ReactDOM.preconnect`, CDN для крупного JS, измеряйте влияние на LCP/INP.
