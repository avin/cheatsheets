# Резолверы и инфраструктура

Резолверы объединяют схему и реальные данные. Следите за контекстом, авторизацией и производительностью.

## Контекст и жизненный цикл

- Контекст GraphQL передаётся в каждый резолвер; используйте его для внедрения сервисов (ORM, loaders, пользователя).
- Не храните в контексте мутируемые объекты без снапшота — каждый запрос должен быть изолирован.
- Добавляйте correlation-id и request-id для трассировки.

```ts
import { GraphQLResolveInfo } from 'graphql';

type Context = {
  user?: AuthUser;
  loaders: Loaders;
  logger: Logger;
};

const resolvers = {
  Query: {
    me: (_parent: unknown, _args: unknown, ctx: Context) => ctx.user,
  },
  User: {
    teams: (user, _args, ctx: Context, info: GraphQLResolveInfo) =>
      ctx.loaders.teamByUser.load(user.id, info),
  },
};
```

## Авторизация и RBAC

- Проводите авторизацию на уровне резолверов или через директивы (`@auth`).
- Избегайте утечки данных в ошибках — добавляйте код и сообщение без подробностей.
- Используйте централизованный guard, чтобы не дублировать проверки.

```ts
const authDirective = createDirective((role: Role) => ({
  async beforeResolve({ context, info }) {
    if (!context.user || !context.user.roles.includes(role)) {
      throw new GraphQLForbiddenError({
        message: `Access denied for ${info.fieldName}`,
        extensions: { code: 'FORBIDDEN' },
      });
    }
  },
}));
```

## DataLoader и N+1

- Используйте `dataloader` (JS/TS) или аналогичные библиотеки (graphql-batch для Ruby, dataloader-java).
- Ключ — прозрачный batching на уровне GraphQL-резолверов.
- Пропагандируйте `GraphQLResolveInfo` в loader, чтобы ограничивать выбираемые поля.

## Ошибки и форматирование

- На сервере используйте `formatError` для нормализации ошибок.
- Храните `extensions.code` (`BAD_USER_INPUT`, `UNAUTHENTICATED`, `INTERNAL_SERVER_ERROR`).
- Для нестандартных ошибок создайте собственные классы, наследующие `GraphQLError`.

## Observability

- Включайте Apollo Tracing или OpenTelemetry-интеграции (Apollo Gateway, Helix, Yoga).
- Логируйте длительность резолверов, количество обращений к DataLoader, размер ответа.
- Настройте дедупликацию логов для батчей.

## Federation и композиция

- При федерации экспортируйте типы через `@key`, используйте `@provides` и `@requires`.
- Планируйте границы ответственности сервисов: избегайте «god-schema».
- При композиции следите за версионностью: `@tag`, changelog схемы, автоматические ревью через Rover/GraphQL Inspector.
