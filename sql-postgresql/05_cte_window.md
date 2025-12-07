# CTE и оконные функции

## WITH (обычный CTE)
```sql
WITH recent AS (
  SELECT * FROM orders WHERE created_at >= CURRENT_DATE - INTERVAL '7 days'
)
SELECT customer_id, COUNT(*) FROM recent GROUP BY customer_id;
```
- Читаемая декомпозиция запроса; виден только внутри запроса.

## Рекурсивный CTE
```sql
WITH RECURSIVE tree AS (
  SELECT id, parent_id, name, 1 AS lvl
  FROM categories
  WHERE parent_id IS NULL
  UNION ALL
  SELECT c.id, c.parent_id, c.name, t.lvl + 1
  FROM categories c
  JOIN tree t ON t.id = c.parent_id
)
SELECT * FROM tree;
```
- База + рекурсивная часть, объединены `UNION ALL`; обязательно `WITH RECURSIVE`.

## Оконная функция: общая форма
```sql
<aggregate_or_window>() OVER (
  PARTITION BY <разбивка>
  ORDER BY <сортировка>
  ROWS|RANGE BETWEEN ...        -- опционально: рамка
)
```
- Возвращает значение по окну строк, не схлопывает набор.

## Ранжирование
```sql
SELECT id, total,
       ROW_NUMBER() OVER (ORDER BY total DESC) AS rn,
       RANK()       OVER (ORDER BY total DESC) AS rnk,
       DENSE_RANK() OVER (ORDER BY total DESC) AS drnk
FROM orders;
```
- `ROW_NUMBER` без пропусков; `RANK` с пропусками на равных; `DENSE_RANK` без пропусков.

## Оконные агрегаты
```sql
SELECT customer_id, total,
       SUM(total)    OVER (PARTITION BY customer_id ORDER BY created_at) AS running_sum,
       AVG(total)    OVER (PARTITION BY customer_id) AS avg_per_customer,
       COUNT(*)      OVER () AS total_rows
FROM orders;
```
- По умолчанию рамка `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW` при наличии ORDER BY.

## Фреймы (ROWS vs RANGE)
```sql
SUM(total) OVER (
  PARTITION BY customer_id
  ORDER BY created_at
  ROWS BETWEEN 1 PRECEDING AND CURRENT ROW
) AS sum_prev_curr
```
- `ROWS` - по количеству строк; `RANGE` - по значениям сортировки (для чисел/дат).

## LAST_VALUE и игнорирование рамки
```sql
SELECT id, total,
       LAST_VALUE(total) OVER (
         ORDER BY created_at
         ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING
       ) AS last_total
FROM orders;
```
- Меняй рамку, иначе `LAST_VALUE` вернет значение текущей строки.

## LAG/LEAD, FIRST_VALUE
```sql
SELECT id, total,
       LAG(total, 1)  OVER (ORDER BY created_at) AS prev_total,
       LEAD(total, 1) OVER (ORDER BY created_at) AS next_total,
       FIRST_VALUE(total) OVER (ORDER BY created_at) AS first_total
FROM orders;
```
- Сдвиг на n строк; можно указать значение по умолчанию третьим аргументом.

## Пример: top-N на группу
```sql
SELECT *
FROM (
  SELECT o.*,
         ROW_NUMBER() OVER (PARTITION BY customer_id ORDER BY total DESC) AS rn
  FROM orders o
) s
WHERE rn <= 3;
```
