## Shadow DOM режимы

- `this.attachShadow({ mode: "open" })` — доступ к `shadowRoot` извне (нормальный сценарий).
- `mode: "closed"` — скрытый shadow, `element.shadowRoot` вернёт `null`.
- Можно передать `delegatesFocus: true` для кастомных контролов, которые автоматически получают фокус внутри shadow.

## Scoped стили

- Shadow DOM изолирует CSS: стили хоста не протекают внутрь и наоборот.
- Общая структура:
  ```js
  const root = this.attachShadow({ mode: "open" });
  root.innerHTML = `
    <style>
      :host { display: block; }
      .card { border: 1px solid var(--card-border, #ccc); }
      ::slotted(h2) { margin: 0; }
    </style>
    <article class="card">
      <slot name="header"></slot>
      <slot></slot>
    </article>
  `;
  ```
- `:host` — сам элемент (можно использовать `:host([variant="filled"])`).
- `:host-context(.dark)` — адаптация под внешнюю тему.
- `::slotted(selector)` — стилизация прямых детей, переданных в конкретный слот.
- Для глубоких потомков снаружи используйте CSS Custom Properties.

## CSS Custom Properties и темы

- Объявляйте переменные на хосте:
  ```css
  fancy-card {
    --card-border: rgba(255,255,255,0.2);
    --card-radius: 12px;
  }
  ```
- В shadow:
  ```css
  .card {
    border-color: var(--card-border, #ddd);
    border-radius: var(--card-radius, 8px);
  }
  ```
- Используйте fallback значения для совместимости.

## adoptedStyleSheets

- Современный способ повторно использовать CSS:
  ```js
  const sheet = new CSSStyleSheet();
  await sheet.replace(`
    :host { display: grid; gap: 12px; }
  `);
  this.shadowRoot.adoptedStyleSheets = [sheet, otherSheet];
  ```
- Работает в современных браузерах (Chrome, Edge, Safari 16.4+, FF 101+ через флаг).
- Можно кэшировать `CSSStyleSheet` между инстансами.
- Для polyfill — используйте `constructable-stylesheets-polyfill`.

## Style Parts

- Для «дырочек» в стиле экспортируйте части:
  ```css
  button::part(label) { font-weight: 600; }
  ```
  ```js
  this.shadowRoot.innerHTML = `
    <style>
      .label { padding: 0.5rem; }
    </style>
    <span class="label" part="label"><slot></slot></span>
  `;
  ```
- Внешний CSS может таргетить `::part(label)` и переопределять стили.
- `::theme` (эксперимент) — аналог для темизации, пока слабо поддерживается.

## Глобальные стили

- Поддержка `@import` внутри shadow `<style>` работает, но влияет только на конкретный shadow.
- Если нужна общая глобальная тема, соединяйте CSS custom properties и `adoptedStyleSheets`.
- Для отдельных состояний (`:focus-visible`, `:disabled`) используйте `:host([disabled])`.

## Анимации и эффекты

- CSS анимации работают как обычно. Для вхождения/выхода можно использовать Web Animations API:
  ```js
  this.shadowRoot.querySelector(".card").animate(
    [{ transform: "scale(0.95)", opacity: 0 }, { transform: "scale(1)", opacity: 1 }],
    { duration: 200, easing: "ease-out" }
  );
  ```
- `:host(:focus-visible)` сочетайте с `delegatesFocus`.

## CSS Reset внутри Shadow

- При необходимости подключайте минимальный reset, чтобы избежать сюрпризов:
  ```css
  :host { box-sizing: border-box; }
  *, *::before, *::after { box-sizing: inherit; }
  ```
- Учитывайте, что пользовательские стили не попадут внутрь — предоставляйте хуки через custom properties и `part`.

## Интеграция с design tokens

- Поддерживайте `--token` переменные для цвета, типографики, spacing.
- Используйте `ElementInternals` (`this.attachInternals().states`) для CSS state API (экспериментально).
