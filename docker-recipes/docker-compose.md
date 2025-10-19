# Docker Compose

## Структура файла

```yaml
version: "3.9"

services:
  web:
    image: registry.example.com/app/web:1.5.0
    build:
      context: .
      dockerfile: Dockerfile
      target: runtime
    env_file:
      - .env
    environment:
      NODE_ENV: production
    ports:
      - "8080:80"
    depends_on:
      - redis
      - db
    healthcheck:
      test: ["CMD", "curl", "-f", "http://localhost/healthz"]
      interval: 30s
      timeout: 5s
      retries: 3

  redis:
    image: redis:7-alpine
    volumes:
      - redis-data:/data

  db:
    image: postgres:16
    restart: unless-stopped
    volumes:
      - pg-data:/var/lib/postgresql/data
    environment:
      POSTGRES_PASSWORD: example

volumes:
  redis-data:
  pg-data:

networks:
  default:
    driver: bridge
```

## Команды CLI

```bash
docker compose up -d
docker compose down --volumes --remove-orphans
docker compose ps
docker compose logs -f web
docker compose exec web sh
```

- `--profile` включает определённые секции: `docker compose --profile debug up`.
- `docker compose cp service:/path ./local` для копирования.

## Override файлы

- По умолчанию читаются `compose.yaml` и `compose.override.yaml`.
- Создание override для dev:

```yaml
# compose.dev.yaml
services:
  web:
    build:
      args:
        ENABLE_DEBUG: "true"
    environment:
      NODE_ENV: development
    volumes:
      - .:/app
    command: npm run dev
```

Запуск:

```bash
docker compose -f compose.yaml -f compose.dev.yaml up
```

## Переменные и расширения

```yaml
x-common-env: &common-env
  NODE_ENV: production
  TZ: UTC

services:
  worker:
    image: app/worker:latest
    environment:
      <<: *common-env
      QUEUE: emails
```

- Поддерживаются `.env` файлы. Значения в `docker compose config` показывают итоговую конфигурацию.

## Секция deploy (Swarm)

```yaml
services:
  web:
    image: app/web:latest
    deploy:
      replicas: 3
      update_config:
        parallelism: 1
        delay: 10s
      restart_policy:
        condition: on-failure
      resources:
        limits:
          cpus: "1.5"
          memory: 1024M
```

- Работает только в Swarm (`docker stack deploy`).

## Темплейты и генерация

- Используйте `yq`, `helm template`, `kompose convert` для генерации.
- Для больших проектов рассмотрите `docker compose config > combined.yml` в CI.
