## Регистрация и создание

- Создавайте класс, наследующий `HTMLElement` (или `HTMLButtonElement` для customized built-ins):
  ```js
  class FancyCard extends HTMLElement {
    constructor() {
      super();
      this.attachShadow({ mode: "open" });
    }
  }
  customElements.define("fancy-card", FancyCard);
  ```
- Проверяйте повторную регистрацию для hot reload:
  ```js
  if (!customElements.get("fancy-card")) {
    customElements.define("fancy-card", FancyCard);
  }
  ```
- Customized built-ins (ограниченная поддержка, только Chrome):
  ```js
  customElements.define("fancy-button", FancyButton, { extends: "button" });
  // <button is="fancy-button">
  ```

## Жизненный цикл

- Конструктор: минимум логики; не обращайтесь к DOM вне shadow до `connectedCallback`.
- `connectedCallback` — инициализация DOM, подписки на события.
- `disconnectedCallback` — отписки, cleanup timers/observers.
- `adoptedCallback` — реакция на перемещение между документами/iframe.
- `attributeChangedCallback(name, oldValue, newValue)` — синхронизация атрибутов.
- Регистрируйте наблюдаемые атрибуты:
  ```js
  static get observedAttributes() {
    return ["variant", "open"];
  }
  ```

## Свойства и атрибуты

- Рефлексия:
  ```js
  get open() {
    return this.hasAttribute("open");
  }
  set open(value) {
    this.toggleAttribute("open", Boolean(value));
  }
  attributeChangedCallback(name, _, value) {
    if (name === "open") {
      this.render();
    }
  }
  ```
- Для чисел/булевых значений используйте парсинг (`Number(value)`, `value !== null`).
- Не создавайте бесконечный цикл «сеттер → `setAttribute` → `attributeChanged`» — сравнивайте старое и новое значение.

## Работа с шаблоном

- Используйте `<template>`:
  ```html
  <template id="card-tpl">
    <style> :host { display: block; } </style>
    <slot name="header"></slot>
    <div class="content"><slot></slot></div>
  </template>
  ```
  ```js
  const template = document.getElementById("card-tpl");
  this.shadowRoot.appendChild(template.content.cloneNode(true));
  ```
- Лениво клонируйте при первом подключении (экономия памяти, если элемент не используется).

## Способы подключения

- HTML:
  ```html
  <fancy-card variant="outlined">
    <h2 slot="header">Заголовок</h2>
    Контент
  </fancy-card>
  ```
- JS:
  ```js
  const card = document.createElement("fancy-card");
  card.variant = "tonal";
  document.body.append(card);
  ```

## `customElements.upgrade`

- Для предрендеренного HTML вызывайте `customElements.upgrade(root)` после загрузки класса, чтобы привести существующие узлы к классу.
- Используйте `customElements.whenDefined("fancy-card").then(...)` для дождаться регистрации перед обращением.

## Progressive enhancement

- Предоставляйте статический HTML внутри `<fancy-card>` как fallback, чтобы пользователи без JS видели контент (Shadow DOM не будет создан).
- Проверяйте наличие API:
  ```js
  if ("customElements" in window) {
    // зарегистрировать
  }
  ```
- Поддерживайте SSR: выводите легковесный HTML + скрипт с lazy load `customElements.define`.

## Общие советы

- Старайтесь избегать глобальных зависимостей внутри конструктора; передавайте конфигурацию через атрибуты/свойства.
- Держите приватное состояние на WeakMap или внутри `#private` полей класса.
- Используйте `AbortController` для управления подписками и clean-up:
  ```js
  this._controller = new AbortController();
  window.addEventListener("resize", () => this.resize(), { signal: this._controller.signal });
  ```
- В `disconnectedCallback` — `this._controller.abort();`.
