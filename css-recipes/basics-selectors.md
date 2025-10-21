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
- Порядок подключения: применяйте `@layer` или `:where()` для снижения специфичности.

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

