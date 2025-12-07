# SELECT и фильтрация

## Базовый SELECT
```sql
SELECT col1 AS c1, col2 + 1 AS c2_expr
FROM public.users u
WHERE u.active
```
- В списке можно выражения/функции; имя через `AS` или пробел.
- Без `FROM` допустимо: `SELECT 1;` (удобно для проверок).

## DISTINCT и DISTINCT ON
```sql
SELECT DISTINCT city FROM users;
SELECT DISTINCT ON (city) city, created_at
FROM users
ORDER BY city, created_at DESC;
```
- `DISTINCT` убирает дубликаты по всем выбранным колонкам.
- `DISTINCT ON (cols)` - оставить первую строку по набору, порядок задает `ORDER BY`.

## WHERE: сравнения и булева логика
```sql
SELECT * FROM orders
WHERE status = 'paid' AND total >= 100 AND NOT archived;
```
- Булевы колонки фильтруются напрямую (`WHERE active`).
- Приоритет: `()` > `NOT` > `AND` > `OR`.

## BETWEEN / IN / NOT
```sql
WHERE price BETWEEN 10 AND 20          -- включительно
WHERE id IN (1, 2, 3)                  -- равно любому из списка
WHERE status NOT IN ('new','blocked')
```
- `NOT IN` с NULL в списке даст пустой результат; избегай NULL в списках.

## NULL-проверки и эквивалентность
```sql
WHERE updated_at IS NULL
WHERE a IS NOT DISTINCT FROM b   -- TRUE даже для двух NULL
```
- `IS [NOT] DISTINCT FROM` - безопасное сравнение с NULL (альтернатива `=`/`<>`).

## LIKE / ILIKE / SIMILAR
```sql
WHERE name LIKE 'Jo%n'           -- % любая строка, _ один символ
WHERE name ILIKE '%ё%'           -- регистронезависимо
WHERE code NOT LIKE 'A\_%' ESCAPE '\'
```
- Для regex см. `SIMILAR TO` или `~`/`~*` (подробнее в функциях: [08_functions_ops.md](08_functions_ops.md)).

## CASE (ветвление)
```sql
SELECT CASE
         WHEN amount >= 1000 THEN 'vip'
         WHEN amount >= 100  THEN 'std'
         ELSE 'low'
       END AS tier
FROM customers;
```
- Есть форма `CASE expr WHEN value THEN ...`.
- Удобен в `SELECT` и `ORDER BY`; в `WHERE` - только предикаты.

## ORDER BY
```sql
ORDER BY created_at DESC, id ASC NULLS LAST
ORDER BY 1, 2 DESC               -- по позициям
```
- Можно сортировать по выражению: `ORDER BY price/NULLIF(qty,0)`.
- `NULLS FIRST|LAST` управляет местом NULL.

## LIMIT / OFFSET / FETCH
```sql
ORDER BY created_at DESC
LIMIT 10 OFFSET 20         -- сдвиг/страница
FETCH FIRST 5 ROWS ONLY    -- аналог LIMIT
```
- Для устойчивой пагинации всегда указывай `ORDER BY`.
- Быстрый скролл - через ключевую пагинацию (`WHERE (col, id) > (...)`), см. продвинутые техники.

## ANY/ALL (массивая/подзапросы)
```sql
WHERE 5 = ANY (ARRAY[2,5,7])      -- найдено совпадение
WHERE price > ALL (VALUES (10),(20),(30))
```
- Работает и с подзапросами: см. детали в [04_subqueries_sets.md](04_subqueries_sets.md).

## Пример комплексного фильтра
```sql
SELECT id, email, created_at,
       CASE WHEN last_login IS NULL THEN 'never' ELSE 'seen' END AS status
FROM users
WHERE role IN ('admin','manager')
  AND (created_at BETWEEN CURRENT_DATE - INTERVAL '30 days' AND CURRENT_DATE)
  AND email ILIKE '%@example.com'
ORDER BY created_at DESC
LIMIT 50;
```
