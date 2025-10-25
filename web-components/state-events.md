## Свойства и состояние

- Храните внутренние данные в полях или WeakMap:
  ```js
  const privates = new WeakMap();
  class ToggleSwitch extends HTMLElement {
    constructor() {
      super();
      privates.set(this, { checked: false });
    }
  }
  ```
- Отражайте состояние в DOM через атрибуты/классы для CSS.
- Используйте `ElementInternals`:
  ```js
  this._internals = this.attachInternals();
  this._internals.states.add("selected");
  ```

## Атрибуты ↔ свойства

- Создайте единый источник правды (обычно свойство):
  ```js
  get checked() {
    return this.hasAttribute("checked");
  }
  set checked(value) {
    this.toggleAttribute("checked", Boolean(value));
    this.render();
  }
  attributeChangedCallback(name, _, value) {
    if (name === "checked") this.render();
  }
  ```
- Для строковых свойств используйте `this.getAttribute`/`this.setAttribute`.
- Boolean: `this.hasAttribute("disabled")`, `this.toggleAttribute`.
- JSON/объекты передавайте через `element.props = {...}` или `dataset`.

## События

- Dispatch:
  ```js
  this.dispatchEvent(new CustomEvent("toggle", {
    detail: { checked: this.checked },
    bubbles: true,
    composed: true,
  }));
  ```
- `bubbles: true` — поднимается по DOM; `composed: true` — выходит из shadow.
- Для `input`-подобных элементов дублируйте нативные события (`input`, `change`).
- Не забывайте `event.preventDefault()` внутри компонента, если нужно перехватить действие.

## Form-associated Custom Elements

- Включайте `static formAssociated = true;`.
- Используйте `ElementInternals`:
  ```js
  this._internals = this.attachInternals();
  this._internals.setFormValue(this.value);
  ```
- Реализуйте методы:
  - `formAssociatedCallback(form)`
  - `formResetCallback()`
  - `formStateRestoreCallback(state, mode)`
- Помечайте валидность:
  ```js
  this._internals.setValidity({ valueMissing: true }, "Укажите значение", this.input);
  ```

## Реактивность без фреймворков

- Используйте `MutationObserver` для слежения за внешними изменениями:
  ```js
  this._observer = new MutationObserver(() => this.update());
  this._observer.observe(this, { attributes: true, childList: true });
  ```
- Лёгкий data-binding:
  ```js
  update(props) {
    Object.assign(this.state, props);
    this.render();
  }
  ```
- Примешивайте `requestAnimationFrame` для batch:
  ```js
  queueMicrotask(() => this.render());
  ```
- Для сложных состояний рассмотрите Lit (`@state()`), но базовая реализация должна оставаться без зависимостей.

## Communication API

- DOM события — основной контракт с внешним миром.
- Для callback’ов предоставляйте свойства:
  ```js
  set onToggle(fn) { this._onToggle = fn; }
  ```
  Вызывайте `this._onToggle?.(detail);`.
- Использование `dispatchEvent` позволяет интегрироваться с любым фреймворком.

## Взаимодействие с сервером

- Fetch внутри компонента:
  ```js
  async connectedCallback() {
    this.shadowRoot.innerHTML = "Loading...";
    const data = await fetchJSON(this.src);
    this.render(data);
  }
  ```
- Используйте `AbortController` для отмены запросов при `disconnectedCallback`.
- Кеширование данных (Cache API, IndexedDB) организуйте вне компонента или через dependency injection.

## Observers

- `IntersectionObserver` — lazy загрузка внутри компонента.
- `ResizeObserver` — подстройка layout.
- Следите за clean-up: `this._resizeObs.disconnect()` в `disconnectedCallback`.
