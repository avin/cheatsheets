# Временные таблицы, DDL инструменты

## Временные таблицы
```sql
CREATE TEMP TABLE tmp_orders AS
SELECT * FROM orders WHERE created_at >= CURRENT_DATE;
```
- Живут в сессии; не видны другим.

## CREATE TABLE AS / SELECT INTO
```sql
CREATE TABLE new_orders AS
SELECT * FROM orders WHERE status = 'new';

SELECT * INTO new_orders2 FROM orders WHERE status = 'new';  -- аналог (создает таблицу)
```

## Материализованные представления
```sql
CREATE MATERIALIZED VIEW mv_top_customers AS
SELECT customer_id, SUM(total) AS sum_total
FROM orders
GROUP BY customer_id;

REFRESH MATERIALIZED VIEW mv_top_customers;
REFRESH MATERIALIZED VIEW CONCURRENTLY mv_top_customers;  -- без блокировки чтения, нужен уник. индекс
```

## Обычные VIEW
```sql
CREATE OR REPLACE VIEW active_users AS
SELECT id, email FROM users WHERE active;

CREATE OR REPLACE VIEW city_orders WITH (security_barrier) AS
SELECT * FROM orders WHERE city = current_setting('app.city');
```
- Вьюхи не хранят данные, только запрос; `security_barrier` полезен с RLS/безопасностью.

## Секционирование (разделы)
```sql
CREATE TABLE events (
  id bigserial,
  created_at date,
  payload jsonb
) PARTITION BY RANGE (created_at);

CREATE TABLE events_2024 PARTITION OF events
  FOR VALUES FROM ('2024-01-01') TO ('2025-01-01');
```
- Типы: RANGE, LIST, HASH. Индексы создаются на партициях или через `CREATE INDEX ON ONLY parent ...`.
- Добавление: `CREATE TABLE events_def PARTITION OF events DEFAULT;` или `ATTACH PARTITION ... FOR VALUES ...`.
- Снятие: `ALTER TABLE events DETACH PARTITION events_2023;`

## Генерация таблицы из серии
```sql
CREATE TABLE calendar AS
SELECT generate_series('2025-01-01'::date, '2025-01-31', INTERVAL '1 day')::date AS d;
```
