## Жизненный цикл и рендер

- Избегайте тяжёлой логики в `constructor` — инициализацию откладывайте до `connectedCallback`.
- Обновления группируйте:
  ```js
  if (this._updateScheduled) return;
  this._updateScheduled = true;
  queueMicrotask(() => {
    this._updateScheduled = false;
    this.render();
  });
  ```
- Рендерите через `DocumentFragment` во избежание layout thrash.
- Не вызывайте `this.render()` внутри `attributeChangedCallback`, если значение не изменилось.

## Работа с DOM

- Batch изменения:
  ```js
  const list = this.shadowRoot.getElementById("list");
  const fragment = document.createDocumentFragment();
  items.forEach((item) => {
    const li = document.createElement("li");
    li.textContent = item.label;
    fragment.append(li);
  });
  list.replaceChildren(fragment);
  ```
- Используйте `requestAnimationFrame` для визуальных обновлений, `requestIdleCallback` — для ленивых.
- `ResizeObserver`/`IntersectionObserver` — для ленивой инициализации (создавайте при подключении, уничтожайте при отключении).

## Кэширование и memoization

- Кешируйте статические template-фрагменты:
  ```js
  const tpl = document.createElement("template");
  tpl.innerHTML = `<style>...</style><slot></slot>`;
  class XBox extends HTMLElement {
    connectedCallback() {
      if (!this.shadowRoot) {
        this.attachShadow({ mode: "open" }).appendChild(tpl.content.cloneNode(true));
      }
    }
  }
  ```
- Сохраняйте вычисленные значения в WeakMap/Map, особенно если данные приходят извне.

## Отложенная инициализация

- Лениво создавайте shadow DOM:
  ```js
  connectedCallback() {
    if (!this.shadowRoot) {
      this.attachShadow({ mode: "open" });
      this.render();
    }
  }
  ```
- Используйте `IntersectionObserver` для отложенной загрузки ресурсов:
  ```js
  this._io = new IntersectionObserver((entries) => {
    if (entries.some((e) => e.isIntersecting)) {
      this.loadData();
      this._io.disconnect();
    }
  });
  this._io.observe(this);
  ```

## SSR/SSG и hydration

- Pre-render HTML и вставляйте `customElements.upgrade` после загрузки модулей.
- Для синхронизации состояния используйте атрибуты/`data-*` + JSON-скрипты.
- Stencil/Lit поддерживают SSR/Hydration из коробки; для ручной реализации флаг `this.hasConnected` помогает избегать двойного рендера.

## Memory management

- Удаляйте ссылки в `disconnectedCallback`, прежде чем элемент будет собран GC.
- `AbortController` → `.abort()` для всех слушателей/наблюдателей.
- Не выбрасывайте огромные массивы в атрибуты; храните их во внешнем сторе.

## Style recalculation

- Используйте `adoptedStyleSheets` + кэш — быстрее, чем `<style>` на каждый инстанс.
- Избегайте динамического `innerHTML` внутри shadow (дорогой reparse).
- Используйте CSS custom properties вместо inline-стилей для динамики — браузер делает меньше перерисовок.

## Layout и paint

- `element.getBoundingClientRect()` делает layout — ограничивайте количество вызовов.
- Для анимаций используйте `transform` и `opacity`.
- Почти все Web Components — inline блоки; устанавливайте `display` (например, `:host { display: contents; }` для «renderless»).

## Diagnostics

- `performance.mark/measure` внутри `render` для оценки времени.
- Chrome DevTools: Performance → «Web Vitals», «Layout Shift Regions».
- Web Vitals (CLS, LCP) — измеряйте с помощью `web-vitals` пакета и эмитите события.
