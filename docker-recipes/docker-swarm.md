# Docker Swarm

## Инициализация и узлы

```bash
docker swarm init --advertise-addr 10.0.0.10

# Получить токены
docker swarm join-token manager
docker swarm join-token worker

# Присоединить узел
docker swarm join --token SWMTKN-... 10.0.0.10:2377
```

- `docker node ls` — список узлов, `docker node update --availability drain <node>` для осушения.
- Промоут/демот:

```bash
docker node promote worker1
docker node demote manager2
```

## Стэки

```bash
docker stack deploy -c stack.yaml prod
docker stack services prod
docker stack ps prod
docker stack rm prod
```

Пример `stack.yaml`:

```yaml
version: "3.9"

services:
  web:
    image: registry.example.com/app/web:1.5.0
    ports:
      - target: 80
        published: 80
        protocol: tcp
        mode: ingress
    networks:
      - frontend
    deploy:
      replicas: 3
      placement:
        constraints:
          - "node.labels.zone==eu-central"
      update_config:
        parallelism: 1
        delay: 10s
      rollback_config:
        parallelism: 0
        delay: 5s

  worker:
    image: registry.example.com/app/worker:1.5.0
    deploy:
      mode: replicated
      replicas: 5
      restart_policy:
        condition: on-failure

networks:
  frontend:
    driver: overlay
```

## Secrets и configs

```bash
echo "super-secret" | docker secret create db_password -
docker config create nginx_conf ./nginx.conf

services:
  web:
    secrets:
      - source: db_password
        target: db_password
        mode: 0400
    configs:
      - source: nginx_conf
        target: /etc/nginx/conf.d/default.conf
```

- Внутри контейнера секреты доступны по пути `/run/secrets/<name>`.
- Configs читаются как файлы, удобно для конфигов.

## Rolling updates и откаты

```bash
docker service update --image registry.example.com/app/web:1.5.1 prod_web
docker service rollback prod_web
```

- Можно обновлять ресурсы, env, replicas.
- `--force` перезапускает контейнеры без изменения конфигурации.

## Мониторинг и диагностика

```bash
docker service ls
docker service ps --no-trunc prod_web
docker node ps node-1
docker node inspect node-1 --pretty
```

- Для логов: `docker service logs -f prod_web`.
- `docker system df` показывает использование диска.

## Overlay сети

- `docker network create --driver overlay --attachable shared-net`
- Для доступа снаружи используйте ingress (Load Balancer -> published ports).

## Рекомендации

- Минимум 3 менеджера (кворум).
- Настройте источник времени (NTP).
- Регулярно обновляйте Docker Engine (Swarm обновляется вместе).
