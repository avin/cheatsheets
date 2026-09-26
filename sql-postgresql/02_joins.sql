-- ---------------------------------------------------
-- 📌 INNER JOIN и LEFT JOIN
-- ---------------------------------------------------
SELECT o.id, u.email
FROM orders AS o
JOIN users AS u ON u.id = o.user_id;

SELECT u.id, o.id AS order_id
FROM users AS u
LEFT JOIN orders AS o ON o.user_id = u.id;
-- LEFT JOIN сохраняет пользователей без заказов; поля o будут NULL.

-- Фильтр правой таблицы в WHERE уберёт строки без совпадения:
SELECT u.id, o.id
FROM users AS u
LEFT JOIN orders AS o
  ON o.user_id = u.id AND o.status = 'paid';

-- ---------------------------------------------------
-- 📌 USING, самосоединение и декартово произведение
-- ---------------------------------------------------
SELECT * FROM order_items JOIN products USING (product_id);
-- USING объединяет одноимённые колонки в результате; NATURAL JOIN
-- не используйте: новая одноимённая колонка незаметно меняет условие.

SELECT employee.name, manager.name AS manager_name
FROM employees AS employee
LEFT JOIN employees AS manager ON manager.id = employee.manager_id;

SELECT size.name, color.name
FROM sizes AS size
CROSS JOIN colors AS color;
-- CROSS JOIN создаёт |sizes| × |colors| строк.

-- ---------------------------------------------------
-- 📌 LATERAL: лучший дочерний ряд для каждой строки
-- ---------------------------------------------------
SELECT u.id, latest.id AS last_order_id, latest.total
FROM users AS u
LEFT JOIN LATERAL (
    SELECT o.id, o.total
    FROM orders AS o
    WHERE o.user_id = u.id
    ORDER BY o.created_at DESC, o.id DESC
    LIMIT 1
) AS latest ON true;

-- Индекс orders(user_id, created_at DESC, id DESC) помогает такому поиску.
-- Для только отсутствующих строк обычно яснее NOT EXISTS.
