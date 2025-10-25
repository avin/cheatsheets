## HTML Templates

- Определяйте шаблон в документе:
  ```html
  <template id="toast-template">
    <style>
      :host { position: fixed; bottom: 1rem; right: 1rem; }
      .toast { background: var(--toast-bg, #222); color: #fff; padding: 12px; border-radius: 6px; }
    </style>
    <div class="toast">
      <slot name="icon"></slot>
      <slot></slot>
    </div>
  </template>
  ```
- В компоненте:
  ```js
  const tpl = document.getElementById("toast-template");
  const fragment = tpl.content.cloneNode(true);
  this.shadowRoot.append(fragment);
  ```
- Для повторного использования кэшируйте `DocumentFragment`.
- Рендер на лету:
  ```js
  const tpl = document.createElement("template");
  tpl.innerHTML = `<style>...</style><slot></slot>`;
  this.shadowRoot.append(tpl.content.cloneNode(true));
  ```

## Slots и fallback контент

- `default` slot:
  ```html
  <slot></slot>
  ```
- Named slots:
  ```html
  <slot name="header"></slot>
  ```
  ```html
  <popup-card>
    <h3 slot="header">Title</h3>
    Body text
  </popup-card>
  ```
- Fallback:
  ```html
  <slot name="header">Default header</slot>
  ```
- Событие `slotchange`:
  ```js
  this.shadowRoot.querySelector("slot[name='header']")
    .addEventListener("slotchange", (event) => {
      const assigned = event.target.assignedNodes({ flatten: true });
      // обновить состояние
    });
  ```

## Композиционные паттерны

- Контролируйте layout внутри компонента, предоставляя ключевые слоты (header, footer, actions).
- Для списков/таблиц используйте `<slot>` с `::slotted(*)` для минимальной стилизации, оставляя свободу пользователю.
- Если нужен контент в специфическом месте внутри nested компонент, подойдет «slot forwarding»:
  ```html
  <!-- внутри child -->
  <slot name="footer" slot="footer"></slot>
  ```
  В родителе `<slot name="footer"></slot>`.

## Мульти-рендеринг

- Перерисовывайте частично:
  ```js
  const listSlot = this.shadowRoot.querySelector("slot[name='item']");
  const container = this.shadowRoot.getElementById("items");
  container.innerHTML = "";
  items.forEach((item) => {
    const node = document.createElement("div");
    node.textContent = item.label;
    container.append(node);
  });
  ```
- Для большего контроля используйте `DocumentFragment` и `insertBefore`.

## Динамические child-компоненты

- Создавайте дочерние элементы программно и передавайте данные:
  ```js
  const item = document.createElement("accordion-item");
  item.setAttribute("heading", "FAQ");
  item.innerHTML = "<p>Ответ</p>";
  this.append(item);
  ```
- Следите за integrity: при удалении дочернего элемента вызывайте методы cleanup, если компонент держит ссылки.

## Reprojection

- Переносите полученный слот дальше:
  ```html
  <!-- shell-component -->
  <inner-component>
    <slot slot="inner"></slot>
  </inner-component>
  ```
- Управляйте порядком: `slot.assignedElements({ flatten: true })` → `element.append`.

## server-side templates

- SSR можно сделать через `<template>` + `customElements.upgrade()`; передавайте данные через атрибуты/JSON (`<script type="application/json">` внутри).
- Используйте `innerHTML =` только с доверенным контентом (см. секцию безопасности).
