# Даты, время, таймзоны

## Типы
- `date`, `time [with[out] time zone]`, `timestamp [with[out] time zone]` (`timestamptz`).
- Всегда храни в `timestamptz`, локализуй при выводе.

## AT TIME ZONE
```sql
SELECT TIMESTAMP '2025-01-31 12:00' AT TIME ZONE 'Europe/Moscow';       -- naive -> tz
SELECT TIMESTAMPTZ '2025-01-31 12:00+03' AT TIME ZONE 'UTC';             -- tz -> сдвиг в местное время
SELECT now() AT TIME ZONE 'America/New_York';
```

## Извлечение частей
```sql
SELECT extract(year FROM now()),
       extract(month FROM now()),
       extract(dow FROM now()),      -- 0 = воскресенье
       date_part('week', now());
```

## Округление и обрезка
```sql
SELECT date_trunc('day', now());     -- начало дня
SELECT date_trunc('week', now());    -- начало недели (понедельник)
SELECT date_trunc('month', now());
```

## Интервалы и сдвиги
```sql
SELECT now() + INTERVAL '2 days';
SELECT CURRENT_DATE + 7;             -- плюс дней
SELECT make_interval(days => 10, hours => 3);
```

## Разница дат/времени
```sql
SELECT age(TIMESTAMPTZ '2025-02-01', TIMESTAMPTZ '2025-01-31');  -- interval
SELECT EXTRACT(EPOCH FROM (now() - INTERVAL '1 day'));           -- сек
```

## Календарные трюки
```sql
-- последний день месяца
SELECT (date_trunc('month', now()) + INTERVAL '1 month - 1 day')::date;

-- первая дата недели (понедельник)
SELECT date_trunc('week', CURRENT_DATE)::date;

-- номер квартала
SELECT extract(quarter FROM now());
```

## Списки таймзон
```sql
SELECT * FROM pg_timezone_names WHERE name LIKE 'Europe/%';
```
