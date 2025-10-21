# Визуальные эффекты и анимации

## Transitions
```css
.button {
  transition: background 200ms ease, transform 200ms ease;
}
.button:hover {
  background: var(--brand-hover);
  transform: translateY(-2px);
}
```
- Свойства: `transition-property`, `duration`, `timing-function`, `delay`. Используйте предустановки (`ease`, `ease-in-out`) или `cubic-bezier(0.4, 0, 0.2, 1)` для кастомных кривых.

## Animations (keyframes)
```css
@keyframes pulse {
  0%, 100% { transform: scale(1); }
  50% { transform: scale(1.05); }
}
.card:hover {
  animation: pulse 1.5s ease-in-out both;
}
```
- Контроль: `animation-iteration-count`, `animation-fill-mode`.

## Transforms
- `transform: translate/scale/rotate/skew;`
- 3D: `transform: perspective(600px) rotateY(15deg);`
- `transform-origin` для центра.

## Filters и backdrop
- `filter: blur(5px)`, `grayscale`, `drop-shadow`.
- `backdrop-filter: blur(10px)` (поддержка Safari/Chromium).

## Shadows
- `box-shadow: 0 4px 12px rgba(0,0,0,0.2)`.
- `text-shadow: 1px 1px 2px rgba(0,0,0,0.6)`.

## Clip-path / mask
- `clip-path: polygon(...)` для фигур.
- `mask-image: linear-gradient(...)`.

## Scroll effects
- `scroll-snap-type`, `scroll-behavior: smooth`.
- Intersection Observer (JS) для lazy-load/animations on scroll.

## prefers-reduced-motion
```css
@media (prefers-reduced-motion: reduce) {
  * {
    animation-duration: 0.001ms !important;
    animation-iteration-count: 1 !important;
    transition-duration: 0.001ms !important;
  }
}
```
- Учитывайте пользователей с чувствительностью к движению.

