# Middleware и безопасность

Настройка цепочек middleware, защита от типичных атак, безопасность приложений.

## Логирование и trace-id

```ts
import morgan from 'morgan';
import { v4 as uuid } from 'uuid';

app.use((req, _res, next) => {
  req.id = uuid();
  next();
});

app.use(
  morgan(':method :url :status :response-time ms', {
    stream: {
      write: (msg) => logger.info({ requestId: req.id, msg }),
    },
  }),
);
```

- Используйте `pino-http` или `winston` для структурированных логов.
- Пробрасывайте `requestId` в контекст, чтобы связывать с бэкендом.

## Парсеры и лимиты

```ts
app.use(express.json({ limit: '1mb' }));
app.use(express.urlencoded({ extended: true }));
app.use(cookieParser());
```

- Ограничивайте размер тела (`limit`), чтобы избежать DoS.
- Включайте `app.disable('x-powered-by')` для скрытия информации о сервере.

## CORS и rate limiting

```ts
import cors from 'cors';
import rateLimit from 'express-rate-limit';

app.use(
  cors({
    origin: ['https://app.example.com'],
    credentials: true,
    allowedHeaders: ['Content-Type', 'Authorization'],
  }),
);

app.use(
  rateLimit({
    windowMs: 15 * 60 * 1000,
    max: 200,
    keyGenerator: (req) => req.ip,
    handler: (_req, res) => res.status(429).json({ message: 'Too many requests' }),
  }),
);
```

## Helmet и CSP

```ts
import helmet from 'helmet';

app.use(
  helmet({
    contentSecurityPolicy: {
      directives: {
        defaultSrc: ["'self'"],
        scriptSrc: ["'self'", 'https://cdn.example.com'],
        imgSrc: ["'self'", 'data:', 'https://images.example.com'],
      },
    },
    crossOriginEmbedderPolicy: false,
  }),
);
```

- Включайте `helmet.expectCt`, `helmet.referrerPolicy`, `helmet.hsts`.
- CSP предупреждения отслеживайте через report-uri.

## Защита от CSRF/XSS

```ts
import csrf from 'csurf';
import xssClean from 'xss-clean';

app.use(xssClean());
app.use(csrf({ cookie: true }));

app.use((req, res, next) => {
  res.cookie('XSRF-TOKEN', req.csrfToken(), { httpOnly: false });
  next();
});
```

- Для SPA на токенах часто лучше double-submit cookies или SameSite=strict.
- Для других защищайте формы server-rendered шаблонами.

## Авторизация и роли

```ts
const requireAuth =
  (...roles: string[]) =>
  (req: Request, _res: Response, next: NextFunction) => {
    const user = req.user;
    if (!user) return next(new UnauthorizedError());
    if (roles.length && !roles.includes(user.role)) {
      return next(new ForbiddenError());
    }
    next();
  };

router.get('/admin/metrics', requireAuth('admin'), metricsHandler);
```

- Используйте `passport`/`express-session` или JWT/opaque tokens.
- Храните роли/permissions в базе, кешируйте в Redis.

## HTTP headers и best practices

- `Strict-Transport-Security`, `X-Content-Type-Options`, `X-Frame-Options`.
- Включайте `compression` с осторожностью (CRIME/BREACH), ограничивайте размеры.
- Обновляйте зависимости, запускайте `npm audit`/`yarn audit`.
