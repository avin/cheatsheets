# Функции, агрегаты и операторы

## Агрегаты
- `count(*)`, `sum(column)`, `avg(column)`.
- `string_agg(column, ',')` — объединение строк.
- `array_agg(column)` — массив значений.
- Кастомные агрегаты (`CREATE AGGREGATE`).

## Built-in функции
- **Дата/время**: `now()`, `current_timestamp`, `age(timestamp)`, `date_trunc('month', ts)`.
- **Текст**: `lower`, `upper`, `substring`, `regexp_replace`.
- **JSON**: `jsonb_extract_path_text`, `jsonb_set`.
- **Array**: `unnest()`, `array_position`, `array_remove`.
- **Math**: `round`, `floor`, `ceil`, `power`, `mod`.

## Операторы
- Сравнение: `=`, `<>`, `<`, `>`, `<=`, `>=`.
- IN/ANY/ALL: `value IN (..)`.
- LIKE/ILIKE: `%` wildcard (ILIKE — case-insensitive).
- `BETWEEN`, `COALESCE`, `NULLIF`.
- JSONB: `->`, `->>`, `#>>`, containment `@>`, key existence `?`.
- Array: `&&` (пересечение), `@>`.

## Custom Functions
```sql
CREATE OR REPLACE FUNCTION calculate_discount(amount numeric)
RETURNS numeric AS $$
BEGIN
    IF amount > 1000 THEN
        RETURN amount * 0.9;
    ELSE
        RETURN amount;
    END IF;
END;
$$ LANGUAGE plpgsql;
```
- Используйте `LANGUAGE SQL` для простых функций.

## Triggers
```sql
CREATE FUNCTION set_updated_at()
RETURNS trigger AS $$
BEGIN
    NEW.updated_at := now();
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER users_set_updated
BEFORE UPDATE ON users
FOR EACH ROW EXECUTE FUNCTION set_updated_at();
```

## Extension
- Включайте через `CREATE EXTENSION` (pgcrypto, uuid-ossp, hstore, postgis).

