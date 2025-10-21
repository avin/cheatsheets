# Типографика и цвета

## Fonts
- Подключение web fonts:
```css
@font-face {
  font-family: 'Inter';
  font-weight: 400;
  src: url('/fonts/inter-regular.woff2') format('woff2');
  font-display: swap;
}
body {
  font-family: 'Inter', system-ui, -apple-system, 'Segoe UI', sans-serif;
}
```
- Поддерживайте fallback (system fonts).
- `font-variant`, `font-feature-settings` для ligatures.

## Размеры
- Используйте `rem` (1 rem = root font-size) для масштабируемости.
- `clamp()` для fluid typography: `font-size: clamp(1rem, 2vw + 0.5rem, 1.5rem);`

## Line height и spacing
- `line-height: 1.5;` — комфортное чтение.
- `letter-spacing`, `word-spacing`.
- `text-transform`, `text-align`, `text-decoration`.

## Цвета
- Форматы: HEX (`#1e90ff`), RGB, HSL (`hsl(210, 100%, 56%)`), `rgba`, `hsla`.
- Используйте HSL для лёгкой корректировки (оттенки).
- CSS Color Level 4: `color-mix(in srgb, #1e90ff 60%, white)`, новые color spaces (`lab`, `lch`).

## Variables для темы
```css
:root {
  --bg: #fff;
  --text: #111;
}
[data-theme='dark'] {
  --bg: #121212;
  --text: #f1f1f1;
}
body {
  background: var(--bg);
  color: var(--text);
}
@media (prefers-color-scheme: dark) {
  :root {
    --bg: #121212;
    --text: #f1f1f1;
  }
}
```
- Учитывайте `prefers-color-scheme` для автоматического переключения темы.

## Text overflow
- `text-overflow: ellipsis; white-space: nowrap; overflow: hidden;`.
- `hyphens: auto;` для переносов.

## Accessibility
- Контраст: WCAG AA (конtrast >= 4.5:1). Используйте инструменты проверки.
- Учитывайте prefers-reduced-motion.
- Используйте `<mark>`, `<strong>` семантически.

