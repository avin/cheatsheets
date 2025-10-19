# Сеть и удалённый доступ

## Диагностика подключения

```bash
ip addr show                         # адреса интерфейсов
ip route show                        # таблица маршрутизации
ping -c 4 1.1.1.1                    # проверка доступности
traceroute example.com               # путь до узла (пакет traceroute)
ss -tulpen                           # активные сокеты и слушатели
```

## HTTP и загрузки

```bash
curl -I https://example.com          # HTTP-заголовки
curl -L -o file.tgz URL              # скачивание с учётом редиректов
wget -c URL                          # докачка прерванного файла
httpie GET https://httpbin.org/ip    # удобный HTTP-клиент (если установлен httpie)
```

## Копирование по сети

```bash
scp file user@host:/tmp/             # копирование через SSH
scp -r project user@host:~/          # рекурсивная передача
rsync -avh --progress src/ user@host:backup/src/   # синхронизация с контролем изменений
rsync -az -e 'ssh -p 2222' dir/ user@host:dir      # указать порт и сжатие
```

## SSH и туннели

```bash
ssh user@host                        # стандартное подключение
ssh -J jump user@target              # проксирование через jump host
ssh -L 8080:localhost:80 user@host   # локальный туннель
ssh-keygen -t ed25519 -C 'laptop'    # создание ключа
```

```bash
cat <<'EOF' >> ~/.ssh/config
Host prod
  HostName server.example.com
  User deploy
  IdentityFile ~/.ssh/id_ed25519
EOF
```
