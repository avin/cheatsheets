# Базовые команды и контейнеры

## Запуск и управление жизненным циклом

```bash
# Запустить контейнер в фоне
docker run -d --name redis-cache -p 6379:6379 redis:7-alpine

# Запустить одноразовую задачу
docker run --rm -it alpine:3.19 sh

# Перезапустить/остановить/удалить
docker restart redis-cache
docker stop redis-cache
docker rm redis-cache
```

- Используйте `--rm` для временных контейнеров, чтобы избегать мусора.
- Флаг `--pull=always` гарантирует актуальный образ.

## Инспекция и копирование

```bash
# Информация о контейнере и его конфигурации
docker inspect redis-cache

# Логи и стриминг
docker logs -f redis-cache

# Выполнить команду внутри работающего контейнера
docker exec -it redis-cache redis-cli info

# Копирование файлов
docker cp ./backup.rdb redis-cache:/data/dump.rdb
docker cp redis-cache:/var/log/app.log ./logs/
```

## Фильтрация и списки

```bash
# Список контейнеров
docker ps
docker ps -a

# Фильтрация по статусу/имени
docker ps --filter "status=exited"
docker ps --filter "name=web"
```

## Ресурсы и лимиты

```bash
docker run -d --name cpu-limited \
  --cpus=1.5 \
  --memory=512m \
  --memory-swap=1g \
  busybox:1.36 sleep 3600
```

- `--cpu-shares` задаёт относительный вес.
- `--pids-limit` ограничивает количество процессов.

## Namespaces и capabilities (кратко)

- `--pid=host`, `--net=host`, `--uts=host` разделяют или шарят namespace с хостом.
- `--cap-add`, `--cap-drop` управляют привилегиями:

```bash
docker run --cap-drop=ALL --cap-add=NET_BIND_SERVICE nginx:1.25
```

## Cleanup и housekeeping

```bash
# Удалить остановленные контейнеры
docker container prune -f

# Удалить все (опасно!)
docker rm -f $(docker ps -aq)

# События и таймлайн
docker events --since 1h
```

- Планируйте cron/systemd timer для `prune` на build-агентах.
- Используйте `docker ps --format` для скриптов и интеграций.
