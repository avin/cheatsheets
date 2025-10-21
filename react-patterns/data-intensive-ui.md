# Data-heavy UI

Паттерны построения богатых интерфейсов с большими объёмами данных: таблицы, списки, агрегации.

## Виртуализованные таблицы с редактированием

- **Цель**: отображать десятки тысяч строк без потери производительности.
- **Инструменты**: `tanstack/table` + `react-virtual`, `ag-grid`, `mui-data-grid`.
- **Паттерн**: храните источник данных в сторе, виртуализируйте видимые строки, редактирование делайте по локальному state с батч-коммитами.

```tsx
const columnHelper = createColumnHelper<Row>();

const columns = [
  columnHelper.accessor('name', {
    header: 'Название',
    cell: (info) => <EditableCell value={info.getValue()} rowId={info.row.original.id} />,
  }),
  columnHelper.accessor('price', {
    header: 'Цена',
    cell: (info) => <NumberCell value={info.getValue()} />,
  }),
];

const table = useReactTable({ data: rows, columns, getCoreRowModel: getCoreRowModel() });
const rowVirtualizer = useVirtualizer({
  count: table.getRowModel().rows.length,
  getScrollElement: () => parentRef.current,
  estimateSize: () => 44,
});
```

## Инкрементальная загрузка и бесконечные списки

- **Паттерн**: используйте `useInfiniteQuery` (React Query) или `IntersectionObserver` для автоподгрузки страниц.
- **Советы**: добавьте `hasNextPage`, показывайте `Skeleton` перед следующей порцией, обрабатывайте ошибки с кнопкой «Повторить».

```tsx
const { data, fetchNextPage, hasNextPage, isFetchingNextPage } = useInfiniteQuery({
  queryKey: ['feed'],
  queryFn: ({ pageParam = 0 }) => fetchFeed(pageParam),
  getNextPageParam: (lastPage) => lastPage.nextCursor ?? undefined,
});
```

## Pivot-агрегации и мемоизация

- **Задача**: пересчитывать агрегаты (суммы, средние, группировки) без блокировки UI.
- **Паттерн**: используйте `useMemo` или Web Worker, разбивайте расчёты на чанки (`requestIdleCallback`).
- **Инструменты**: `arquero`, `duckdb-wasm`, `lodash.groupby`.

```tsx
const grouped = useMemo(() => {
  return data.reduce<Record<string, number>>((acc, item) => {
    acc[item.category] = (acc[item.category] ?? 0) + item.amount;
    return acc;
  }, {});
}, [data]);
```

## Stream и realtime для аналитики

- **Паттерн**: используйте SSE/WebSocket для обновления графиков и метрик, храните последние n значений.
- **Советы**: сглаживайте данные (moving average), ограничивайте частоту обновлений (`throttle`), показывайте индикаторы «live».

```tsx
function useMetricsStream() {
  const [metrics, setMetrics] = useState<Metric[]>([]);

  useEffect(() => {
    const source = new EventSource('/api/metrics/stream');
    source.addEventListener('message', (event) => {
      const update = JSON.parse(event.data) as Metric;
      setMetrics((prev) => [...prev.slice(-49), update]);
    });
    return () => source.close();
  }, []);

  return metrics;
}
```

## Skeleton и placeholder стратегии

- **Цель**: избежать «скачков» при загрузке данных и поддержать восприятие.
- **Паттерн**: skeleton для списков/таблиц, placeholder для графиков, `aria-busy` для доступности.
- **Инструменты**: React Query `initialData`, Suspense fallback, костомные skeleton-компоненты.

```tsx
function TableSkeleton({ rows = 5 }: { rows?: number }) {
  return (
    <table className="skeleton-table" aria-busy="true">
      <tbody>
        {Array.from({ length: rows }).map((_, index) => (
          <tr key={index}>
            <td>
              <div className="skeleton-line" />
            </td>
            <td>
              <div className="skeleton-line" />
            </td>
            <td>
              <div className="skeleton-line" />
            </td>
          </tr>
        ))}
      </tbody>
    </table>
  );
}
```

## Управление памятью и возобновляемость

- **Проблема**: большие данные держат память, при уходе пользователя надо освобождать ресурсы.
- **Решения**: используйте `AbortController`, `clearInterval`, `queryClient.removeQueries` для устаревших списков.
- **Offline-режим**: храните кэш в IndexedDB через `react-query-persistClient`, синхронизируйте при возврате онлайн.
