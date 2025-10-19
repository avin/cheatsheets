# Архивы и сжатие

## tar + gzip/bzip2/xz

```bash
tar czf backup.tar.gz project/               # создать архив gzip
tar cJf backup.tar.xz project/               # xz с лучшим сжатием
tar tzf backup.tar.gz                        # просмотреть содержимое без распаковки
tar xf backup.tar.gz -C /tmp/restore         # распаковать в указанный каталог
```

## zip и unzip

```bash
zip -r logs.zip logs/                        # создать zip-архив
unzip -l logs.zip                            # список файлов
unzip logs.zip -d /tmp/logs                  # распаковать
```

## Параллельное сжатие

```bash
pigz -p 4 bigfile                            # gzip-магазина с несколькими потоками
xz -T0 bigfile                               # xz задействует все доступные ядра
```

## Сжатие без архивации

```bash
gzip --keep access.log                       # оставить оригинал
zstd file.txt                                # сверхбыстрое сжатие (пакет zstd)
```

## Работа с .tar.{gz,xz} в одну команду

```bash
tar caf backup.tar.zst project/              # GNU tar сам подбирает сжатие по расширению
```
