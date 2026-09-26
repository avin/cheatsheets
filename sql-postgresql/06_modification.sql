-- ---------------------------------------------------
-- 📌 INSERT и атомарный upsert
-- ---------------------------------------------------
INSERT INTO users (email, display_name)
VALUES ('ada@example.com', 'Ada'), ('lin@example.com', 'Lin')
RETURNING id, email;

INSERT INTO users (email, display_name)
VALUES ('ada@example.com', 'Ada Lovelace')
ON CONFLICT (email) DO UPDATE
SET display_name = EXCLUDED.display_name
RETURNING id, display_name;

-- ON CONFLICT требует уникального ограничения/индекса по ключу конфликта.
-- EXCLUDED — предложенная к вставке строка.

-- ---------------------------------------------------
-- 📌 UPDATE/DELETE с ограничением по условию
-- ---------------------------------------------------
UPDATE orders AS o
SET status = 'cancelled'
WHERE o.status = 'new'
  AND o.created_at < now() - interval '30 days'
RETURNING o.id;

DELETE FROM sessions
WHERE expires_at < now()
RETURNING user_id;

-- Перед массовой модификацией выполните SELECT с тем же WHERE.
-- RETURNING показывает реально изменённые строки.

-- ---------------------------------------------------
-- 📌 UPDATE FROM и перенос данных через CTE
-- ---------------------------------------------------
UPDATE products AS p
SET price = changes.new_price
FROM (VALUES (1, 19.90), (2, 29.90)) AS changes(id, new_price)
WHERE p.id = changes.id
RETURNING p.id, p.price;

WITH moved AS (
    DELETE FROM queue
    WHERE created_at < now() - interval '7 days'
    RETURNING id, payload
)
INSERT INTO queue_archive (id, payload)
SELECT id, payload FROM moved;

-- В UPDATE FROM обеспечьте максимум одну строку источника на целевую строку.

-- ---------------------------------------------------
-- 📌 COPY: серверный импорт/экспорт
-- ---------------------------------------------------
COPY import_stage (email, display_name)
FROM '/srv/import/users.csv'
WITH (FORMAT csv, HEADER true);

-- COPY с файловым путём читает файл на сервере и требует особых прав.
-- Для файла на своей машине используйте psql \copy (см. 18_psql_workflow.md).
