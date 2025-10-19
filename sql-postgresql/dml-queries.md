# DML: запросы, joins, подзапросы

## SELECT + JOIN
```sql
SELECT o.id, o.total, c.name
FROM orders o
JOIN customers c ON c.id = o.customer_id
LEFT JOIN payments p ON p.order_id = o.id
WHERE o.created_at >= now() - interval '30 days'
ORDER BY o.created_at DESC
LIMIT 50;
```

## Типы JOIN
- `INNER JOIN` — пересечение.
- `LEFT JOIN` — все из левой + совпадения.
- `RIGHT JOIN` — редко, симметрия.
- `FULL OUTER JOIN` — объединение.
- `CROSS JOIN` — декартово произведение.
- `LATERAL JOIN` — join с подзапросом, использующим поля из левой части.

## Подзапросы
```sql
-- inline
SELECT * FROM products WHERE price > (SELECT avg(price) FROM products);

-- EXISTS / NOT EXISTS
SELECT * FROM users u
WHERE EXISTS (
    SELECT 1 FROM orders o WHERE o.user_id = u.id AND o.status = 'pending'
);
```

## Common Table Expressions (CTE)
```sql
WITH recent_orders AS (
    SELECT * FROM orders WHERE created_at >= now() - interval '7 days'
)
SELECT * FROM recent_orders WHERE total > 100;
```
- Recusive CTE (`WITH RECURSIVE`) для деревьев.

## Window functions
```sql
SELECT user_id, total,
       rank() OVER (PARTITION BY user_id ORDER BY total DESC) AS order_rank
FROM orders;
```
- `row_number`, `rank`, `dense_rank`, `lag`, `lead`, `sum()` over window.

## JSON / JSONB
```sql
SELECT data->>'name', data->'stats'->>'score'
FROM events
WHERE (data->>'type') = 'purchase';
```
- `jsonb` оператор `@>`, индекс GIN.

## Upsert / batch
см. базовый синтаксис (ON CONFLICT).

## Pagination best practices
- `OFFSET/LIMIT` для небольших данных.
- Cursor: `WHERE (created_at, id) > ($1, $2)` ORDER BY.
- Используйте `keyset pagination` для больших таблиц.

