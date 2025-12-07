# Изменение данных

## INSERT
```sql
INSERT INTO users (email, name) VALUES ('a@ex.com', 'Ann');
INSERT INTO users (email, name) VALUES
  ('b@ex.com', 'Bob'),
  ('c@ex.com', 'Cat');           -- многократные значения

INSERT INTO archive_users
SELECT * FROM users WHERE active = FALSE;  -- вставка из запроса
```

## ON CONFLICT (upsert)
```sql
INSERT INTO users (id, email, name)
VALUES (1, 'a@ex.com', 'Ann')
ON CONFLICT (id) DO UPDATE
  SET email = EXCLUDED.email, updated_at = now();

INSERT INTO users (email, name)
VALUES ('x@ex.com','X')
ON CONFLICT (email) DO NOTHING;
```
- `EXCLUDED.col` - значение из вставки.

## RETURNING
```sql
INSERT INTO users (email) VALUES ('new@ex.com') RETURNING id;
UPDATE users SET active = FALSE WHERE last_login < now() - INTERVAL '1 year' RETURNING id, email;
DELETE FROM users WHERE active = FALSE RETURNING *;
```
- Удобно для получения ключей/измененных строк.

## UPDATE
```sql
UPDATE orders
SET total = total * 0.9,
    updated_at = now()
WHERE status = 'new';
```
- Можно ссылаться на другие таблицы: `UPDATE ... FROM ...`.
```sql
UPDATE orders o
SET customer_name = c.name
FROM customers c
WHERE c.id = o.customer_id;
```

## DELETE
```sql
DELETE FROM orders WHERE created_at < CURRENT_DATE - INTERVAL '90 days';
```
- С `USING` для условий: 
```sql
DELETE FROM orders o
USING customers c
WHERE o.customer_id = c.id AND c.active = FALSE;
```

## Массовые операции и CTE
```sql
WITH moved AS (
  DELETE FROM orders
  WHERE created_at < CURRENT_DATE - INTERVAL '365 days'
  RETURNING *
)
INSERT INTO orders_archive SELECT * FROM moved;
```
- Позволяет «переносить» строки за один запрос.

## COPY (импорт/экспорт)
```sql
COPY users TO '/tmp/users.csv' CSV HEADER;
COPY users FROM '/tmp/users.csv' CSV HEADER;
```
- Требует прав на файловую систему сервера; в `psql` можно `\copy` для клиентской стороны.
