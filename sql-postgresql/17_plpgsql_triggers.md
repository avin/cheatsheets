# Функции, процедуры и триггеры

## Функции (plpgsql)
```sql
CREATE OR REPLACE FUNCTION inc(val int, step int DEFAULT 1)
RETURNS int
LANGUAGE plpgsql
IMMUTABLE
AS $$
BEGIN
  RETURN val + step;
END;
$$;
```
- `IMMUTABLE`/`STABLE`/`VOLATILE` влияет на планировщик; для запросов безопаснее `STABLE`.
- Значения по умолчанию для аргументов; `RETURNS TABLE(...)` для множественных колонок; `RETURNS SETOF` для набора.
- Вызов: `SELECT inc(5);`

## Процедуры
```sql
CREATE OR REPLACE PROCEDURE do_cleanup(days int)
LANGUAGE plpgsql
AS $$
BEGIN
  DELETE FROM logs WHERE created_at < CURRENT_DATE - days;
  COMMIT;  -- в процедурах разрешен контроль транзакций
END;
$$;

CALL do_cleanup(30);
```
- В отличие от функций, процедуры можно `CALL` и управлять транзакцией.

## DO-блок (одноразовый)
```sql
DO $$
DECLARE
  v integer := 1;
BEGIN
  RAISE NOTICE 'value: %', v;
END;
$$;
```

## SECURITY DEFINER / search_path
```sql
CREATE OR REPLACE FUNCTION admin_only()
RETURNS void
LANGUAGE plpgsql
SECURITY DEFINER
SET search_path = public
AS $$ BEGIN PERFORM 1; END; $$;
```
- Выполняется с правами владельца; всегда фиксируй `search_path`, чтобы избежать подмены.

## Триггеры
```sql
CREATE OR REPLACE FUNCTION set_updated_at()
RETURNS trigger AS $$
BEGIN
  NEW.updated_at := now();
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_set_updated_at
BEFORE UPDATE ON users
FOR EACH ROW
WHEN (OLD.* IS DISTINCT FROM NEW.*)
EXECUTE FUNCTION set_updated_at();
```
- `BEFORE|AFTER|INSTEAD OF`, `FOR EACH ROW` или `FOR EACH STATEMENT`.
- В триггере доступны `NEW`/`OLD` записи (для `INSERT` только `NEW`, для `DELETE` только `OLD`).

## Event triggers (DDL)
```sql
CREATE OR REPLACE FUNCTION log_ddl()
RETURNS event_trigger AS $$
BEGIN
  INSERT INTO ddl_log(objid, tag) VALUES (tg_event, tg_tag);
END;
$$ LANGUAGE plpgsql;

CREATE EVENT TRIGGER trg_ddl ON ddl_command_end
EXECUTE FUNCTION log_ddl();
```
- Срабатывают на события DDL (`ddl_command_start|end`, `sql_drop` и др.).
