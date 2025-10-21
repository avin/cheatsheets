# Маршрутизация и layout

Паттерны организации навигации, layout-структуры и загрузки данных в современных React-приложениях.

## Layout-деревья и nested маршруты

- **Когда**: повторяющиеся части UI (хедер, сайдбар) и вложенные разделы.
- **Инструменты**: React Router `createBrowserRouter` с layout routes, Next.js App Router (`layout.tsx`, `page.tsx`).
- **Плюсы**: переиспользование общих компонентов, изоляция логики разделов, code splitting по маршрутам.

Пример React Router: layout отрисовывает общий каркас, а вложенные маршруты добавляют контент.

```tsx
const router = createBrowserRouter([
  {
    element: <DashboardLayout />,
    children: [
      { index: true, element: <Overview /> },
      { path: 'analytics', element: <Analytics /> },
      { path: 'settings/*', element: <SettingsRoutes /> },
    ],
  },
]);

function DashboardLayout() {
  return (
    <div className="dashboard">
      <Sidebar />
      <main>
        <Outlet />
      </main>
    </div>
  );
}
```

## Protected routes и контроль доступа

- **Паттерн**: маршруты, требующие аутентификации, оборачиваются в guard-компонент, который проверяет токен/роль.
- **Варианты**: redirect на `/login`, отображение skeleton/fallback, Route Objects с `loader`, который может бросить `redirect`.
- **Советы**: храните состояние авторизации в глобальном сторе, учитывайте серверный рендер (Next.js middleware).

Компонент `RequireAuth` сначала показывает спиннер при загрузке пользователя, затем либо пропускает дочерний контент, либо отправляет на страницу логина.

```tsx
function RequireAuth({ children }: { children: React.ReactNode }) {
  const user = useAuthUser();
  if (user.status === 'loading') {
    return <Spinner />;
  }
  if (!user.authenticated) {
    return <Navigate to="/login" replace />;
  }
  return <>{children}</>;
}
```

## Data API в маршрутах

- **React Router loaders/actions**: дают возможность загружать данные до рендера, поддерживают отложенную загрузку (`defer`).
- **Next.js App Router**: серверные компоненты и `generateMetadata`, `fetch` с кэшированием на уровне маршрута.
- **Принципы**: отдельный кэш на сегмент, управление ошибками через `errorElement`, `notFound`.

Объявление ниже создаёт файл-маршрут через `@tanstack/router`, где `loader` получает данные до рендера, а страница читает их через `useLoaderData`.

```tsx
export const route = createFileRoute('/projects/:id')({
  loader: async ({ params }) => {
    return fetchProject(params.id);
  },
  component: ProjectPage,
});

function ProjectPage() {
  const project = Route.useLoaderData();
  return <ProjectDetails project={project} />;
}
```

## Prefetch и плавные переходы

- **Задача**: загрузить данные/бандл до навигации, чтобы переход был мгновенным.
- **Инструменты**: React Router `useFetcher`, Next.js `Link` с `prefetch`, `router.prefetch` для программных переходов, Service Worker для кэширования.
- **Советы**: префетчите только «холодные» маршруты, отключайте на медленных соединениях (`navigator.connection`), очищайте кэш при обновлении версии.

`ProjectsLink` заранее загружает список проектов через `fetcher.load`, а при переходе использует уже готовые данные.

```tsx
function ProjectsLink() {
  const fetcher = useFetcher<ProjectList>();
  useEffect(() => {
    fetcher.load('/api/projects');
  }, [fetcher]);

  return (
    <NavLink to="/projects">
      Проекты {fetcher.data ? `(${fetcher.data.count})` : null}
    </NavLink>
  );
}
```

## SSR, SSG и гибриды

- **Next.js**: комбинация `generateStaticParams`, `revalidateTag`, `dynamic = 'force-dynamic'` для тонкой настройки.
- **Remix/React Router**: SSR с потоковым рендером, пересылка данных через `loader`.
- **Практика**: выбирайте стратегию по типу страниц — маркетинг (SSG+ISR), дашборды (SSR), интерактивные SPA (CSR).

Пример ниже показывает страницу Next.js App Router с ISR — посты пересобираются раз в минуту, а `notFound` выбрасывается при отсутствии данных.

```tsx
// Next.js App Router
export const revalidate = 60; // ISR раз в минуту

export default async function BlogPost({ params }: { params: { slug: string } }) {
  const post = await getPost(params.slug);
  if (!post) notFound();
  return <Article post={post} />;
}
```
