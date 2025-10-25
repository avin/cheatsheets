## Интеграция с фреймворками

- **React**:
  - Используйте `ref` для обращения к элементу и настройке событий:
    ```tsx
    function FancyCardWrapper(props) {
      const ref = useRef(null);
      useEffect(() => {
        const handler = (event) => props.onToggle?.(event.detail);
        ref.current?.addEventListener("toggle", handler);
        return () => ref.current?.removeEventListener("toggle", handler);
      }, [props.onToggle]);
      return <fancy-card ref={ref} variant={props.variant}>{props.children}</fancy-card>;
    }
    ```
  - Для атрибутов со строковыми значениями используйте `element.setAttribute`; для boolean — `toggleAttribute`.
- **Vue 3**:
  - Добавьте `defineCustomElement` или `app.config.compilerOptions.isCustomElement`.
  - Используйте `v-model:prop` для двусторонней связи (через события).
- **Svelte**:
  - В `svelte.config.cjs`: `kit.vite = { ssr: { noExternal: ["web-components-lib"] } }`.
  - События: `<fancy-card on:toggle={handle}>`.
- **Angular**:
  - `schemas: [CUSTOM_ELEMENTS_SCHEMA]` в модуле.
  - События через `fromEvent`.

## Микрофронтенды

- Публикуйте компоненты как независимые пакеты, версионируйте через SemVer.
- Используйте `customElements.define` guard, чтобы избежать переопределений при multiple bundles.
- Загружайте компоненты лениво:
  ```js
  import("./components/fancy-card.js");
  ```
  Или через `IntersectionObserver`.
- При Module Federation экспортируйте файл, который регистрирует все элементы.

## Разделение по пакетам

- `index.js` — именованные экспорты классов (для tree-shaking).
- `define.js` — функция, которая регистрирует все элементы:
  ```js
  export function defineComponents(prefix = "") {
    if (!customElements.get(`${prefix}fancy-card`)) {
      customElements.define(`${prefix}fancy-card`, FancyCard);
    }
  }
  ```
- Позволяет потребителям регистрировать с namespace (`acme-card`).

## Документация и каталоги

- Формируйте `stories`:
  ```js
  export const Default = () => html`<fancy-card>Content</fancy-card>`;
  ```
- Используйте `zeroheight`/`Backlight` для guideline дизайн-системы.
- Генерируйте `custom-elements.json` (команда `lit analyze`, `stencil build --docs`).

## Контрактные тесты

- Pact: описывайте события/атрибуты, проверяйте интеграцию с сервисами.
- OpenAPI → генерируйте клиенты внутри компонента (если он делает fetch).
- Снимайте скриншоты в Storybook (Chromatic) для регрессий.

## Хостинг и дистрибуция

- CDN: публикуйте `dist/define.js`, добавляйте `<script type="module" src="https://cdn.example.com/fancy-card.js"></script>`.
- ESBuild/ Rollup → генерируйте `iife` для устаревших окружений (если нужно):
  ```js
  format: "iife", name: "FancyCardBundle"
  ```
- Добавляйте `modulepreload` ссылку и `import()` на стороне приложения.

## Версионирование и миграции

- Document breaking changes (атрибуты, события, CSS parts).
- Используйте `Deprecation` warning внутри компонента:
  ```js
  if (this.hasAttribute("old-prop")) {
    console.warn("[fancy-card] 'old-prop' is deprecated; use 'variant'");
  }
  ```
- Поддерживайте `major.minor.patch` и changelog.
