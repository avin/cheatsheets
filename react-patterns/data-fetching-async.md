# Загрузка данных и конкурентность

Паттерны асинхронной работы, синхронизации и конкурентного рендеринга в React.

## useEffect + AbortController

- **Базовый случай**: загрузка данных внутри компонента с отменой запроса при размонтировании.
- **Лайфхаки**: храните флаг отмены, сбрасывайте состояние при смене входных параметров, используйте `AbortSignal.timeout()` в современных браузерах.

```tsx
function useUser(userId: string) {
  const [data, setData] = useState<User | null>(null);
  const [status, setStatus] = useState<'idle' | 'loading' | 'error' | 'success'>('idle');

  useEffect(() => {
    if (!userId) return;
    const controller = new AbortController();
    setStatus('loading');

    fetch(`/api/users/${userId}`, { signal: controller.signal })
      .then((res) => {
        if (!res.ok) throw new Error(`HTTP ${res.status}`);
        return res.json();
      })
      .then((json) => {
        setData(json);
        setStatus('success');
      })
      .catch((error) => {
        if (error.name !== 'AbortError') {
          setStatus('error');
        }
      });

    return () => controller.abort();
  }, [userId]);

  return { data, status };
}
```

## React Query / SWR

- **Задача**: кеширование, повторные попытки, дедупликация запросов и синхронизация фона без ручного кода.
- **Паттерн**: конфигурируйте глобальный QueryClient, пиши `queryKey` и `queryFn`, используйте селекторы данных.
- **Особенности**: указывайте `staleTime`, `cacheTime`, настраивайте `retry` с `retryDelay`, добавляйте devtools.

```tsx
const queryClient = new QueryClient({
  defaultOptions: {
    queries: {
      retry: 2,
      staleTime: 5 * 60_000,
      refetchOnWindowFocus: false,
    },
  },
});

function UserView({ userId }: { userId: string }) {
  const { data, isLoading, isError, refetch } = useQuery({
    queryKey: ['user', userId],
    queryFn: () => fetch(`/api/users/${userId}`).then((res) => res.json()),
    enabled: Boolean(userId),
  });

  if (isLoading) return <Spinner />;
  if (isError) return <ErrorFallback onRetry={() => refetch()} />;
  return <UserCard user={data} />;
}
```

## Suspense и `use` в серверных компонентах

- **Concurrent Rendering**: Suspense позволяет показывать fallback до готовности данных, работает с `React.lazy`, серверными компонентами и библиотеками, поддерживающими `suspense`.
- **Паттерн**: оборачивайте загрузку в ресурс (`wrapPromise`) или используйте библиотеки (`react-query` с experimental `suspense`).
- **React 18 RSC**: на сервере можно приостановить рендер, вызвав `await` или `use(fetchPromise)`, клиент получит стрим.

```tsx
// Серверный компонент (Next.js App Router)
import { getUser } from '@/lib/data';

export default async function UserPage({ params }: { params: { id: string } }) {
  const user = await getUser(params.id); // Suspense автоматически обработает обещание
  return <UserProfile user={user} />;
}
```

```tsx
function SuspenseExample({ userId }: { userId: string }) {
  return (
    <Suspense fallback={<Skeleton />}>
      <UserDetails userId={userId} />
    </Suspense>
  );
}
```

## Стриминг и инкрементальное обновление

- **Server Components + Streaming**: используйте `renderToPipeableStream` (Node) или `renderToReadableStream` (Edge) для постепенного вывода HTML.
- **Progressive hydration**: разделяйте крупные страницы на «острова», используйте `React.lazy` для позднего подключения интерактивных частей.
- **Работа с формами**: в Next.js можно применять `server actions` для оптимистичных обновлений и повторного `revalidatePath`.

```tsx
// Node сервер
const stream = renderToPipeableStream(<App />, {
  bootstrapScripts: ['/static/main.js'],
  onShellReady() {
    stream.pipe(res);
  },
});
```

## Консистентность и синхронизация

- **Проблема**: данные меняются на сервере, UI должен оставаться согласованным.
- **Решения**: подписки (WebSocket, SSE), оптимистические апдейты с отменой при ошибке, `queryClient.invalidateQueries` после мутаций.
- **Практика**: используйте версионирование (`etag`), `lastModified`, `cursor` для пагинации; синхронизируйте optimistic state с серверным ответом.
