-- ---------------------------------------------------
-- 📌 Группировка и условные агрегаты
-- ---------------------------------------------------
SELECT user_id,
       count(*) AS orders_count,
       count(*) FILTER (WHERE status = 'paid') AS paid_count,
       sum(total) FILTER (WHERE status = 'paid') AS paid_total,
       avg(total) AS average_total
FROM orders
GROUP BY user_id
HAVING count(*) >= 3;

-- count(*) считает строки, count(column) пропускает NULL.
-- WHERE фильтрует до группировки, HAVING — после неё.
-- sum/avg для пустого набора дают NULL; при необходимости COALESCE(..., 0).

-- ---------------------------------------------------
-- 📌 Группировка по выражению и число разных значений
-- ---------------------------------------------------
SELECT date_trunc('month', created_at) AS month,
       count(*) AS orders_count,
       count(DISTINCT user_id) AS buyers
FROM orders
GROUP BY 1
ORDER BY 1;

-- Для отчётов по местному календарю сначала определите нужный часовой пояс.

-- ---------------------------------------------------
-- 📌 Несколько уровней итогов одним запросом
-- ---------------------------------------------------
SELECT region, status, sum(total) AS amount,
       grouping(region) AS region_is_total,
       grouping(status) AS status_is_total
FROM orders
GROUP BY ROLLUP (region, status)
ORDER BY region NULLS LAST, status NULLS LAST;

-- GROUPING отличает строку итога от обычной группы со значением NULL.
-- GROUPING SETS ((region), (status), ()) задаёт произвольный набор итогов.

-- ---------------------------------------------------
-- 📌 Упорядоченная агрегация
-- ---------------------------------------------------
SELECT user_id,
       string_agg(DISTINCT status, ', ' ORDER BY status) AS statuses,
       array_agg(id ORDER BY created_at DESC, id DESC) AS order_ids
FROM orders
GROUP BY user_id;

-- Порядок в array_agg/string_agg задавайте внутри агрегата.
