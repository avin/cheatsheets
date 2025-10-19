# Процессы и службы

## Просмотр процессов

```bash
ps aux --sort=-%mem | head         # топ по памяти
pgrep -fl nginx                   # поиск процесса по имени
top                               # интерактивный мониторинг (Shift+M сортировка по памяти)
htop                              # более удобный top (если установлен)
```

## Управление процессами

```bash
kill -TERM 1234                   # корректно завершить
pkill -f 'python server.py'       # завершить по шаблону команды
kill -STOP 1234 && kill -CONT 1234   # приостановить/возобновить
```

## Приоритеты

```bash
nice -n 10 long_task.sh           # запустить с пониженным приоритетом
sudo renice -n -5 -p 1234         # повысить приоритет существующего процесса
```

## Службы (systemd)

```bash
systemctl status nginx            # статус сервиса
sudo systemctl restart nginx      # перезапуск
systemctl --user enable app.timer # пользовательский юнит
journalctl -u nginx -b            # логи текущей загрузки
journalctl -u nginx -f            # следить за логами
```
