-- ---------------------------------------------------
-- 📌 Моменты времени и календарные даты
-- ---------------------------------------------------
SELECT now() AS transaction_time,
       current_date AS local_date,
       TIMESTAMPTZ '2026-09-26 12:00:00+03' AS instant;

-- timestamptz хранит момент; вывод зависит от TimeZone сессии.
-- date и timestamp без time zone нужны для календарной даты или местного времени.

-- ---------------------------------------------------
-- 📌 Показ момента в нужной зоне и преобразование местного времени
-- ---------------------------------------------------
SELECT created_at AT TIME ZONE 'Europe/Istanbul' AS local_wall_time
FROM orders;

SELECT TIMESTAMP '2026-09-26 09:00:00'
       AT TIME ZONE 'Europe/Istanbul' AS instant_from_local_time;

-- Результат первого выражения — timestamp, второго — timestamptz.
-- Для событий храните исходный момент как timestamptz.

-- ---------------------------------------------------
-- 📌 Интервалы, округление и границы периода
-- ---------------------------------------------------
SELECT date_trunc('day', now() AT TIME ZONE 'Europe/Istanbul') AS local_day_start;
SELECT now() + interval '7 days' AS next_week;
SELECT extract(epoch FROM (now() - created_at)) AS age_seconds FROM orders;

-- Полуоткрытый диапазон удобен для индекса и не теряет доли секунды:
SELECT id
FROM orders
WHERE created_at >= TIMESTAMPTZ '2026-09-01 00:00:00+03'
  AND created_at <  TIMESTAMPTZ '2026-10-01 00:00:00+03';

-- Не пишите created_at::date = ... для большой таблицы без нужного индекса.

-- ---------------------------------------------------
-- 📌 Календарь через generate_series
-- ---------------------------------------------------
SELECT day::date
FROM generate_series(
    DATE '2026-09-01',
    DATE '2026-09-30',
    INTERVAL '1 day'
) AS calendar(day);

SELECT name, utc_offset FROM pg_timezone_names WHERE name LIKE 'Europe/%';
-- Названия зон храните в формате IANA, не как фиксированное смещение.
