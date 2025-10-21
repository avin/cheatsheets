# Тестирование компонентов

Подходы к тестированию React-приложений: от юнитов до визуальных и e2e тестов.

## React Testing Library

- **Принципы**: тесты через пользовательские сценарии (`screen.getByRole`), минимум моков, реальный DOM.
- **Практика**: управляйте временем через `vi.useFakeTimers`, оборачивайте в нужные провайдеры (`renderWithProviders`).

```tsx
import { render, screen, fireEvent } from '@testing-library/react';
import { QueryClientProvider } from '@tanstack/react-query';
import { queryClient } from '@/test-utils/queryClient';

test('отправка формы вызывает API', async () => {
  render(
    <QueryClientProvider client={queryClient}>
      <SignupForm />
    </QueryClientProvider>,
  );

  await user.type(screen.getByLabelText(/email/i), 'user@example.com');
  await user.type(screen.getByLabelText(/пароль/i), '12345678');
  await user.click(screen.getByRole('button', { name: /создать/i }));

  expect(await screen.findByText(/готово/i)).toBeInTheDocument();
});
```

## Тестирование хуков

- **Инструменты**: `@testing-library/react-hooks` (deprecated) → `@testing-library/react` + `renderHook`.
- **Паттерн**: окружайте хук нужными провайдерами, используйте `act` для async.

```tsx
import { renderHook, act } from '@testing-library/react';

test('useModal переключает состояние', () => {
  const { result } = renderHook(() => useModal());
  expect(result.current.open).toBe(false);
  act(() => result.current.openModal());
  expect(result.current.open).toBe(true);
});
```

## Mock сервисов и сетевых запросов

- **MSW (Mock Service Worker)**: перехватывает `fetch`/XHR, позволяет описывать сценарии (`rest.get`, `graphql.query`).
- **Советы**: для юнитов можно мокать `fetch` через `vi.fn()`, но для интеграции используйте MSW для реалистичности.
- **Практика**: конфиг `setupTests.ts` поднимает MSW перед тестами, очищает хэндлеры.

```tsx
import { rest } from 'msw';
import { setupServer } from 'msw/node';

const server = setupServer(
  rest.get('/api/user', (_req, res, ctx) => res(ctx.json({ name: 'Alice' }))),
);

beforeAll(() => server.listen());
afterEach(() => server.resetHandlers());
afterAll(() => server.close());
```

## Storybook и визуальные регрессии

- **Storybook**: храните истории как документацию и тесты состоятний компонента.
- **Визуальные тесты**: Chromatic, Percy, Loki сравнивают скриншоты.
- **Советы**: используйте `play` функции в Storybook 7+ для интеграционных сценариев, накладывайте accessibility чек.

```ts
// Button.stories.tsx
export const Primary: Story = {
  args: { variant: 'primary', children: 'Сохранить' },
  play: async ({ canvasElement }) => {
    const canvas = within(canvasElement);
    await userEvent.click(canvas.getByRole('button'));
  },
};
```

## E2E тесты

- **Инструменты**: Playwright, Cypress, WebdriverIO для браузерных сценариев.
- **Паттерн**: запускайте приложение в prod-сборке, используйте data-testid только при необходимости, синхронизируйтесь с бэкендом через фикстуры.
- **Советы**: создавайте «контейнеры» данных (seed), очищайте после тестов, используйте `test.use({ storageState })` для авторизации.

```ts
// e2e/profile.spec.ts
test('пользователь обновляет профиль', async ({ page }) => {
  await page.goto('/profile');
  await page.getByLabel('Имя').fill('Alice');
  await page.getByRole('button', { name: 'Сохранить' }).click();
  await expect(page.getByText('Профиль обновлён')).toBeVisible();
});
```

## Контрактные тесты и API

- **Pact/Schema tests**: проверяют, что UI и бэкенд согласованы по контракту.
- **GraphQL**: генерируйте типы (GraphQL Codegen), валидируйте схемы в CI.
- **REST**: храните OpenAPI спецификации, валидируйте ответы на основе схем.

## Производительность и метрики тестов

- **Линтеры**: ESLint с плагином `testing-library`, чтобы избегать анти-паттернов (например, `getByTestId` вместо ролей).
- **Coverage**: комбинируйте `vitest`/`jest` coverage с e2e, устанавливайте пороги.
- **CI**: прогоняйте быстрые тесты на каждый PR, медленные (e2e, visual) — на nightly или по тегам.
