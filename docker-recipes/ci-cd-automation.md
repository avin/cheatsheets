# CI/CD и автоматизация

## BuildKit и кэширование

- Включите BuildKit в CI:

```bash
export DOCKER_BUILDKIT=1
docker build --progress=plain -t app:${GIT_SHA} .
```

- Используйте inline cache:

```bash
docker build \
  --cache-from=type=registry,ref=registry.example.com/app/build-cache:latest \
  --cache-to=type=registry,ref=registry.example.com/app/build-cache:latest,mode=max \
  -t registry.example.com/app/web:${GIT_SHA} .
```

## GitHub Actions

```yaml
name: Build and Push

on:
  push:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: docker/setup-buildx-action@v3
      - uses: docker/login-action@v3
        with:
          registry: ${{ secrets.REGISTRY_URL }}
          username: ${{ secrets.REGISTRY_USER }}
          password: ${{ secrets.REGISTRY_PASS }}
      - uses: docker/build-push-action@v5
        with:
          context: .
          push: true
          tags: |
            registry.example.com/app/web:${{ github.sha }}
            registry.example.com/app/web:latest
          cache-from: type=registry,ref=registry.example.com/app/build-cache:latest
          cache-to: type=registry,ref=registry.example.com/app/build-cache:latest,mode=max
```

## GitLab CI

```yaml
stages:
  - build

variables:
  DOCKER_DRIVER: overlay2

build-image:
  stage: build
  image: docker:24
  services:
    - docker:24-dind
  script:
    - docker login -u "$CI_REGISTRY_USER" -p "$CI_REGISTRY_PASSWORD" "$CI_REGISTRY"
    - docker build -t "$CI_REGISTRY_IMAGE:$CI_COMMIT_SHORT_SHA" .
    - docker push "$CI_REGISTRY_IMAGE:$CI_COMMIT_SHORT_SHA"
```

- Для BuildKit используйте `DOCKER_BUILDKIT=1` и `docker buildx bake`.

## Тегирование и версии

- Используйте семантические теги:
  - `1.4.3`, `1.4`, `1`, `latest`
- Добавляйте метаданные в LABEL:

```Dockerfile
LABEL org.opencontainers.image.source="https://github.com/org/repo" \
      org.opencontainers.image.revision="${GIT_SHA}"
```

## Автоматическое обновление

- Watchtower/Diun для обновления контейнеров.
- Renovate/Dependabot для обновления базовых образов (`docker-compose` manifests).

## Lifecycle hooks

- `docker system prune` после сборок (на CI).
- `docker compose pull && docker compose up -d` в deploy скриптах.

## Multi-platform артефакты

- `docker buildx create --use`
- `docker buildx imagetools inspect repo/image:tag` для проверки.

## Верификация

- Прогон тестов внутри образа:

```bash
docker run --rm app:${GIT_SHA} pytest
```

- Сканирование уязвимостей как часть пайплайна (Trivy, Grype, Scout).
