# Базовый синтаксис и селекторы

## Основы
- Правило: `selector { property: value; }`.
- Комментарии: `/* comment */`.
- Каскад: приоритет (Specificity) → inline style > ID > class > element.
- Inheritance: некоторые свойства наследуются (font, color).

## Селекторы
- Тип селектора: `div`, `p`.
- Class: `.btn`, `.card.primary`.
- ID: `#header` (использовать минимально).
- Группировка: `h1, h2, h3 { ... }`.
- Combinators:
  - `div p` (descendant),
  - `div > p` (child),
  - `div + p` (adjacent sibling),
  - `div ~ p` (general sibling).
- Attribute selectors: `input[type="email"]`, `a[href^="https"]`.
- Pseudo-classes: `:hover`, `:focus`, `:active`, `:nth-child(2n)`, `:not(.disabled)`, `:has()` (в новом CSS).
- Pseudo-elements: `::before`, `::after`, `::first-line`, `::marker`.

## Specificity tips
- Используйте классы, избегайте сложных селекторов.
- Порядок подключения: используйте `@layer` для определения приоритетов слоёв, `:where()`/`:is()` для объединения селекторов без повышения специфичности.
- Пример специфичности: `#id` (100) > `.class` (10) > `button` (1). Комбинации суммируются (`button.primary` ⇒ 11).

## Custom properties
```css
:root {
  --brand-color: #1e90ff;
}
.button {
  background: var(--brand-color);
}
```
- Можно менять на уровне компонентов (`.dark { --brand-color: #0d6efd; }`).

## Units
- `px`, `em`, `rem`, `%`, `vh`, `vw`, `vmin`, `vmax`.
- `rem` — относительный к корневому `font-size`.
- `min()`, `max()`, `clamp()` — динамические вычисления.


## Cascade layers
```css
@layer reset, base, components, utilities;
@layer reset {
  *, *::before, *::after { margin: 0; padding: 0; }
}
@layer base {
  body { font-family: var(--font, system-ui); }
}
```
- Объявляйте порядок слоёв один раз и подключайте файлы (`@import` c аргументом `layer`). Это делает каскад предсказуемым и упрощает переопределения.
