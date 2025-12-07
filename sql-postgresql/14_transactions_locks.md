# Транзакции и блокировки

## Базовые команды
```sql
BEGIN;
-- запросы
COMMIT;   -- или ROLLBACK;
```
- В `psql`: `BEGIN;`/`COMMIT;` или `\set AUTOCOMMIT off`.

## Уровни изоляции
```sql
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;   -- по умолчанию
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
```
- `READ COMMITTED`: каждое чтение видит зафиксированные на момент запроса.
- `REPEATABLE READ`: снимок на момент начала транзакции.
- `SERIALIZABLE`: обнаруживает конфликтующие сериализационные аномалии (возможны ошибки).

## SELECT ... FOR UPDATE/SHARE
```sql
SELECT * FROM orders WHERE id = 10 FOR UPDATE;           -- блокирует строки для записи другими
SELECT * FROM orders WHERE id = 10 FOR NO KEY UPDATE;    -- мягче, блокирует без ключей
SELECT * FROM orders WHERE id = 10 FOR SHARE;            -- разделяемый
SELECT * FROM orders WHERE id = 10 FOR KEY SHARE;        -- разделяемый для ключей
```
- `FOR UPDATE NOWAIT` или `FOR UPDATE SKIP LOCKED` - не ждать блок/пропустить заблокированные.

## SAVEPOINT и двухфазные
```sql
SAVEPOINT sp1;
-- ... запросы ...
ROLLBACK TO sp1;   -- откатить часть

PREPARE TRANSACTION 'txn1';      -- двухфазный коммит (осторожно)
COMMIT PREPARED 'txn1';          -- завершить
```

## Явные блокировки таблиц
```sql
LOCK TABLE orders IN SHARE MODE;       -- редкий кейс, осторожно
```

## Advisory locks (логические)
```sql
SELECT pg_advisory_lock(42);      -- ждать
SELECT pg_try_advisory_lock(42);  -- не ждать, boolean
SELECT pg_advisory_unlock(42);
```
- Блокировки по произвольному ключу (session или transaction). Не проверяют существование строк.

## Deadlock и порядок
- Всегда берите блокировки в одном порядке между транзакциями, чтобы избежать дедлоков.
- Обрабатывать ошибку `deadlock detected` повтором.

## Просмотр блокировок и активных запросов
```sql
SELECT * FROM pg_locks WHERE NOT granted;
SELECT pid, state, query FROM pg_stat_activity WHERE state != 'idle';
```

## Автофиксация DDL
- DDL-команды (CREATE/ALTER/DROP) сами открывают транзакцию; в `psql` можно обернуть в ручной `BEGIN`.
