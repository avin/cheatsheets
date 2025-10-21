# Доступность и ARIA

## Основы a11y
- Семантические теги → screen readers понимают структуру.
- Alt-тексты для изображений (`alt="Descriptive text"`).
- Управление focus: используйте `tabindex="0"` для того, чтобы элемент участвовал в таб-навигации; `tabindex="-1"` — только программный focus. Избегайте `tabindex` > 0.
- Contrast (WCAG 2.1 AA ≥ 4.5:1).

## ARIA roles
- Используйте когда нет семантического эквивалента.
- Примеры ролей: `role="button"`, `role="dialog"`, `role="navigation"`, `role="alert"`.
- ARIA не заменяет корректную разметку — "ARIA is for developers, not for content".

## ARIA атрибуты
- `aria-label`, `aria-labelledby` — подпись.
- `aria-describedby` — дополнительное описание.
- `aria-hidden="true"` — скрыть от screen reader.
- `aria-live="polite"|"assertive"` — объявление динамических изменений.
- `aria-expanded`, `aria-controls` для аккордеонов.

## Forms
- `<label for="id">` связывает с `input`.
- Error сообщения: `aria-invalid="true"`, `aria-describedby="error-id"`.
- Use `fieldset`/`legend`.

## Keyboard accessibility
- Всё взаимодействие должно быть доступно через клавиатуру (`Enter`, `Space`, стрелки).
- Custom компоненты: обрабатывайте `keydown`, `keyup`.
- Focus management: `focus()` после появления модальных окон.

## Skip links
```html
<a class="skip-link" href="#main">Skip to content</a>
```
- Разместите в начале для быстрого перехода.

## Testing
- Screen reader (NVDA, VoiceOver, JAWS).
- Lighthouse Accessibility, Axe DevTools.
- Keyboard-only walkthrough.

