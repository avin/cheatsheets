# Функции и операторы (частое)

## Арифметика и сравнение
```sql
SELECT 1 + 2, 5 / 2, 5 / 2.0, 5 % 2, 2 ^ 3;
SELECT col1 = col2, col1 <> col2, col > 0;
```
- `/` с целыми - целочисленное; для дроби - хотя бы один операнд numeric/float.

## CASE / COALESCE / NULLIF
```sql
SELECT CASE WHEN score >= 90 THEN 'A' ELSE 'B' END AS grade,
       COALESCE(note, 'n/a') AS note_safe,
       NULLIF(a, b) AS a_unless_equal;
```

## Строки
```sql
SELECT concat('a','b'), 'a'||'b' AS cat;
SELECT upper('Привет'), lower('SQL'), initcap('hello world');
SELECT length('привет'), char_length('привет');   -- символы
SELECT trim(both ' .' FROM ' hi. '), ltrim('  x'), rtrim('y  ');
SELECT replace('abcabc','a','X');
SELECT substring('abcde', 2, 3);                  -- bcd
SELECT split_part('a,b,c', ',', 2);               -- b
```
- Сопоставление: `LIKE`, `ILIKE`, `~` (regex), `~*` (regex без регистра), `SIMILAR TO`.

## Регулярки (кратко)
```sql
SELECT 'abc' ~ '^a', regexp_replace('abc123', '\d+', '', 'g'), regexp_matches('a1b2', '([a-z])(\d)');
```
- Флаги: `i` (регистр), `g` (глобально), `m` (мультистрока).

## Даты/время
```sql
SELECT now(), CURRENT_DATE, CURRENT_TIMESTAMP;
SELECT date_trunc('day', now()), extract(year FROM now());
SELECT make_date(2025, 1, 31), make_timestamp(2025,1,31,12,0,0);
SELECT age('2025-01-31', '2024-12-31');          -- interval разница
```
- Округление дат: `date_trunc`. Извлечение частей: `extract(epoch FROM ts)`, `extract(dow FROM ts)`.

## Интервалы и арифметика
```sql
SELECT now() + INTERVAL '1 day', now() - INTERVAL '3 hours';
SELECT '2025-02-01'::date + 7;        -- дни
SELECT '12:00'::time + INTERVAL '30 min';
```

## Преобразование типов
```sql
SELECT '42'::int, CAST('3.14' AS numeric), col::text;
SELECT to_char(now(), 'YYYY-MM-DD'), to_date('2025-01-31','YYYY-MM-DD');
SELECT to_timestamp(1700000000);  -- из epoch секунд
```

## JSONB и массивы (подробнее в `10_json_array.md`)
```sql
SELECT data->'name', data->>'name', data#>>'{address,city}' FROM users;
SELECT arr[1], arr[1:3], arr || ARRAY[4,5] AS arr_cat FROM (VALUES (ARRAY[1,2,3])) v(arr);
```

## Прочее полезное
```sql
SELECT greatest(1,5,3), least(10,2,7);
SELECT width_bucket(15, 0, 100, 5);   -- в какой корзине 1..5
SELECT random();                      -- 0..1
```
