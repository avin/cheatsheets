-- ---------------------------------------------------
-- 📌 Значения по умолчанию и защита от деления на ноль
-- ---------------------------------------------------
SELECT COALESCE(nickname, display_name, 'Гость') AS label
FROM users;

SELECT revenue / NULLIF(quantity, 0) AS unit_revenue
FROM sales;

SELECT CASE WHEN status = 'paid' THEN total ELSE 0 END AS paid_amount
FROM orders;

-- COALESCE выбирает первое не-NULL значение; NULLIF превращает совпадение в NULL.

-- ---------------------------------------------------
-- 📌 Строки и регулярные выражения
-- ---------------------------------------------------
SELECT trim('  Ada  ') AS trimmed,
       lower('Example@Mail.COM') AS normalized,
       split_part('a:b:c', ':', 2) AS middle,
       concat_ws(' ', 'Ada', NULL, 'Lovelace') AS full_name;

SELECT email FROM users WHERE email ~* '^[a-z0-9._%+-]+@example\.com$';
SELECT regexp_replace('phone: 123-456', '[^0-9]', '', 'g');

-- Регулярка выше лишь пример фильтра; полноценную проверку адреса делайте на границе приложения.
-- Для поиска по подстроке и индексирования проверьте pg_trgm.

-- ---------------------------------------------------
-- 📌 Типы и сравнение с NULL
-- ---------------------------------------------------
SELECT '2026-09-26'::date AS day,
       CAST('17' AS integer) AS n,
       10 IS DISTINCT FROM NULL AS safely_different;

SELECT greatest(5, 9, 3), least(5, 9, 3);

-- Приводите тип на стороне параметра, а не индексируемой колонки,
-- если хотите сохранить возможность использовать обычный индекс.

-- ---------------------------------------------------
-- 📌 Полезные функции для безопасного вывода
-- ---------------------------------------------------
SELECT format('Заказ %s: %s', id, status) AS label FROM orders;
SELECT pg_typeof(now()) AS timestamp_type;
SELECT current_setting('TimeZone') AS session_timezone;

-- format('%I', name) экранирует идентификатор, '%L' — SQL-литерал.
-- Динамический SQL всё равно требует внимательной работы с параметрами.
