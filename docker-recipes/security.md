# Безопасность и управление доступом

## Пользователи и права

- Запускайте процессы от непривилегированного пользователя:

```Dockerfile
RUN addgroup --system app && adduser --system --ingroup app app
USER app
```

- Контейнеры по умолчанию запускаются с UID 0 (root). Используйте `--user`:

```bash
docker run -d --user 1000:1000 myorg/app:latest
```

- Управление group/UID mapping (`--userns-remap`).

## Capabilities и seccomp

- Список capabilities: `docker run --rm alpine capsh --print`.
- Ограничить привилегии:

```bash
docker run --cap-drop=ALL --cap-add=NET_BIND_SERVICE myorg/web
```

- Пользовательский seccomp-профиль:

```bash
docker run --security-opt seccomp=seccomp-profile.json ...
```

## AppArmor/SELinux

- Для AppArmor: `--security-opt apparmor=unconfined` (временно отключить).
- SELinux контексты: `:Z` для монтирования директорий.

## Чтение/запись и секреты

- Делайте volumes `readonly` где возможно (`-v data:/data:ro`).
- Не храните секреты в образах; используйте:
  - `docker secret` (Swarm)
  - внешние менеджеры (Vault, SSM, Doppler)
  - BuildKit secrets (`RUN --mount=type=secret`)

## Сканирование уязвимостей

- `docker scout cves myorg/app:latest`
- Trivy, Grype (интегрируются в CI/CD).
- При использовании собственных Registry — включайте автоматические сканеры (Harbor, GitLab Container Registry).

## Подпись образов

### Docker Content Trust / Notary v1

```bash
export DOCKER_CONTENT_TRUST=1
docker trust sign registry.example.com/app/web:1.5.0
docker trust inspect --pretty registry.example.com/app/web
```

### Cosign (Sigstore)

```bash
cosign sign --key cosign.key registry.example.com/app/web:1.5.0
cosign verify registry.example.com/app/web:1.5.0 --key cosign.pub
```

## Runtime защита

- Включайте `readonly-rootfs`, `no-new-privileges`:

```bash
docker run --read-only --tmpfs /tmp --security-opt no-new-privileges myorg/app
```

- Используйте `auditd`/Falco для мониторинга syscalls.

## Обновления и патчи

- Регулярно обновляйте базовые образы (`FROM debian:bookworm-slim -> ...`).
- Автоматические rebuilds при обновлении уязвимых пакетов.
- Собственный base image => собственные security baseline (CIS Docker Benchmark).

## RBAC и политики

- Ограничивайте доступ через встроенную систему ролей Docker EE / Mirantis.
- Для OSS-дистрибуции — используйте `sudo` и группы (`docker` группа = root).
- В Kubernetes (если используете) — контролируйте через PSP/OPA/Gatekeeper (но это вне Docker core).
