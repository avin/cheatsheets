# Базы данных и ORM

Express.js часто используется вместе с SQL/NoSQL базами, ORM и клиентами.

## Подключение и пул соединений

### Knex

```ts
import knex from 'knex';

const db = knex({
  client: 'pg',
  connection: process.env.DATABASE_URL,
  pool: { min: 2, max: 10 },
});

export default db;
```

### Prisma

```ts
import { PrismaClient } from '@prisma/client';

const prisma = new PrismaClient({
  log: ['query', 'warn', 'error'],
});

export default prisma;
```

## Репозитории и сервисы

- Создавайте слой `repository` для запросов (`user.repository.ts`).
- Определяйте сервисы, которые используют репозитории и преобразуют данные.

```ts
class UserRepository {
  constructor(private client: PrismaClient) {}

  findById(id: string) {
    return this.client.user.findUnique({ where: { id } });
  }

  create(data: CreateUserInput) {
    return this.client.user.create({ data });
  }
}
```

## Транзакции

```ts
await prisma.$transaction(async (tx) => {
  const user = await tx.user.create({ data: input });
  await tx.auditLog.create({
    data: { userId: user.id, action: 'USER_CREATED' },
  });
});
```

- В Knex используйте `db.transaction`.
- При retry учитывайте `Serializable`/`Repeatable Read`.

## Миграции

- Для Knex: `knex migrate:make`, `knex migrate:latest`.
- Для Prisma: `prisma migrate dev`, `prisma migrate deploy`.
- Храните миграции в репозитории, запускайте в CI/CD.

## DataLoader и кеширование

```ts
import DataLoader from 'dataloader';

const userLoader = new DataLoader(async (ids: readonly string[]) => {
  const users = await prisma.user.findMany({
    where: { id: { in: [...ids] } },
  });
  const map = new Map(users.map((user) => [user.id, user]));
  return ids.map((id) => map.get(id) ?? null);
});

app.use((req, _res, next) => {
  req.loaders = { userLoader };
  next();
});
```

- Инжектируйте `loaders` в request context, сбрасывайте на каждый запрос.
- Добавляйте layer Redis/in-memory кеша для часто используемых данных.

## Retry и ошибки

- Избегайте бесконечных retry; используйте backoff (например, `p-retry`).
- Оборачивайте специфичные ошибки (например, `UniqueConstraintFailed`) в доменные исключения.

```ts
try {
  await userRepository.create(data);
} catch (err) {
  if (isPrismaError(err) && err.code === 'P2002') {
    throw new ConflictError('Email already exists');
  }
  throw err;
}
```

## Мониторинг БД

- Снимайте метрики пула (`pool.numUsed`, `pool.numIdle`).
- Логируйте медленные запросы (> 200 ms).
- Используйте APM (NewRelic, Datadog, Otel) для отслеживания латентности.
