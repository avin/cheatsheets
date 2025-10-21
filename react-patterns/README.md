# React Patterns шпаргалка

Справочник по паттернам организации React-кода: композиция компонентов, управление состоянием, асинхронные данные, оптимизация производительности и устойчивость UI.

### 🧩 [Компонентная архитектура](./component-architecture.md)

- Контейнеры/презентеры, композиция и контракт props
- Render props, compound-компоненты, контролируемые элементы
- Многослойные провайдеры и разделение ответственности

### 🪝 [Паттерны хуков и состояния](./hooks-state-patterns.md)

- Разделение локального и глобального state
- useReducer, кастомные хуки, мемоизация вычислений
- Контекст-селекторы, атомарные сторы, синхронизация с URL

### 🌐 [Загрузка данных и конкурентность](./data-fetching-async.md)

- useEffect + AbortController, дедупликация запросов
- Suspense, concurrent rendering, React Query/SWR
- Consistency, повторные попытки, синхронизация с сервером

### ⚡ [Оптимизация производительности](./performance-optimizations.md)

- Мемоизация компонентов и функций
- Виртуализация списков, transitions, дымовые тесты производительности
- Анализ Rerender через профилировщики и ESLint

### 🛡️ [Устойчивость и обработка ошибок](./error-boundaries-resilience.md)

- Error boundaries, fallback UI, retry-паттерны
- Работа с Suspense, graceful degradation
- Интеграция с мониторингом и логированием

### 🧾 [Формы и валидация](./forms-validation.md)

- Контролируемые/неконтролируемые формы, формы-библиотеки
- Схемы валидации (Zod/Yup), оптимистичные сабмиты
- Управление состоянием ошибок, accessibility форм

### 🧭 [Маршрутизация и layout](./routing-layout.md)

- Nested маршруты, layout-деревья, protected routes
- Data APIs React Router и Next.js App Router
- Prefetch, загрузка данных на уровне маршрута

### 🔄 [Сайд-эффекты и синхронизация](./side-effects-sync.md)

- Таймеры, подписки, WebSocket/SSE
- Finite-state машины, эффекты через Redux Toolkit/Zustand
- useSyncExternalStore, event sourcing, optimistic updates

### 🧱 [Code splitting и загрузка](./code-splitting.md)

- React.lazy, Suspense boundaries, динамические импорты
- Prefetch/Preload, критический рендер
- Стратегии для маршрутов, компонентов и библиотек

### 🖥️ [Server Components и Actions](./server-components.md)

- Разделение client/server компонентов, стриминг
- Server Actions, мутации и ре-валидирование
- Ограничения окружений и шаринг кода

### ♿ [Доступность и порталы](./accessibility-portals.md)

- Порталы, focus management, trap и возврат фокуса
- useId, aria-атрибуты, keyboard UX
- Паттерны модалок, меню, всплывающих окон

### 🧪 [Тестирование компонентов](./testing-patterns.md)

- React Testing Library, Jest, Vitest
- Тестирование хуков, Storybook тесты, визуальные регрессии
- Стратегии e2e и контрактных тестов

### 📊 [Data-heavy UI](./data-intensive-ui.md)

- Таблицы и гриды, агрегации, инкрементальная загрузка
- Виртуализация с редактированием, бесконечные списки
- Кэширование и консистентность данных

### 📈 [Наблюдаемость и метрики](./observability-metrics.md)

- reportWebVitals, пользовательские метрики
- Интеграция с Sentry, Datadog, OpenTelemetry
- Производственные алерты и анализ деградаций
