# Базовый синтаксис и типы

## Подключение и сессия
- `psql postgresql://user:pass@host:5432/db` — CLI клиент.
- `\l` — список баз, `\c dbname` — переключение, `\dt` — таблицы.
- `\timing on` — показывать время запросов.

## Основные типы
- Числовые: `integer`, `bigint`, `numeric`, `serial`/`bigserial`.
- Строки: `text`, `varchar(n)`.
- Дата/время: `timestamp`, `timestamptz`, `date`, `time`, `interval`.
- Булевы: `boolean` (`true/false`).
- JSON: `json`, `jsonb` (бинарный, индексируемый).
- Array: `integer[]`, `text[]`.
- UUID: `uuid` (генерировать через `pgcrypto`/`uuid-ossp`).
- `enum`, `range` типы (`numrange`, `tsrange`).

## SELECT basics
```sql
SELECT column1, column2
FROM table
WHERE condition
GROUP BY column
HAVING aggregate_condition
ORDER BY column ASC
LIMIT 100 OFFSET 0;
```

## INSERT/UPDATE/DELETE
```sql
INSERT INTO users (id, name, email)
VALUES (gen_random_uuid(), 'Alice', 'alice@example.com')
RETURNING *;

UPDATE users
SET name = 'Alice Smith'
WHERE id = 'uuid'
RETURNING *;

DELETE FROM users WHERE id = 'uuid';
```

## UPSERT (ON CONFLICT)
```sql
INSERT INTO accounts (id, balance)
VALUES ($1, $2)
ON CONFLICT (id)
DO UPDATE SET balance = accounts.balance + EXCLUDED.balance;
```

## Comments
- `-- single line`
- `/* multi
   line */`

## Соглашения
- snake_case для имен таблиц/колонок.
- Primary keys — surrogate (UUID, serial), foreign keys.
- Используйте `NOT NULL` по умолчанию, `CHECK` для ограничений.
