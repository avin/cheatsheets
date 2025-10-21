# Доступность и порталы

Паттерны создания доступных интерактивных компонентов в React с использованием порталов и управления фокусом.

## Порталы и модальные окна

- **Когда**: нужно вывести контент поверх всего приложения (модалки, тултипы), не нарушая DOM-иерархию.
- **Паттерн**: рендер через `createPortal` в элемент вне основного дерева (например, `#modal-root`).
- **Советы**: блокируйте прокрутку фона (`body.style.overflow`), возвращайте фокус после закрытия.

```tsx
function Modal({ open, onClose, children }: Props) {
  const container = document.getElementById('modal-root');
  const lastFocused = useRef<HTMLElement | null>(null);

  useEffect(() => {
    if (open) {
      lastFocused.current = document.activeElement as HTMLElement;
    } else if (lastFocused.current) {
      lastFocused.current.focus();
    }
  }, [open]);

  if (!open || !container) return null;

  return createPortal(
    <div role="dialog" aria-modal="true" className="modal">
      <button type="button" onClick={onClose} aria-label="Закрыть">
        ×
      </button>
      {children}
    </div>,
    container,
  );
}
```

## Focus Trap и циклическая навигация

- **Цель**: при открытии модалки клавиатура остаётся внутри компонента.
- **Инструменты**: `focus-trap`, `@headlessui/react`, `@radix-ui/react-dialog`.
- **Паттерн**: на `Tab` перемещать фокус по фокусируемым элементам, на `Escape` закрывать.
- **Риски**: самописные реализации легко пропускают edge-case (disabled элементы, reverse Tab, screen readers), поэтому для продакшена лучше использовать battle-tested библиотеки.

```tsx
function useFocusTrap(active: boolean, containerRef: React.RefObject<HTMLElement>) {
  useEffect(() => {
    if (!active) return;
    const elements = containerRef.current?.querySelectorAll<HTMLElement>(
      'a[href], button, textarea, input, select, [tabindex]:not([tabindex="-1"])',
    );
    if (!elements?.length) return;
    const first = elements[0];
    const last = elements[elements.length - 1];

    function handleKey(event: KeyboardEvent) {
      if (event.key !== 'Tab') return;
      if (event.shiftKey && document.activeElement === first) {
        event.preventDefault();
        last.focus();
      } else if (!event.shiftKey && document.activeElement === last) {
        event.preventDefault();
        first.focus();
      }
    }

    document.addEventListener('keydown', handleKey);
    return () => document.removeEventListener('keydown', handleKey);
  }, [active, containerRef]);
}
```

## useId и связь элементов

- **Проблема**: при SSR/CSR ID должны совпадать, чтобы ARIA-атрибуты работали корректно.
- **Решение**: используйте `useId` для генерации стабильных идентификаторов, объединяйте их в `aria-labelledby`, `aria-describedby`.

```tsx
function Tooltip({ label, children }: { label: string; children: React.ReactElement }) {
  const id = useId();
  return (
    <>
      {cloneElement(children, { 'aria-describedby': id })}
      <span role="tooltip" id={id} className="tooltip">
        {label}
      </span>
    </>
  );
}
```

## Доступные меню и раскрывающиеся списки

- **Паттерн**: для меню используйте `role="menu"` и `role="menuitem"`, обрабатывайте клавиши `ArrowUp/Down`, `Enter`, `Space`, `Escape`.
- **Инструменты**: Headless UI, Radix UI, Reach UI — уже реализуют ARIA-паттерны.
- **Советы**: храните фокусируемый индекс в state, объявляйте `aria-haspopup`, `aria-expanded`.

```tsx
function Menu() {
  const [open, setOpen] = useState(false);
  const [activeIndex, setActiveIndex] = useState(0);
  const items = ['Профиль', 'Настройки', 'Выход'];

  const handleKey = (event: React.KeyboardEvent<HTMLDivElement>) => {
    if (event.key === 'ArrowDown') {
      setActiveIndex((index) => (index + 1) % items.length);
    } else if (event.key === 'ArrowUp') {
      setActiveIndex((index) => (index - 1 + items.length) % items.length);
    } else if (event.key === 'Escape') {
      setOpen(false);
    }
  };

  return (
    <div className="menu">
      <button
        type="button"
        aria-haspopup="menu"
        aria-expanded={open}
        onClick={() => setOpen((value) => !value)}
      >
        Меню
      </button>
      {open && (
        <div role="menu" tabIndex={-1} onKeyDown={handleKey}>
          {items.map((item, index) => (
            <button
              key={item}
              role="menuitem"
              className={index === activeIndex ? 'active' : ''}
              onMouseEnter={() => setActiveIndex(index)}
            >
              {item}
            </button>
          ))}
        </div>
      )}
    </div>
  );
}
```

## Сообщения и live region

- **Применение**: уведомления, обновления состояния, результаты валидации.
- **Паттерн**: `aria-live="polite"` для ненавязчивых, `aria-live="assertive"` для критичных сообщений.
- **Советы**: ограничивайте количество объявлений, избегайте спама, очищайте после прочтения.

```tsx
function LiveAnnouncements({ message }: { message: string | null }) {
  return (
    <div aria-live="polite" aria-atomic="true" className="sr-only">
      {message}
    </div>
  );
}
```

## Проверка доступности

- **Инструменты**: axe DevTools, Lighthouse, Storybook a11y, Testing Library `toHaveAccessibleName`.
- **Процессы**: включайте a11y чек-листы в PR, тестируйте клавиатурой, используйте скринридеры (VoiceOver, NVDA).
- **Метрики**: отслеживайте ошибки в production через Sentry Performance или A11y линтеры в CI.
