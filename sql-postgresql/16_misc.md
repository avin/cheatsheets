# Разное полезное

## Генераторы
```sql
SELECT generate_series(1,5);                          -- int
SELECT generate_series('2025-01-01'::date, '2025-01-10', INTERVAL '1 day'); -- даты
SELECT generate_series(0, 1, 0.2);                    -- шаг дробный
```

## Случайные значения
```sql
SELECT random();                          -- 0..1
SELECT floor(random() * 10 + 1)::int;     -- 1..10
```

## Последовательности
```sql
CREATE SEQUENCE seq_demo START 1 INCREMENT 1;
SELECT nextval('seq_demo'), currval('seq_demo');
ALTER SEQUENCE seq_demo RESTART WITH 1;
```
- `serial/bigserial` автоматически создает sequence и default.

## COPY через psql
- Серверный `COPY` требует доступ к файлам сервера; в `psql` используйте клиентский:
```psql
\copy users TO 'users.csv' CSV HEADER
\copy users FROM 'users.csv' CSV HEADER
```

## Полезные системные представления
```sql
SELECT * FROM pg_indexes WHERE schemaname = 'public';
SELECT * FROM information_schema.tables WHERE table_schema = 'public';
SELECT * FROM pg_stat_activity;
```

## Расширения
```sql
CREATE EXTENSION IF NOT EXISTS citext;
CREATE EXTENSION IF NOT EXISTS uuid-ossp;
```
- Расширяют типы/функции (требуются права).

## Размеры и статистика
```sql
SELECT pg_size_pretty(pg_database_size(current_database()));
SELECT relname, pg_size_pretty(pg_total_relation_size(oid)) AS total
FROM pg_class
WHERE relkind = 'r'
ORDER BY pg_total_relation_size(oid) DESC
LIMIT 10;
```

## Быстрые приемы в psql
- `\watch 5` - повторять последний запрос каждые 5 секунд.
- `\echo :VAR` и `\set VAR 'value'` - переменные.
