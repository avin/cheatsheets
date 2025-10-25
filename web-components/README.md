# Web Components шпаргалка

Практические рецепты по созданию, стилизации и интеграции Custom Elements, Shadow DOM и HTML Templates без тяжёлых фреймворков.

### 🧱 [Основы и жизненный цикл](./basics-lifecycle.md)

- Регистрация `customElements`, наследование от `HTMLElement`.
- Жизненный цикл (`connectedCallback`, `attributeChangedCallback`, `adoptedCallback`).
- Работа с `observedAttributes`, `upgrade()` и graceful fallback.

***

### 🎨 [Shadow DOM и стили](./shadow-styling.md)

- Создание shadow root, `attachShadow` режимы.
- Scoped CSS, `:host`, `::slotted`, CSS custom properties.
- Темизация, `adoptedStyleSheets`, Style Parts.

***

### 🧩 [Шаблоны, слоты и композиция](./templates-slots.md)

- `<template>`, клонирование и lazy instantiation.
- Named slots, fallback content, `slotchange`.
- Паттерны композиции и интеграция стороннего контента.

***

### 🔄 [Состояние и события](./state-events.md)

- Рефлексия атрибутов ↔ свойств, `ElementInternals`.
- Custom events, `dispatchEvent`, bubbling/compose.
- Синхронизация с хост-приложением, data binding без фреймворков.

***

### 🔧 [Инструменты и тестирование](./tooling-testing.md)

- Lit, FAST, Stencil как вспомогательные слои.
- Storybook, Playwright, Web Test Runner, @web/test-runner.
- Bundling (Vite/Rollup), типы (TypeScript) и linting.

***

### 🚀 [Производительность и оптимизация](./performance.md)

- Lifecycle best practices, batching DOM операций.
- Rehydration, `IntersectionObserver`, `ResizeObserver`.
- SSR/SSG подходы, pre-render и hydration стратегий.

***

### ♿ [Accessibility и безопасность](./a11y-security.md)

- Доступность: ARIA, `ElementInternals`, focus delegation.
- Формы и `formAssociated` элементы.
- Security (CSP, sandbox, доверенные шаблоны).

***

### 🔗 [Интероп и паттерны интеграции](./interop-patterns.md)

- Встраивание в React/Vue/Svelte, события и refs.
- Микрофронтенды, lazy-loading, versioning.
- Публикация npm-пакетов, документация и контрактные тесты.
