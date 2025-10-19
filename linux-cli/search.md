# Поиск и фильтрация

## Поиск файлов по имени

```bash
find /var/log -type f -name '*.log'              # классический find
find . -maxdepth 2 -mtime -1 -size +10M          # файлы изменённые за сутки и крупнее 10 МБ
fd --hidden --type f config ~/.config            # быстрый поиск (пакет fd)
locate ssh_config                                # поиск по базе mlocate (обновлять `sudo updatedb`)
```

## Поиск по содержимому

```bash
grep -R "TODO" src/                              # рекурсивно по каталогу
grep -RIn "panic" src/                           # показать номера строк
rg --hidden --glob '!node_modules' "pattern"     # ripgrep, игнор npm-зависимости
```

## Фильтрация по времени и правам

```bash
find . -type f -perm 600                         # файлы с конкретными правами
find /tmp -type f -mtime +7 -delete              # удалить старше 7 дней
```

## Анализ вывода других команд

```bash
journalctl -u nginx | rg ERROR                   # быстро найти ошибки в логах
ls -lh | sort -k5 -h                             # отсортировать по размеру
```
