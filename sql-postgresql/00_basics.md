# База

## Подключение и сессия
- CLI: `psql -h host -p 5432 -U user -d db` (пароль через `PGPASSWORD` или `.pgpass`).
- Метаданные: `\dt` (таблицы), `\d table` (структура), `\df` (функции), `\l` (БД), `\c db` (смена БД), `\dn` (схемы), `\du` (роли).
- Выход: `\q`. Формат: `\pset format aligned|unaligned`, `\timing` - включить тайминг.

## Синтаксис и идентификаторы
- Регистр: незаключенные идентификаторы приводятся к lower; чувствительный регистр - только в кавычках `"CamelCase"`.
- Схемы: `schema.table`; поиск управляется `search_path`.
- Комментарии: `-- однострочный`, `/* блочный */`.
- Константы: строки в `'...'`, экранирование `''`; для спецсимволов - E-строки `E'line\n'`.

## Типы данных (частые)
- `boolean` - логика; `smallint|integer|bigint` - целые; `numeric(p,s)` - точная дробь; `real|double precision` - плавающая; `serial|bigserial` - автоинкремент.
- `text`, `varchar(n)`, `char(n)` - строки; `citext` - нечувствительная к регистру; `uuid` - идентификаторы.
- `date`, `time [with[out] time zone]`, `timestamp [with[out] time zone]`, `interval` - календарь/время.
- `bytea` - бинарные; `json|jsonb` - структурированные; `xml`; `inet|cidr|macaddr` - сети; `money`; `bit` - битовые.
- `enum` (`CREATE TYPE mood AS ENUM ('sad','ok','happy')`), `int4range|numrange|daterange|tsrange` - диапазоны; `hstore` (ключ-значение); `tsvector|tsquery` - полнотекст.
- Массивы: `int[]`, `text[]`; многомерные - `int[][]`.

## NULL, сравнения, логика
- `IS NULL`/`IS NOT NULL` - проверка; `NULL` не равно ничему, даже себе.
- Операторы: `=`, `<>`, `<`, `>`, `<=`, `>=`; результат может быть `UNKNOWN`.
- Комбинации: `AND`, `OR`, `NOT`; приоритет `()` > `NOT` > `AND` > `OR`.
- Подстановка/обнуление: `COALESCE(a, b)` - первое не-NULL; `NULLIF(a,b)` - вернуть NULL при равенстве.

## Базовая выборка
```sql
SELECT col1 AS c1, col2
FROM schema.table t
WHERE t.flag = TRUE AND t.kind IN ('A', 'B')
ORDER BY t.created_at DESC
LIMIT 10 OFFSET 20;
```
- Алиасы можно без `AS`: `table t`, `col c`.
- `DISTINCT` - убирает дубликаты по указанным колонкам.
- `ORDER BY 1 DESC` - сортировка по позиции колонки в SELECT.

## Вставка, обновление, удаление (кратко)
```sql
INSERT INTO tab (a, b) VALUES (1, 'x');
UPDATE tab SET b = 'y' WHERE id = 10;
DELETE FROM tab WHERE a < 0;
```
- Возврат строк из DML: `... RETURNING *;` (подробно в [06_modification.md](06_modification.md)).

## Типовые преобразования
- Явное: `CAST(col AS integer)` или `col::integer` - управляемый тип.
- Форматирование/парсинг дат: `TO_CHAR(now(), 'YYYY-MM-DD')`, `TO_DATE('2025-01-31','YYYY-MM-DD')`.
- Задание времени с TZ: `TIMESTAMPTZ '2025-01-31 12:00+03'`.
- Интервалы: `INTERVAL '2 days'`; арифметика: `now() + INTERVAL '1 day'`.

## Полезные служебные
- Текущее время: `now()`, `CURRENT_DATE`, `CURRENT_TIMESTAMP`, `clock_timestamp()` (реальное время при каждом вызове).
- Случайное: `random()`; генератор последовательностей: `generate_series(1,5)` (см. `16_misc.md`).
- Версия сервера: `SELECT version();`.

## Мини-справка по DDL
```sql
CREATE TABLE IF NOT EXISTS demo (
  id bigserial PRIMARY KEY,
  name text NOT NULL,
  active boolean DEFAULT TRUE,
  created_at timestamptz DEFAULT now()
);
```
- Добавить колонку: `ALTER TABLE demo ADD COLUMN note text;`
- Изменить тип: `ALTER TABLE demo ALTER COLUMN name TYPE varchar(50);`
- Удалить: `DROP TABLE demo;` (при необходимости `CASCADE`).
- Современная автонумерация: `GENERATED ALWAYS AS IDENTITY` (аналог serial без лишней sequence роли).
- Сгенерированные колонки: `ALTER TABLE demo ADD COLUMN name_upper text GENERATED ALWAYS AS (upper(name)) STORED;`
- Пользовательские типы: `CREATE TYPE status AS ENUM ('new','done');` или `CREATE DOMAIN email AS text CHECK (value LIKE '%@%');`
