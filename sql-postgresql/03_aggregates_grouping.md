# Агрегаты и группировка

## Основные агрегаты
```sql
SELECT COUNT(*) AS cnt,
       COUNT(col) AS cnt_not_null,
       SUM(amount) AS total,
       AVG(amount) AS avg_amt,
       MIN(amount), MAX(amount)
FROM payments;
```
- `COUNT(*)` считает строки; `COUNT(col)` игнорирует NULL.

## GROUP BY
```sql
SELECT status, COUNT(*) AS cnt, SUM(total) AS sum_total
FROM orders
GROUP BY status;
```
- В `SELECT` допускаются только сгруппированные колонки и агрегаты.

## HAVING
```sql
SELECT customer_id, SUM(total) AS sum_total
FROM orders
GROUP BY customer_id
HAVING SUM(total) > 1000;
```
- Фильтр после агрегации; `WHERE` - до.

## Группировка по выражению
```sql
SELECT date_trunc('day', created_at) AS day, COUNT(*) AS cnt
FROM events
GROUP BY date_trunc('day', created_at)
ORDER BY day;
```
- Выражение в `SELECT` должно совпадать с `GROUP BY` (или быть функционально зависимым при включении `primary key`/`unique` - в чистом SQL лучше дублировать).

## GROUPING SETS / ROLLUP / CUBE
```sql
SELECT status, source, COUNT(*) AS cnt
FROM orders
GROUP BY GROUPING SETS (
  (status, source),   -- обычная группировка
  (status),           -- по status
  ()                  -- итого
);

SELECT status, source, COUNT(*) AS cnt
FROM orders
GROUP BY ROLLUP (status, source);  -- добавит промежуточные итоги слева направо

SELECT status, source, COUNT(*) AS cnt
FROM orders
GROUP BY CUBE (status, source);    -- все комбинации + итого
```
- Для различения строк итога: `GROUPING(status)` возвращает 1, если колонка агрегирована в этой строке.

## DISTINCT в агрегатах
```sql
SELECT COUNT(DISTINCT user_id) AS users,
       SUM(DISTINCT amount)    AS sum_unique_amounts
FROM payments;
```

## Условные агрегаты (FILTER)
```sql
SELECT
  COUNT(*) FILTER (WHERE status = 'paid')    AS paid,
  COUNT(*) FILTER (WHERE status = 'cancel')  AS canceled,
  AVG(total) FILTER (WHERE total > 0)        AS avg_positive
FROM orders;
```

## Пример: топ-N по группам
Обычно через оконные функции (см. [05_cte_window.md](05_cte_window.md)), но кратко:
```sql
SELECT *
FROM (
  SELECT o.*, ROW_NUMBER() OVER (PARTITION BY customer_id ORDER BY total DESC) AS rn
  FROM orders o
) s
WHERE rn <= 3;
```
