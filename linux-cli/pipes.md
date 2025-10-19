# Перенаправления и конвейеры

## Потоки ввода/вывода

```bash
command > out.log              # stdout в файл (перезапись)
command >> out.log             # stdout в файл (добавление)
command 2> errors.log          # stderr в файл
command &> all.log             # stdout и stderr вместе
command > out.log 2>&1         # POSIX-совместимый вариант объединения
```

## Конвейеры

```bash
grep -R "pattern" src | less    # передать вывод следующей команде
cat numbers.txt | paste -sd+ | bc             # подсчёт суммы в конвейере
```

## Heredoc и Herestring

```bash
cat <<'EOF' > script.sh
#!/usr/bin/env bash
echo "Hello!"
EOF
```

```bash
bc <<< "2^16"                    # передать короткую строку на stdin
```

## xargs и параллелизм

```bash
find . -name '*.tmp' -print0 | xargs -0 rm -f        # безопасно работать с пробелами
printf '%s\0' url1 url2 | xargs -0 -n1 -P4 curl -O   # параллельная загрузка
```

## tee и многоцелевой вывод

```bash
make 2>&1 | tee build.log         # одновременно смотреть в терминале и писать в файл
```
