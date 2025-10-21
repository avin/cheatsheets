# Сайд-эффекты и синхронизация

Паттерны работы с побочными эффектами, внешними источниками данных и согласованностью состояния.

## Эффекты как транзакции

- **Принцип**: каждый `useEffect` должен быть независимой транзакцией — описывать причину (dependencies) и откат (`cleanup`).
- **Советы**: группируйте связанные эффекты, используйте `abortController` для async, избегайте «эффектов по совпадению» без зависимостей.

Пример ниже подписывается на событие аналитики и гарантированно отменяет его при размонтировании или смене `location.pathname`.

```tsx
useEffect(() => {
  const controller = new AbortController();
  analytics.track('page:view', { path: location.pathname }, controller.signal);
  return () => controller.abort();
}, [location.pathname]);
```

## Таймеры и планирование

- **Паттерн**: храните ID таймера в `useRef`, очищайте в cleanup, используйте `useEffectEvent` (React 18+) или кастомный хук для актуальных ссылок.
- **Инструменты**: `setTimeout`, `requestAnimationFrame`, `scheduler` API.

Кастомный хук `useDebouncedCallback` сохраняет ссылку на функцию и очищает таймер, обеспечивая корректную работу даже при часто меняющихся пропсах.

```tsx
function useDebouncedCallback<T extends (...args: any[]) => void>(callback: T, delay: number) {
  const callbackRef = useRef(callback);
  const timerRef = useRef<number | null>(null);

  useEffect(() => {
    callbackRef.current = callback;
  }, [callback]);

  const debounced = useCallback((...args: Parameters<T>) => {
    if (timerRef.current !== null) {
      window.clearTimeout(timerRef.current);
    }
    timerRef.current = window.setTimeout(() => {
      callbackRef.current(...args);
    }, delay);
  }, [delay]);

  useEffect(() => () => {
    if (timerRef.current !== null) {
      window.clearTimeout(timerRef.current);
    }
  }, []);

  return debounced;
}
```

## Подписки: WebSocket, SSE, BroadcastChannel

- **Когда**: необходим real-time, синхронизация вкладок, push-обновления.
- **Паттерн**: выносим подключение в `useEffect`, используем `useSyncExternalStore` или глобальный стор для thread-safe состояния, учитываем reconnect/backoff.
- **Советы**: типизируйте события, используйте Ping/Pong, следите за очисткой соединения.

Хук `useChatSocket` открывает WebSocket на комнату, добавляет новые сообщения в state и закрывает соединение, когда компонент размонтируется.

```tsx
function useChatSocket(roomId: string) {
  const [messages, setMessages] = useState<Message[]>([]);

  useEffect(() => {
    const socket = new WebSocket(`wss://chat.example.com/rooms/${roomId}`);
    socket.addEventListener('message', (event) => {
      const payload = JSON.parse(event.data) as Message;
      setMessages((prev) => [...prev, payload]);
    });

    return () => {
      socket.close();
    };
  }, [roomId]);

  return messages;
}
```

## Finite-state машины и сложные флоу

- **Задача**: управлять UI, зависящим от множества состояний/переходов (мастеры, процессинговые шаги).
- **Инструменты**: XState (`createMachine`), Stately, Redux Toolkit `createSlice` + enum-like состояния.
- **Плюсы**: явные переходы, математика конечных автоматов, визуализация.

Машина `paymentMachine` описывает процесс оплаты: переход из `idle` в `processing`, ожидание ответа и обработку успеха/ошибки с возможностью ретрая.

```tsx
const paymentMachine = createMachine({
  id: 'payment',
  initial: 'idle',
  states: {
    idle: {
      on: { SUBMIT: 'processing' },
    },
    processing: {
      invoke: {
        src: 'processPayment',
        onDone: 'success',
        onError: 'failure',
      },
    },
    success: {},
    failure: {
      on: { RETRY: 'processing' },
    },
  },
});
```

## `useSyncExternalStore` и внешние источники

- **Когда**: состояние хранится вне React (WebSocket store, Redux без `Provider`, EventEmitter).
- **Паттерн**: реализовать store с `getState` и `subscribe`, в компоненте использовать `useSyncExternalStore` для получения актуальных данных.
- **Плюсы**: совместимо с concurrent rendering, не требует `useEffect`.

Шаблон `useExternalState` подписывается на внешнее хранилище и выбирает нужный срез через селектор, не заставляя компонент следить за остальным state.

```tsx
const store = createExternalStore(initialState);

export function useExternalState<T>(selector: (state: State) => T): T {
  return useSyncExternalStore(store.subscribe, () => selector(store.getState()));
}
```

## Event sourcing и оптимистичные апдейты

- **Идея**: записываем цепочку событий (create/update/delete), восстанавливаем состояние, применяем оптимистичные обновления.
- **Паттерн**: `useReducer` с массивом событий, или интеграция с бэкендом, который возвращает stream обновлений.
- **Риски**: конфликты между клиентами; используйте версии (`version`, `etag`) и механизмы разрешения (merge, last-write-wins).

Редьюсер обрабатывает события добавления и переключения дела, позволяя хранить историю действий и откатываться при необходимости.

```tsx
type Event = { type: 'add'; payload: Todo } | { type: 'toggle'; payload: string };

function reducer(state: Todo[], event: Event) {
  switch (event.type) {
    case 'add':
      return [...state, event.payload];
    case 'toggle':
      return state.map((todo) =>
        todo.id === event.payload ? { ...todo, completed: !todo.completed } : todo,
      );
    default:
      return state;
  }
}
```

## Согласованность и синхронизация вкладок

- **Инструменты**: `BroadcastChannel`, `storage` events, `navigator.locks`.
- **Паттерн**: слушайте события обновления, обновляйте локальный кэш, показывайте пользователю индикаторы «несохранённых изменений».
- **Советы**: debounce синхронизацию, при offline-режиме сохраняйте очередь изменений (IndexedDB).
- **Риски**: `BroadcastChannel` не поддержан в некоторых мобильных браузерах и Safari < 15.4, а `navigator.locks` доступен только в Chromium; добавляйте feature detection и fallbacks.

Хук `useTabSync` рассылает события через канал и при получении `invalidate` очищает кэш React Query во всех вкладках.

```tsx
function useTabSync(channelName: string) {
  useEffect(() => {
    const channel = new BroadcastChannel(channelName);
    channel.postMessage({ type: 'joined', timestamp: Date.now() });
    channel.onmessage = (event) => {
      if (event.data.type === 'invalidate') {
        queryClient.invalidateQueries({ queryKey: event.data.key });
      }
    };
    return () => channel.close();
  }, [channelName]);
}
```
