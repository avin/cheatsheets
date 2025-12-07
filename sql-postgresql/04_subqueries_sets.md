# Подзапросы и операции множеств

## Скалярные подзапросы
```sql
SELECT id, (SELECT MAX(total) FROM orders) AS max_total
FROM orders;
```
- Возвращает одну строку/колонку; больше строк - ошибка.

## Табличные подзапросы
```sql
SELECT t.id, t.total
FROM (SELECT id, total FROM orders WHERE status = 'paid') t
WHERE t.total > 100;
```
- Можно использовать в `FROM` как временную таблицу.

## Коррелированные подзапросы
```sql
SELECT c.id, c.name,
       (SELECT COUNT(*) FROM orders o WHERE o.customer_id = c.id) AS orders_cnt
FROM customers c;
```
- Подзапрос ссылается на внешнюю строку.

## EXISTS / NOT EXISTS
```sql
SELECT c.id, c.name
FROM customers c
WHERE EXISTS (
  SELECT 1 FROM orders o WHERE o.customer_id = c.id AND o.status = 'paid'
);
```
- `EXISTS` возвращает TRUE, если подзапрос вернул хотя бы строку.
- Часто быстрее, чем `IN` для коррелированных условий.

## ANY/SOME/ALL
```sql
SELECT * FROM orders
WHERE total > ALL (SELECT limit FROM discounts);   -- больше любого значения (строже)

SELECT * FROM orders
WHERE status = ANY (ARRAY['new','paid']);         -- найдено совпадение
```
- `SOME` = `ANY`.

## Операции множеств
```sql
SELECT id FROM a
UNION            -- уникально
SELECT id FROM b;

SELECT id FROM a
UNION ALL        -- с дубликатами
SELECT id FROM b;

SELECT id FROM a
INTERSECT        -- пересечение
SELECT id FROM b;

SELECT id FROM a
EXCEPT           -- в a, но не в b
SELECT id FROM b;
```
- `ORDER BY` ставится один раз после всего выражения: `(query1 UNION query2) ORDER BY 1;`
- Колонки и их типы должны совпадать по позициям.

## LATERAL и подзапросы в FROM
Для топ-строки на каждую внешнюю:
```sql
SELECT u.id, x.last_order
FROM users u
LEFT JOIN LATERAL (
  SELECT o.id AS last_order
  FROM orders o
  WHERE o.user_id = u.id
  ORDER BY o.created_at DESC
  LIMIT 1
) x ON TRUE;
```
