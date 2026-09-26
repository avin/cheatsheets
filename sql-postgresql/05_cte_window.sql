-- ---------------------------------------------------
-- 📌 CTE: дайте имя этапу запроса
-- ---------------------------------------------------
WITH paid AS (
    SELECT user_id, total FROM orders WHERE status = 'paid'
)
SELECT user_id, sum(total) AS paid_total
FROM paid GROUP BY user_id;

-- CTE улучшает чтение сложного запроса, но не гарантирует материализацию.
-- MATERIALIZED фиксирует отдельное вычисление; применяйте после EXPLAIN.

-- ---------------------------------------------------
-- 📌 Рекурсивный CTE: дерево подчинения
-- ---------------------------------------------------
WITH RECURSIVE team AS (
    SELECT id, manager_id, name, 0 AS depth
    FROM employees WHERE id = 42
  UNION ALL
    SELECT e.id, e.manager_id, e.name, team.depth + 1
    FROM employees AS e
    JOIN team ON e.manager_id = team.id
    WHERE team.depth < 20
)
SELECT id, name, depth FROM team ORDER BY depth, id;

-- Лимит глубины защищает от бесконечного обхода при ошибочных циклах.

-- ---------------------------------------------------
-- 📌 Ранжирование и top-N в каждой группе
-- ---------------------------------------------------
SELECT id, user_id, total,
       row_number() OVER (PARTITION BY user_id ORDER BY total DESC, id) AS n,
       rank() OVER (PARTITION BY user_id ORDER BY total DESC) AS rank_with_ties
FROM orders;

WITH ranked AS (
    SELECT o.*,
           row_number() OVER (
               PARTITION BY user_id ORDER BY created_at DESC, id DESC
           ) AS rn
    FROM orders AS o
)
SELECT id, user_id, created_at FROM ranked WHERE rn <= 3;

-- row_number уникален, rank оставляет пропуски при равенстве значений.

-- ---------------------------------------------------
-- 📌 Оконная сумма, предыдущая строка и фрейм
-- ---------------------------------------------------
SELECT id, user_id, created_at, total,
       sum(total) OVER (
           PARTITION BY user_id ORDER BY created_at, id
           ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
       ) AS running_total,
       lag(total) OVER (
           PARTITION BY user_id ORDER BY created_at, id
       ) AS previous_total,
       last_value(total) OVER (
           PARTITION BY user_id ORDER BY created_at, id
           ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING
       ) AS final_total
FROM orders;

-- Без явного фрейма last_value часто видит только текущую группу равных строк.
