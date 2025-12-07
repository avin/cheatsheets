# JOINы

## INNER/LEFT/RIGHT/FULL
```sql
SELECT o.id, c.name
FROM orders o
JOIN customers c ON c.id = o.customer_id;          -- inner

SELECT o.id, c.name
FROM orders o
LEFT JOIN customers c ON c.id = o.customer_id;     -- есть заказ даже без покупателя

SELECT o.id, c.name
FROM orders o
RIGHT JOIN customers c ON c.id = o.customer_id;    -- редко нужно

SELECT o.id, c.name
FROM orders o
FULL JOIN customers c ON c.id = o.customer_id;     -- обе стороны, пустые заполняются NULL
```

## USING и NATURAL
```sql
SELECT * FROM orders JOIN customers USING (customer_id);
```
- `USING (col)` убирает дублирующиеся колонки в выводе; подходит, если имя одинаково.
- `NATURAL JOIN` - соединение по всем совпадающим именам (лучше избегать, неявно).

## CROSS JOIN
```sql
SELECT * FROM colors CROSS JOIN sizes;  -- декартово произведение
```
- Можно записывать через запятую в `FROM`, но явный `CROSS JOIN` читаемее.

## Самосоединение
```sql
SELECT e.id, e.name, m.name AS manager
FROM employees e
LEFT JOIN employees m ON m.id = e.manager_id;
```
- Используй разные алиасы одной таблицы.

## JOIN на выражениях/условиях
```sql
SELECT o.id, p.name
FROM orders o
JOIN products p
  ON p.id = o.product_id
 AND p.active
 AND o.created_at >= p.valid_from;
```
- Условия join могут включать любые предикаты; фильтры после соединения - в `WHERE`.

## Полезно про NULL в соединениях
- В `LEFT JOIN` фильтр по правой таблице в `WHERE` делает соединение внутренним. Держи такие условия в `ON`, если хочешь сохранить строки без совпадения.
```sql
SELECT o.id, p.name
FROM orders o
LEFT JOIN products p
  ON p.id = o.product_id
 AND p.active = TRUE   -- условие внутри ON, чтобы не терять пустые совпадения
WHERE o.status = 'new';
```

## LATERAL (коротко)
```sql
SELECT u.id, x.top_order
FROM users u
LEFT JOIN LATERAL (
  SELECT o.id AS top_order
  FROM orders o
  WHERE o.user_id = u.id
  ORDER BY o.created_at DESC
  LIMIT 1
) x ON TRUE;
```
- Подзапрос ссылается на строки внешней таблицы; полезно для "top per row".
