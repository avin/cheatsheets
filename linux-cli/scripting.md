# Bash-скрипты и автоматизация

## Заголовок и режимы

```bash
#!/usr/bin/env bash
set -euo pipefail            # выйти при ошибке, использовать строгий режим
IFS=$'\n\t'
```

## Переменные и строки

```bash
name="World"
echo "Hello, ${name}"
readonly VERSION="1.2.3"
```

## Функции и возвраты

```bash
log() { printf '[%s] %s\n' "$(date +%H:%M:%S)" "$*"; }

cleanup() {
  rm -f "$TMPFILE"
}
trap cleanup EXIT
```

## Циклы и условия

```bash
for file in *.log; do
  [[ -e "$file" ]] || continue
  gzip "$file"
done

if [[ ${#@} -eq 0 ]]; then
  echo "Usage: $0 <args>" >&2
  exit 1
fi
```

## Массивы и словари

```bash
files=(a.txt b.txt c.txt)
declare -A ports=([http]=80 [https]=443)
for role in "${!ports[@]}"; do
  echo "$role -> ${ports[$role]}"
done
```

## cron

```bash
crontab -e                          # отредактировать задания
# */15 * * * * /home/user/backup.sh   # пример запуска каждые 15 минут
```

## systemd-таймер

```bash
cat <<'EOF' > ~/.config/systemd/user/cleanup.service
[Unit]
Description=Очистка tmp

[Service]
Type=oneshot
ExecStart=/home/user/bin/cleanup.sh
EOF

cat <<'EOF' > ~/.config/systemd/user/cleanup.timer
[Unit]
Description=Очистка tmp ежедневно

[Timer]
OnCalendar=*-*-* 03:00
Persistent=true

[Install]
WantedBy=timers.target
EOF

systemctl --user enable --now cleanup.timer
```
