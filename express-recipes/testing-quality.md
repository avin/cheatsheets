# Тестирование и качество

Инструменты и подходы к проверке Express-приложений.

## Unit и интеграционные тесты

```ts
import request from 'supertest';
import { buildApp } from '../app';

describe('Users API', () => {
  const app = buildApp();

  it('creates a user', async () => {
    const response = await request(app)
      .post('/users')
      .send({ email: 'test@example.com', name: 'Test' })
      .expect(201);

    expect(response.body.email).toBe('test@example.com');
  });
});
```

- Используйте `buildApp()` без запуска `app.listen`.
- Мокайте внешние сервисы (`nock`, `msw/node`, `jest.mock`).
- Для e2e запускайте тестовую БД (docker-compose, testcontainers).

## Наборы инструментов

- **Jest**: авто-мок, watch-mode, coverage. Настройте `setupFilesAfterEnv`, глобальные переменные (URL API).
- **Vitest**: быстрый runner + TS ESM. Совместим с `supertest`.
- **Mocha + Chai**: гибкая конфигурация, подходит для микросервисов.

## Линтеры и форматтеры

- `eslint-config-airbnb` / `eslint-config-xo` + плагины для Node/TS.
- `prettier` или `biome` для единого стиля.
- Проверки в CI: `npm run lint`, `npm run test`, `npm run typecheck`.

## TypeScript и типизация

- `tsconfig.json`: `strict`, `noImplicitAny`, `moduleResolution: nodeNext`.
- Используйте `ts-node-dev`/`tsx` для локальной разработки.
- Генерируйте типы API (OpenAPI → TS через `openapi-typescript`).

## Coverage

- `jest --coverage`, `c8` (для V8 coverage), `nyc`.
- Следите за ключевыми путями: контроллеры, сервисы, middleware.
- Добавляйте пороги (threshold) в CI (`branches >= 80%`, `statements >= 90%`).

## Проверка маршрутов

- Создавайте файлы `*.spec.ts` рядом с контроллерами.
- Тестируйте ошибки (`expect(422)`, `expect(response.body.message)`).
- Имитация авторизации: создавайте тестового пользователя, генерируйте JWT.

## Contract тесты

- Выравнивайте фронт и бэк через OpenAPI + тесты (`pact`, `dredd`).
- Проверяйте, что backend соответствует ожидаемой схеме.
- Дополняйте e2e сценариями (Postman/Newman, k6).

## CI/CD

- Выносите команды в `package.json` (`test:unit`, `test:e2e`, `lint`, `typecheck`).
- Запускайте сборку с `--runInBand` для последовательности, если тесты используют общие ресурсы.
- Храните артефакты (coverage, junit) для отображения в CI.
