# Деплой и эксплуатация

Практики эксплуатации Express-приложений в продакшене.

## PM2 / Cluster

- Используйте `pm2 start dist/server.js -i max` для запуска нескольких воркеров.
- Ведите лог-файлы (`pm2 logs`, `pm2 flush`).
- Следите за реакцией на `SIGINT`/`SIGTERM` — graceful shutdown.

## Docker

```dockerfile
FROM node:20-alpine AS build
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY . .
RUN npm run build

FROM node:20-alpine
WORKDIR /app
ENV NODE_ENV=production
COPY --from=build /app/dist ./dist
COPY package*.json ./
RUN npm ci --omit=dev
CMD ["node", "dist/server.js"]
```

- Используйте `HEALTHCHECK` для проверки endpoint `/healthz`.
- Применяйте multi-stage build, чтобы уменьшить размер image.

## systemd

```ini
[Unit]
Description=Express API
After=network.target

[Service]
Environment=NODE_ENV=production
ExecStart=/usr/bin/node /var/www/app/dist/server.js
WorkingDirectory=/var/www/app
Restart=always
User=node
Group=node

[Install]
WantedBy=multi-user.target
```

- Логи отправляйте в journald или файл (`StandardOutput=journal`).
- Следите за перезапусками (`systemctl status express.service`).

## Конфигурация

- `dotenv` для локальной разработки, в проде — переменные окружения.
- Храните секреты в Vault/Sealed Secrets/SSM.
- Разделяйте `config.ts` по окружениям (dev/staging/prod).

## Feature Flags

- Используйте LaunchDarkly, Unleash, FlagSmith.
- Подключайте SDK в Express, кешируйте флаги для запросов.

## Мониторинг и логирование

- Метрики: Prometheus (`prom-client`), Datadog, New Relic.
- Логирование: Pino + транспорт в Elastic/Kibana, Loki.
- Alerting: SLO/SLA на время ответа, количество 5xx.

## Миграции и задачи

- Настраивайте pre-deploy hooks: `npm run migrate`.
- Если миграции долгие, применяйте zero-downtime (copy table, swap).
- Планируйте rollbacks (меньший schema change, feature flag).
