# Оптимизация производительности

Подходы к снижению количества ререндеров, уменьшению размера бандла и улучшению UX при работе React-приложений.

## Мемоизация компонентов и селекторов

- **Инструменты**: `React.memo`, `useMemo`, `useCallback`, мемоизированные селекторы (`reselect`).
- **Паттерн**: мемоизируйте компоненты, которые получают стабильные props и дорого рендерятся; мемоизируйте функции, которые передаются вниз по дереву.
- **Риски**: избыточная мемоизация усложняет код и может создать утечки, если зависимости указаны неверно.

В примере `ListItem` обёрнут в `React.memo`, поэтому он проверяет, изменились ли `item` и `onSelect`. Родитель создаёт стабильный обработчик через `useCallback`, чтобы пропы не пересоздавались и лишние ререндеры не происходили.

```tsx
const ListItem = React.memo(function ListItem({ item, onSelect }: Props) {
  return (
    <li>
      <button onClick={() => onSelect(item.id)}>{item.title}</button>
    </li>
  );
});

const List = ({ items, onSelect }: { items: Item[]; onSelect: (id: string) => void }) => {
  const stableSelect = useCallback((id: string) => onSelect(id), [onSelect]);
  return (
    <ul>
      {items.map((item) => (
        <ListItem key={item.id} item={item} onSelect={stableSelect} />
      ))}
    </ul>
  );
};
```

## Разделение контекста и `useSyncExternalStore`

- **Цель**: избежать ререндеров всего дерева при изменении глобального состояния.
- **Паттерн**: используйте несколько контекстов (например, `ThemeContext`, `UserContext`) или внешнее хранилище с `useSyncExternalStore`.
- **Инструменты**: Zustand, Jotai, Redux Toolkit 2.0 (`useSelector` на селекторах), `valtio`.

`useSyncExternalStore` подписывается на внешнее состояние и вызывает селектор, что позволяет избежать прокидывания пропсов вниз по дереву и контролировать, какие части состояния вызывают ререндер.

```tsx
const store = createStore(initialState);

function useStore(selector: (state: State) => any) {
  return useSyncExternalStore(store.subscribe, () => selector(store.getState()));
}
```

## Виртуализация списков

- **Когда**: большие таблицы/ленты (1000+ элементов), где полный рендер невозможен.
- **Инструменты**: `react-window`, `react-virtual`, `tanstack/react-virtual`.
- **Советы**: задавайте фиксированную высоту/ширину, обрабатывайте ресайз, используйте «скелетоны» для загрузки.

`react-virtual` рассчитывает, какие строки видны пользователю, и рисует только их. Высота контейнера задаёт окно, а `transform: translateY` позволяет имитировать полную высоту списка без траты ресурсов на невидимые элементы.

```tsx
const rowVirtualizer = useVirtualizer({
  count: rows.length,
  getScrollElement: () => parentRef.current,
  estimateSize: () => 48,
});

return (
  <div ref={parentRef} style={{ height: 400, overflow: 'auto' }}>
    <div style={{ height: `${rowVirtualizer.getTotalSize()}px`, position: 'relative' }}>
      {rowVirtualizer.getVirtualItems().map((virtualRow) => (
        <div
          key={virtualRow.key}
          style={{
            position: 'absolute',
            top: 0,
            left: 0,
            width: '100%',
            transform: `translateY(${virtualRow.start}px)`,
          }}
        >
          <Row row={rows[virtualRow.index]} />
        </div>
      ))}
    </div>
  </div>
);
```

## Concurrent features и transitions

- **startTransition**: помечает обновление как некритичное, оставляя UI отзывчивым (например, фильтры, поиск).
- **useDeferredValue**: отложенное значение, полезно для синхронизации ввода и отображения.
- **Советы**: показывайте индикаторы загрузки, поддерживайте `isPending` из `useTransition`.

Здесь ввод сохраняется мгновенно, а фильтрация списка помещена в `startTransition`. Пользователь видит индикатор `isPending`, пока тяжелая операция выполняется в фоновом режиме.

```tsx
function SearchBox() {
  const [query, setQuery] = useState('');
  const [filtered, setFiltered] = useState(items);
  const [isPending, startTransition] = useTransition();

  const handleChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const next = event.target.value;
    setQuery(next);
    startTransition(() => {
      setFiltered(filterItems(items, next));
    });
  };

  return (
    <>
      <input value={query} onChange={handleChange} placeholder="Поиск" />
      {isPending && <Spinner />}
      <Results items={filtered} />
    </>
  );
}
```

## Анализ и профилактика деградаций

- **Профилирование**: React DevTools Profiler, Lighthouse, `why-did-you-render`.
- **ESLint**: правила `react-hooks/exhaustive-deps`, `react-perf/jsx-no-new-object-as-prop`, `@next/next/no-img-element`.
- **Буджеты**: настройте performance budgets в CI, отслеживайте LCP/INP, используйте `reportWebVitals`.
- **Превентивные меры**: отключайте development-only коды через условную компиляцию, используйте `babel-plugin-transform-remove-console`, анализируйте бандл (`source-map-explorer`, `webpack-bundle-analyzer`).
