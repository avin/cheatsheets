# Маршруты и контроллеры

Основы построения REST API и контроллеров в Express.js.

## Базовая структура

```ts
import express from 'express';
import type { Request, Response, NextFunction } from 'express';

const app = express();
app.use(express.json());

app.get('/status', (_req: Request, res: Response) => {
  res.json({ ok: true, timestamp: Date.now() });
});

app.listen(3000, () => {
  console.log('Server listening on port 3000');
});
```

## CRUD-маршруты

```ts
const router = express.Router();

router.get('/users', listUsers);
router.post('/users', createUser);
router.get('/users/:id', getUserById);
router.put('/users/:id', updateUser);
router.delete('/users/:id', deleteUser);

async function listUsers(_req: Request, res: Response) {
  const users = await userService.findAll();
  res.json(users);
}
```

- Используйте `Router` для логической группировки (например, `/users`, `/teams`).
- Структурируйте контроллеры в отдельные файлы (`users.controller.ts`).

## Параметризация и вложенные маршруты

```ts
router.get('/teams/:teamId/members/:memberId', (req, res) => {
  const { teamId, memberId } = req.params;
  res.json({ teamId, memberId });
});

router.param('teamId', async (req, _res, next, value) => {
  const team = await teamService.findById(value);
  if (!team) return next(new NotFoundError('Team not found'));
  req.team = team;
  next();
});
```

## Валидация входных данных

### Zod

```ts
import { z } from 'zod';

const createUserSchema = z.object({
  email: z.string().email(),
  name: z.string().min(1),
  role: z.enum(['user', 'admin']).default('user'),
});

const validate =
  <T extends z.ZodTypeAny>(schema: T) =>
  (req: Request, _res: Response, next: NextFunction) => {
    const result = schema.safeParse(req.body);
    if (!result.success) return next(new BadRequestError(result.error.message));
    req.body = result.data;
    next();
  };

router.post('/users', validate(createUserSchema), async (req, res) => {
  const user = await userService.create(req.body);
  res.status(201).json(user);
});
```

### Joi/Yup (альтернативы)

- Используйте middleware вроде `celebrate` (для Joi) или `useValidation` (для Yup).
- Отделяйте схемы в отдельный модуль (`user.schema.ts`).

## Формирование ответов

- Возвращайте `status` явным образом (`res.status(201).json({...})`).
- Вводите общий формат (`{ data, meta, errors }`).
- Для длинных операций используйте `202 Accepted` + endpoint статуса.

## Обработка ошибок

```ts
class HttpError extends Error {
  constructor(public status: number, message: string, public details?: unknown) {
    super(message);
  }
}

const errorHandler: express.ErrorRequestHandler = (err, _req, res, _next) => {
  if (err instanceof HttpError) {
    return res.status(err.status).json({ message: err.message, details: err.details });
  }
  console.error(err);
  res.status(500).json({ message: 'Internal Server Error' });
};

app.use(errorHandler);
```

- Логируйте ошибки централизованно, добавляйте correlation-id.
- В продакшене скрывайте внутренние сообщения, предоставляйте коды (`ERR_USER_NOT_FOUND`).

## Популярные паттерны

- Контроллер возвращает `Promise`, ошибки обрабатываются в catch-wrapper (`wrapAsync(controller)`).
- DTO/mapper слой переводит модели БД в публичные, чтобы не раскрывать лишнее.
- Внедряйте `request-scoped` контейнер для сервисов (на базе Awilix/Inversify).
