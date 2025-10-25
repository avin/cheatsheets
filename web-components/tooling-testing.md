## Инструментарий

- **Lit** — минималистичный слой над Web Components:
  ```ts
  import { LitElement, html, css } from "lit";
  class FancyCard extends LitElement {
    static styles = css`:host { display: block; }`;
    static properties = { title: { type: String } };
    render() { return html`<h2>${this.title}</h2><slot></slot>`; }
  }
  customElements.define("fancy-card", FancyCard);
  ```
- **FAST** (Microsoft) — дизайн-система + модель наблюдения.
- **Stencil** — компилятор, генерирует стандартные Web Components + optional hydration.
- **Hybrids**, **Haunted**, **Atomico** — альтернативные реактивные слои.
- При выборе инструмента следите за размером рантайма и потребностями проекта.

## Сборка и упаковка

- Совместимость ES Modules:
  ```json
  {
    "type": "module",
    "exports": "./dist/index.js",
    "module": "./dist/index.js"
  }
  ```
- Rollup/Vite:
  ```js
  // rollup.config.mjs
  export default {
    input: "src/index.js",
    output: [
      { file: "dist/index.js", format: "es", sourcemap: true }
    ],
    plugins: [resolve(), commonjs(), terser()]
  };
  ```
- Добавляйте `custom-elements.json` (W3C draft) для документирования API.
- Для tree-shaking избегайте singleton и side effects.

## Типы и документация

- TypeScript декларации:
  ```ts
  declare global {
    interface HTMLElementTagNameMap {
      "fancy-card": FancyCard;
    }
  }
  ```
- JSDoc комментарии — для автогенерации документации (Storybook Docs, API Reference).
- `tsconfig`: `compilerOptions.lib` включает `"DOM", "DOM.Iterable", "ES2022"`; `emitDecoratorMetadata` не нужен.

## Тестирование

- **Web Test Runner (@web/test-runner)**:
  ```js
  import { expect, fixture } from "@open-wc/testing";
  it("renders slot content", async () => {
    const el = await fixture(`<fancy-card>Hi</fancy-card>`);
    expect(el.shadowRoot.querySelector("slot")).to.exist;
  });
  ```
- **Playwright / Cypress** — e2e сценарии, проверка взаимодействия с приложением.
- **Jest + @webcomponents/custom-elements** (polyfill) — unit-тесты без браузера (jsdom + happy-dom).
- **Storybook** — документация + визуальные регрессии (Chromatic, Loki).
- Golden tests: `@web/test-runner-visual-regression`.

## Линтинг и форматирование

- ESLint: `plugin:wc/recommended`, `@open-wc/eslint-config`.
- Stylelint с поддержкой `:host`, `::slotted` (добавьте `custom-elements` к селекторам).
- Prettier — форматирование `.js`, `.ts`, `.html` шаблонов (Lit поддерживается через `prettier-plugin-lit`).

## Dev-сервер и HMR

- Vite/Web Dev Server (`@web/dev-server`) имеют HMR для modules, но `customElements.define` нельзя переопределить.
- Используйте `customElements.get` guard + `import.meta.hot.accept`.
- При hot reload пересоздавайте shadow DOM (`this.shadowRoot.innerHTML = ""`) в `connectedCallback`.

## Debugging

- Browser DevTools:
  - Elements панель: вкладка «Event Listeners», «Properties».
  - `Shadow DOM` можно раскрыть (Chrome DevTools > Settings > Preferences > Elements > Show user agent shadow DOM).
- `customElements.getName(el.constructor)` (Chrome) возвращает имя элемента.
- Логируйте lifecycle-hooks с `console.log` и `performance.mark`.

## Дистрибуция

- Публикуйте на npm как ESM + `types`.
- Для CDN используйте `esm.sh`, `jspm`, `skypack` — следите за CORS и integrity.
- Создавайте `index.mjs` (tree-shakable exports) и `define.js` (side-effect define) для удобства потребителей:
  ```js
  // define.js
  import { FancyCard } from "./index.js";
  if (!customElements.get("fancy-card")) {
    customElements.define("fancy-card", FancyCard);
  }
  ```
