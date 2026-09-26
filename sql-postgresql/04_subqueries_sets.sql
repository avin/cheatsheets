-- ---------------------------------------------------
-- 📌 EXISTS и NOT EXISTS: проверка наличия без размножения строк
-- ---------------------------------------------------
SELECT u.id, u.email
FROM users AS u
WHERE EXISTS (
    SELECT 1 FROM orders AS o
    WHERE o.user_id = u.id AND o.status = 'paid'
);

SELECT u.id
FROM users AS u
WHERE NOT EXISTS (
    SELECT 1 FROM orders AS o WHERE o.user_id = u.id
);

-- Для отсутствия связанных строк NOT EXISTS безопаснее NOT IN при NULL.

-- ---------------------------------------------------
-- 📌 Скалярный и табличный подзапрос
-- ---------------------------------------------------
SELECT p.id, p.price
FROM products AS p
WHERE p.price > (SELECT avg(price) FROM products);

SELECT ranked.user_id, ranked.total
FROM (
    SELECT user_id, sum(total) AS total
    FROM orders GROUP BY user_id
) AS ranked
WHERE ranked.total >= 1000;

-- Скалярный подзапрос обязан вернуть не более одной строки.

-- ---------------------------------------------------
-- 📌 ANY/ALL и операции множеств
-- ---------------------------------------------------
SELECT id FROM products
WHERE price > ALL (SELECT price FROM products WHERE category = 'budget');

SELECT email FROM newsletter_subscribers
INTERSECT
SELECT email FROM users;

SELECT email FROM newsletter_subscribers
EXCEPT
SELECT email FROM bounced_addresses;

SELECT email FROM users
UNION ALL
SELECT email FROM guest_users;

-- UNION/INTERSECT/EXCEPT без ALL удаляют дубликаты; UNION ALL сохраняет их.
-- В ветках операции должны совпадать количество колонок и совместимость типов.

-- ---------------------------------------------------
-- 📌 LATERAL: коррелированная таблица в FROM
-- ---------------------------------------------------
SELECT u.id, recent.order_id
FROM users AS u
CROSS JOIN LATERAL (
    SELECT o.id AS order_id
    FROM orders AS o
    WHERE o.user_id = u.id
    ORDER BY o.created_at DESC
    LIMIT 2
) AS recent;

-- CROSS JOIN LATERAL исключит пользователя без заказов;
-- LEFT JOIN LATERAL (...) ON true сохранит его.
