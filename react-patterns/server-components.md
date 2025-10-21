# Server Components и Actions

Практики работы с React Server Components (RSC) и серверными действиями на базе Next.js App Router и других фреймворков.

## Клиентские vs серверные компоненты

- **Правило**: файлы без директивы `"use client"` исполняются на сервере, не попадают в бандл и могут обращаться к секретам.
- **Когда использовать серверные**: данные из БД, файловая система, конфигурация, тяжёлые вычисления.
- **Клиентские компоненты**: интерактивные элементы, хуки браузера, состояние UI.

Связка ниже показывает типичный сценарий: серверный `UsersPage` получает данные из БД и передаёт их клиентскому `UsersTable`, который уже отвечает за интерактивность.

```tsx
// app/users/page.tsx — серверный компонент
import { getUsers } from '@/db/users';
import UsersTable from './UsersTable';

export default async function UsersPage() {
  const users = await getUsers();
  return <UsersTable users={users} />;
}

// app/users/UsersTable.tsx — клиентский компонент
"use client";

export default function UsersTable({ users }: { users: User[] }) {
  const [sort, setSort] = useState<'asc' | 'desc'>('asc');
  const sorted = useMemo(() => sortUsers(users, sort), [users, sort]);
  return (
    <>
      <SortToggle value={sort} onChange={setSort} />
      <Table rows={sorted} />
    </>
  );
}
```

## Data fetching и кэширование

- **fetch в RSC**: автоматически кэшируется (HTTP cache), поддерживает `revalidate` и `cache: 'no-store'`.
- **Паттерн**: используйте `fetch` напрямую, передавайте данные в клиентские компоненты через props.
- **Инструменты**: `revalidateTag`, `revalidatePath`, `unstable_cache`.

В функции `getPost` мы указываем `next.tags` и `revalidate`, чтобы Next.js обслуживал кэшированные ответы и мог выборочно их сбрасывать.

```tsx
export const revalidate = 120;

async function getPost(slug: string) {
  const res = await fetch(`https://api.example.com/posts/${slug}`, {
    next: { tags: ['posts'], revalidate: 120 },
  });
  if (!res.ok) throw new Error('Failed');
  return res.json();
}
```

## Streaming и progressive rendering

- **Цель**: отправить пользователю HTML как можно раньше, догружая тяжёлые блоки позже.
- **Next.js**: `loading.tsx` для сегментов, `Suspense` внутри серверных компонентов, `ReactDOM.fetch`.
- **Лайфхаки**: разбивайте страницы на секции, используйте `Suspense` вокруг медленных запросов.

Страница `Dashboard` рендерит сразу «каркас», а график и ленту активности стримит позже, показывая skeleton до готовности данных.

```tsx
// app/dashboard/page.tsx
import { Suspense } from 'react';
import SalesChart from './SalesChart';
import ActivityFeed from './ActivityFeed';

export default function Dashboard() {
  return (
    <>
      <Suspense fallback={<Skeleton height={240} />}>
        <SalesChart />
      </Suspense>
      <Suspense fallback={<Skeleton height={320} />}>
        <ActivityFeed />
      </Suspense>
    </>
  );
}
```

## Server Actions и мутации

- **Паттерн**: объявление асинхронной функции с директивой `"use server"`, использование из клиентских компонентов через `form` или прямой вызов.
- **Преимущества**: нет необходимости поднимать API-роут, автоматическая сериализация форм, прямой доступ к БД.
- **Риски**: следите за CSRF (используйте `headers().get('cookie')`, `revalidatePath`), проверяйте права доступа.

Серверное действие `createTask` работает с БД и инвалидацией кэша, а клиентская форма отправляет его через встроенный механизм `action`.

```tsx
"use server";
import { revalidatePath } from 'next/cache';

export async function createTask(formData: FormData) {
  const title = formData.get('title');
  await db.task.create({ data: { title } });
  revalidatePath('/tasks');
}
```

```tsx
"use client";
import { createTask } from './actions';

export function TaskForm() {
  const [state, action] = useFormState(createTask, { status: 'idle' });
  return (
    <form action={action}>
      <input name="title" required />
      <button type="submit">Добавить</button>
      {state.status === 'success' && <p>Готово!</p>}
    </form>
  );
}
```

## Ограничения окружения

- **Нельзя в RSC**: использовать браузерные API (`window`, `document`), хуки состояния, `useEffect`.
- **Нельзя в client components**: импортировать серверные зависимости (fs, БД), получать секреты.
- **Стратегия**: изолируйте инфраструктурный код в серверных модулях, передавайте минимальные данные в клиент.

## Совместное использование кода

- **Shared utils**: перекладывайте чистые функции в `lib/` без подмножества окружения.
- **Бандлы**: клиентские компоненты должны импортировать только ESM без side-effects, серверные могут использовать CommonJS.
- **Валидация**: делитесь схемами (Zod) и типами (`*.d.ts`) между сервером и клиентом для согласованности.
