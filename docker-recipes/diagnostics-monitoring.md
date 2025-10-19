# Диагностика и мониторинг

## Логи и их просмотр

```bash
docker logs -f --tail=100 app
docker logs --since 10m app

# JSON-файлы логов (по умолчанию)
sudo tail -f /var/lib/docker/containers/<container-id>/*-json.log
```

- Настройте драйвер логов (`gelf`, `syslog`, `fluentd`, `awslogs`):

```bash
docker run -d --log-driver=gelf --log-opt gelf-address=udp://graylog:12201 myorg/app
```

## Мониторинг ресурсов

```bash
docker stats --no-stream
docker stats app1 app2
```

- Для Swarm: `docker service ps --no-trunc`, `docker node ps`.

## Инспекция сети

```bash
docker exec -it app ss -tulpen
docker exec -it app ip addr show
docker network inspect app-net
```

- Проверка соединений:

```bash
docker exec -it app curl -v http://service:8080
```

## Диагностика файловой системы

```bash
docker system df
docker system prune
docker volume inspect data-volume
docker run --rm -it -v data-volume:/data alpine ls -R /data
```

- Удаление dangling images: `docker image prune`.

## События и таймлайн

```bash
docker events --since 1h --until 10m
docker events --filter 'container=app'
```

- Используйте для аудита: кто/когда стартовал контейнер.

## Healthcheck

- Команда: `docker inspect --format '{{json .State.Health}}' app`.
- Состояние: `starting`, `healthy`, `unhealthy`.

## Интеграция с Prometheus / cAdvisor

- Запустите cAdvisor:

```bash
docker run -d \
  --name=cadvisor \
  --volume=/:/rootfs:ro \
  --volume=/var/run:/var/run:ro \
  --volume=/sys:/sys:ro \
  --volume=/var/lib/docker/:/var/lib/docker:ro \
  --publish=8080:8080 \
  gcr.io/cadvisor/cadvisor:v0.47.0
```

- Scrape метрики Prometheus, визуализируйте в Grafana.

## Интеграция с ELK/EFK

- Логи в Elasticsearch:

```bash
docker run -d --log-driver=gelf --log-opt gelf-address=udp://logstash:12201 ...
```

- Используйте Fluent Bit/Logstash для парсинга JSON.

## Troubleshooting

- `docker container top app` — процессы внутри.
- `docker exec app cat /etc/os-release` — базовый образ.
- `docker inspect app --format '{{.HostConfig.RestartPolicy.Name}}'`
- `journalctl -u docker` — проблемы с демоном.

## Профилирование и дебаг

- `docker run --privileged --pid=host justincormack/nsenter1` для доступа к namespaces.
- `strace`/`gdb` внутри контейнера (при необходимости).
- Для сети: `tcpdump` на хосте `sudo tcpdump -i br-<id>`.
