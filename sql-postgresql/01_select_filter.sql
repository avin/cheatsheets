-- ---------------------------------------------------
-- 📌 SELECT, вычисляемые поля и DISTINCT
-- ---------------------------------------------------
SELECT u.id, u.email, lower(u.email) AS normalized_email
FROM users AS u
WHERE u.active
ORDER BY u.id;

SELECT DISTINCT city FROM users;

-- DISTINCT ON оставляет первую строку в каждой группе по ORDER BY.
SELECT DISTINCT ON (user_id) user_id, status, created_at
FROM orders
ORDER BY user_id, created_at DESC, id DESC;

-- ---------------------------------------------------
-- 📌 Фильтры с NULL и набором значений
-- ---------------------------------------------------
SELECT id
FROM orders
WHERE status IN ('paid', 'shipped')
  AND total BETWEEN 100 AND 500
  AND cancelled_at IS NULL;

SELECT id FROM users WHERE city IS DISTINCT FROM 'Paris';
-- IS DISTINCT FROM сравнивает NULL как отдельное значение.
-- NOT IN с NULL внутри списка может вернуть UNKNOWN; для подзапроса берите NOT EXISTS.

SELECT id
FROM users
WHERE email ILIKE '%@example.com'
  AND code LIKE 'A\_%' ESCAPE '\';
-- В LIKE: % — любое число символов, _ — ровно один.

-- ---------------------------------------------------
-- 📌 CASE, сортировка и предсказуемая пагинация
-- ---------------------------------------------------
SELECT id, total,
       CASE WHEN total >= 1000 THEN 'large'
            WHEN total >= 100 THEN 'medium'
            ELSE 'small' END AS tier
FROM orders
ORDER BY created_at DESC, id DESC NULLS LAST
LIMIT 20;

-- OFFSET прост, но на глубоких страницах дорог и нестабилен при новых записях.
-- Для ленты используйте курсор и тот же порядок полей:
SELECT id, created_at
FROM orders
WHERE (created_at, id) < (TIMESTAMPTZ '2026-01-01 12:00+00', 500)
ORDER BY created_at DESC, id DESC
LIMIT 20;

-- ---------------------------------------------------
-- 📌 ANY/ALL и массив значений
-- ---------------------------------------------------
SELECT id FROM orders WHERE status = ANY (ARRAY['paid', 'shipped']);
SELECT id FROM products WHERE price > ALL (ARRAY[10, 20, 30]);

-- Для пустого массива ANY возвращает false, ALL — true.
