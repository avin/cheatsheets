# Роли, права, утилиты

## Роли/пользователи
```sql
CREATE ROLE app LOGIN PASSWORD 'secret';
ALTER ROLE app WITH CREATEDB;        -- права
DROP ROLE app;
```
- Роли могут быть без LOGIN (группы).

## Привилегии
```sql
GRANT SELECT, INSERT ON TABLE public.users TO app;
GRANT USAGE ON SCHEMA public TO app;
GRANT CONNECT ON DATABASE mydb TO app;
REVOKE INSERT ON public.users FROM app;
```
- Права на объекты: `TABLE`, `SEQUENCE`, `FUNCTION`, `SCHEMA`, `DATABASE`.

## Владелец и передачи
```sql
ALTER TABLE users OWNER TO app;
GRANT app TO admin;           -- admin получает роль app
SET ROLE app;                 -- временно принять роль
```

## Схемы и search_path
```sql
CREATE SCHEMA IF NOT EXISTS reporting AUTHORIZATION app;
SET search_path TO reporting, public;
```

## Политики RLS (Row-Level Security)
```sql
ALTER TABLE accounts ENABLE ROW LEVEL SECURITY;
CREATE POLICY account_owner ON accounts
  USING (owner_id = current_user_id());   -- функция или колонка пользователя
ALTER TABLE accounts FORCE ROW LEVEL SECURITY;           -- даже для владельца
```
- Правила на чтение (`USING`) и запись (`WITH CHECK`); политики комбинируются с `OR`.

## Полезные GRANTы скопом
```sql
GRANT SELECT ON ALL TABLES IN SCHEMA public TO app;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT ON TABLES TO app;
```
- `ALTER DEFAULT PRIVILEGES` задает права для будущих объектов.

## Служебные \команды psql (коротко)
- `\dt [schema.]pattern` - таблицы.
- `\d table` - структура.
- `\df` - функции.
- `\du` - роли.
- `\dn` - схемы.
- `\l` - базы.

## Диагностика и планы
```sql
EXPLAIN SELECT * FROM orders WHERE status = 'new';
EXPLAIN (ANALYZE, BUFFERS) SELECT * FROM orders WHERE status = 'new';
```
- `ANALYZE` запускает выполнение, `BUFFERS` показывает работу с кешом.

## Статистика и системные вьюхи
```sql
SELECT * FROM pg_stat_activity;
SELECT * FROM pg_locks;
SELECT relname, n_live_tup, n_dead_tup FROM pg_stat_all_tables WHERE schemaname = 'public';
```
