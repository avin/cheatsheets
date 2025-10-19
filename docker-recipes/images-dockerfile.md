# Образы и Dockerfile

## Создание образов

```bash
# Стандартная сборка
docker build -t registry.example.com/app/web:1.2.0 .

# BuildKit (по умолчанию в Docker 23+)
DOCKER_BUILDKIT=1 docker build --progress=plain -t app:dev .

# Мультиплатформенная сборка
docker buildx build --platform linux/amd64,linux/arm64 \
  -t registry.example.com/app/web:1.2.0 --push .
```

## Многоступенчатая сборка

```Dockerfile
FROM node:20-alpine AS build
WORKDIR /app
COPY package*.json ./
RUN npm ci --omit=dev
COPY . .
RUN npm run build

FROM nginx:1.25-alpine
COPY --from=build /app/dist /usr/share/nginx/html
COPY nginx.conf /etc/nginx/conf.d/default.conf
```

- Используйте `--target` для отладки промежуточных стадий.
- Для повторного применения результатов включайте BuildKit cache (`RUN --mount=type=cache`).

## Оптимизация размера

- Избегайте `latest`, фиксируйте версии `FROM`.
- Очищайте кеши:

```Dockerfile
RUN apt-get update && apt-get install -y curl \
 && rm -rf /var/lib/apt/lists/*
```

- Для Go/Rust-образов копируйте только статически собранный бинарник.

## Распространённые паттерны

- **ENTRYPOINT/CMD**:

```Dockerfile
ENTRYPOINT ["./entrypoint.sh"]
CMD ["nginx", "-g", "daemon off;"]
```

- **Аргументы build'а**:

```Dockerfile
ARG APP_VERSION
ENV VERSION=${APP_VERSION}
RUN echo "$VERSION" > /app/VERSION
```

```bash
docker build --build-arg APP_VERSION=1.4.3 -t app:1.4.3 .
```

- **Secret mounts (BuildKit)**:

```Dockerfile
# syntax=docker/dockerfile:1.7
RUN --mount=type=secret,id=npmrc target=/root/.npmrc \
    npm ci
```

## Работа с Registry

```bash
docker login registry.example.com
docker tag app:dev registry.example.com/app:dev
docker push registry.example.com/app:dev
docker pull registry.example.com/app:dev
```

- Чистка локального кеша:

```bash
docker image prune -a
```

- Просмотр слоёв:

```bash
docker history app:dev
```

## BuildKit полезности

- `--cache-from/--cache-to` для сохранения кэша между CI-джобами.
- `RUN --mount=type=cache,target=/root/.cache/pip` ускоряет Python-сборки.
- `COPY --link` (Dockerfile v1.4+) снижает использование inodes.
