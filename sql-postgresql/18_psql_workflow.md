# psql и ежедневная работа

`psql` — клиент PostgreSQL. Его команды с обратной косой чертой обрабатывает клиент, поэтому их удобнее держать здесь, а не в SQL-файлах с рецептами.

## Подключение и безопасный запуск файла

```bash
psql "postgresql://localhost:5432/shop?sslmode=require" -U app_reader
psql -X -v ON_ERROR_STOP=1 -d shop -f migrations/001_create_users.sql
```

- **Когда применять**: быстрый просмотр базы и повторяемый запуск SQL-файла.
- **Плюсы**: `ON_ERROR_STOP` останавливает сценарий при первой ошибке; `-X` игнорирует пользовательский `.psqlrc`.
- **Риск**: не помещайте пароль в командную строку или историю оболочки. Для локальной работы используйте защищённый файл учётных данных или запрос пароля.
- Несколько операторов из файла не становятся одной транзакцией автоматически. Если нужна атомарность, явно используйте `BEGIN`/`COMMIT` или `--single-transaction`; учитывайте команды вроде `CREATE INDEX CONCURRENTLY`.

## Команды внутри psql

```text
\conninfo          текущее подключение
\dn                схемы
\dt app.*          таблицы схемы app
\d+ app.orders     колонки, индексы и размер
\x auto            расширенный вывод для широких строк
\timing on         время выполнения каждого запроса
\i ./query.sql     выполнить локальный SQL-файл
\q                 выход
```

Это команды `psql`, а не SQL: не отправляйте их через драйвер приложения.

## Импорт CSV с машины клиента

```text
\copy app.import_stage(email, display_name) FROM './users.csv' WITH (FORMAT csv, HEADER true)
```

- **Когда применять**: CSV находится на вашей машине; `COPY ... FROM '/path'` читает путь на сервере.
- **Плюсы**: не нужны права сервера на локальный файл.
- **Риск**: проверьте порядок колонок, кодировку и результат на небольшой партии перед полной загрузкой.

## Выгрузка и диагностика

```bash
pg_dump --format=custom --file=shop.dump shop
pg_restore --list shop.dump
psql -d shop -c "SELECT current_database(), current_user, current_setting('TimeZone');"
```

`pg_dump` создаёт логическую резервную копию. Проверяйте восстановление отдельно: наличие файла ещё не доказывает, что резервная копия пригодна.

## Справочник

- [PostgreSQL: psql](https://www.postgresql.org/docs/current/app-psql.html)
- [PostgreSQL: COPY](https://www.postgresql.org/docs/current/sql-copy.html)
- [PostgreSQL: pg_dump](https://www.postgresql.org/docs/current/app-pgdump.html)
