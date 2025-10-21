# Layout: Flexbox и Grid

## Flexbox
- Контейнер: `display: flex;` (или `inline-flex`).
- Основные свойства контейнера:
  - `flex-direction: row|column`.
  - `flex-wrap: wrap` (перенос строк).
  - `justify-content`: `flex-start`, `center`, `space-between`, `space-around`, `space-evenly`.
  - `align-items`: `stretch`, `center`, `flex-start`.
  - `gap` — расстояние между элементами.
- Свойства элементов:
  - `flex: 1` (grow, shrink, basis).
  - `flex-grow`, `flex-shrink`, `flex-basis`.
  - `align-self` (перекрывает align-items).

```css
.container {
  display: flex;
  flex-direction: row;
  gap: 1rem;
}
.card {
  flex: 1 1 200px;
}
```

## Grid
- Контейнер: `display: grid; grid-template-columns/rows`.
- Определение колонок/строк:
  - `grid-template-columns: repeat(3, 1fr);`
  - `grid-template-columns: 200px 1fr minmax(150px, 20%);`
- Gap: `gap: 16px;`
- Выравнивание: `justify-items`, `align-items`, `justify-content`, `align-content`.
- Placement: `grid-column: 1 / span 2;`.
- Areas:
```css
.grid {
  display: grid;
  grid-template-areas:
    "header header"
    "sidebar content"
    "footer footer";
}
.header { grid-area: header; }
```

## Responsive layout
- `@media (min-width: 768px) { ... }` — media queries для breakpoints.
- Container queries: объявите `container-type: inline-size;` на родителе и используйте `@container (min-width: 400px) { ... }`.
- Grid auto-fit пример:
  ```css
  .gallery {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 1rem;
  }
  ```
  Карточки автоматически подстраиваются под ширину.

## CSS functions
- `calc()`, `min()`, `max()`, `clamp()`.
- `clamp(1rem, 2vw, 2rem)` — адаптивный размер.

## Positioning
- `position: relative/absolute/fixed/sticky`.
- Sticky header: `position: sticky; top: 0;` (зависимость от родителя).

## Multi-column
- `column-count`, `column-gap` для текcта.

## Layout debug
- Chrome DevTools → Layout pane (Flex, Grid overlays).
- `outline: 1px solid` временно.

