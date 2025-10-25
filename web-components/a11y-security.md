## Accessibility базис

- Назначайте роль и метки:
  ```js
  connectedCallback() {
    this.setAttribute("role", "button");
    this.setAttribute("tabindex", this.hasAttribute("disabled") ? "-1" : "0");
    this.setAttribute("aria-pressed", String(this.pressed));
  }
  ```
- Используйте `ElementInternals`:
  ```js
  this._internals = this.attachInternals();
  this._internals.role = "switch";
  this._internals.ariaDisabled = String(this.disabled);
  ```
- Поддерживайте клавиатуру: `keydown` → Space/Enter для элементов, эмулирующих кнопки.
- `delegatesFocus: true` + `:host(:focus-visible)` — контроль стилей фокуса.

## ARIA и `formAssociated`

- Для элементов форм:
  ```js
  static formAssociated = true;
  connectedCallback() {
    this._internals.setFormValue(this.value);
  }
  ```
- `aria-describedby`, `aria-labelledby` → связывайте с внутренними элементами (используйте `id` в shadow и `this.shadowRoot.getElementById` для связи).
- Избегайте дублирования `role`/`ARIA`, если используете настоящие `<button>`, `<input>` внутри shadow.

## Focus менеджмент

- Сохраняйте порядок табуляции: `:host { display: inline-block; }`.
- Чтобы прокинуть фокус внутрь:
  ```js
  this.attachShadow({ mode: "open", delegatesFocus: true });
  ```
- В противном случае `focus()` должен фокусировать внутренний элемент:
  ```js
  focus() {
    this.shadowRoot.querySelector("button").focus();
  }
  ```
- Обрабатывайте `focusin/focusout` для синхронизации классов.

## Обработка ошибок и fallback

- Если компонент критичен, предоставьте fallback-контент:
  ```html
  <custom-player>
    <a href="video.mp4">Скачать видео</a>
  </custom-player>
  ```
- При ошибках рендера отображайте сообщения внутри shadow.

## Security

- Не внедряйте непроверенный HTML в shadow:
  ```js
  const allowed = DOMPurify.sanitize(userHTML, { RETURN_DOM_FRAGMENT: true });
  this.shadowRoot.append(allowed);
  ```
- Избегайте eval-подобных конструкций; Web Components — обычный JS, CSP с `script-src 'self'` работает штатно.
- Для шаблонов используйте `TrustedHTML` (в браузерах поддерживающих Trusted Types):
  ```js
  this.shadowRoot.innerHTML = window.trustedTypes?.defaultPolicy?.createHTML(markup) ?? markup;
  ```
- Ограничивайте доступ к закрытым API; не храните секреты в атрибутах.

## CSP и sandbox

- Web Components совместимы с CSP — заранее добавляйте `nonce` на inline-стили, если сервер прописывает политику.
- Для `<iframe sandbox>` убедитесь, что функции работают без доступа к `customElements` (подключайте скрипты внутри iframe).

## Международная поддержка

- Учитывайте `dir="rtl"`: `:host([dir="rtl"])` → swap layout.
- Используйте `Intl` API для форматов внутри компонента, или передавайте форматированную строку.
- Реагируйте на `lang` изменения: `element.lang` или `:host-context([lang="..."])`.

## Тестирование доступности

- axe-core + @open-wc/testing:
  ```js
  import { fixture, expect, a11ySuite } from "@open-wc/testing";
  a11ySuite("fancy-card");
  ```
- Проверяйте с Screen Reader (NVDA, VoiceOver).
- Проверяйте табуляцию, high contrast mode (`forced-colors`), prefers-reduced-motion.
