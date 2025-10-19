# Volumes и сети

## Volumes

```bash
# Список и создание
docker volume ls
docker volume create --name data-volume

# Использование
docker run -d --name db \
  -v data-volume:/var/lib/postgresql/data \
  postgres:16
```

- `--mount` более выразителен:

```bash
docker run -d --name db \
  --mount type=volume,src=data-volume,dst=/var/lib/postgresql/data \
  postgres:16
```

### Bind mounts и tmpfs

```bash
docker run -d --name web \
  --mount type=bind,src=/srv/web/config,dst=/etc/nginx/conf.d,readonly \
  --mount type=tmpfs,dst=/tmp/cache \
  nginx:1.25
```

- `:ro`, `:Z` (SELinux), `:cached`/`:delegated` (Docker Desktop) - опции для контроля.

### Backup/restore

```bash
docker run --rm \
  --mount type=volume,src=data-volume,dst=/data \
  -v "$(pwd)":/backup \
  alpine tar czf /backup/db.tar.gz -C /data .
```

## Сети

### Типы сетей

- `bridge` (по умолчанию): изолированная сеть на хосте.
- `host`: общий сетевой стек с хостом.
- `overlay`: распределённая сеть (Swarm / Docker Desktop).
- `macvlan`: контейнер получает MAC/IP из физической сети.

### Создание и подключение

```bash
docker network create --driver bridge --subnet 172.22.0.0/16 app-net

docker run -d --name api --network app-net \
  -p 8080:8080 app:latest
```

- Добавить существующий контейнер:

```bash
docker network connect app-net redis-cache
```

### Инспекция и диагностика

```bash
docker network inspect app-net
docker network ls

# Проверить DNS-резолв
docker run --rm --network app-net alpine nslookup api
```

### Порт-маппинг

```bash
docker run -d --name app \
  -p 80:80 \
  -p 443:443 \
  myorg/web:2.0
```

- `-p ip:host_port:container_port` (например, `-p 127.0.0.1:8080:80`).
- Для UDP добавляйте `/udp`.

### Firewall и ограничение доступа

- Изолируйте сети `docker network create --internal`.
- Управляйте iptables через `--iptables=false` (оставьте Docker, если нет потребности).
- Используйте `--icc=false` для запрета межконтейнерного трафика без явной сети.
