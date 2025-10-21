# Практики, инструменты и performance

## Архитектура CSS
- BEM (Block__Element--Modifier) — структурированная методология имен.
- ITCSS, SMACSS — организуйте слои (settings, tools, generic, elements, objects, components, utilities).
- CSS Modules / CSS-in-JS — изоляция стилей.
- Utility-first (Tailwind) — набор классов.

## Препроцессоры / PostCSS
- Sass/SCSS: переменные, mixins, nested, `@use`/`@forward`.
- PostCSS: autoprefixer, postcss-preset-env, nesting.
- Less, Stylus (legacy).

## Build tools
- Autoprefixer: добавляет вендорные префиксы.
- Lintинг: Stylelint.
- CSS Minify: cssnano, PurgeCSS (удаление неиспользуемых классов).
- CSS-in-JS: Emotion, Styled Components — beware bundle size.

## Performance
- Сократите `repaint`/`reflow`: избегайте тяжелых `box-shadow`/`filter` на множестве элементов.
- Анимации → `transform` и `opacity` (GPU friendly).
- Критический CSS: inline для above-the-fold, остальные async (load CSS asynchronously).
- Reduce HTTP requests (combine, use HTTP2).

## Debugging tools
- Browser DevTools: Layout, Animations, Color picker.
- `outline: 1px solid red;` (temporarily) для отладки.
- Accessibility: Lighthouse, Axe.

## Variables & theming
- Define themes: `[data-theme="dark"] { --bg: #111; }`.
- CSS `@layer` + `:where()` для управления специфичностью.

## Reset / normalize
- CSS reset (Meyer, modern CSS reset) или normalize.css для базовой типографики.

## Deployment
- Content hashing (`styles.[hash].css`).
- HTTP headers: `Cache-Control`, `ETag`.
- Monitor bundle size (Webpack analyzer, Bundlephobia).

