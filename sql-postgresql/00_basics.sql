-- ---------------------------------------------------
-- 📌 Таблица и типы: выбирайте тип под смысл данных
-- ---------------------------------------------------
CREATE TABLE app_user (
    id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    email text NOT NULL UNIQUE,
    display_name text NOT NULL,
    active boolean NOT NULL DEFAULT true,
    created_at timestamptz NOT NULL DEFAULT now(),
    profile jsonb NOT NULL DEFAULT '{}'::jsonb
);

-- timestamptz хранит момент времени; часовой пояс показа задаёт сессия.
-- Для денег с фиксированной точностью используйте numeric(p, s), не float.
-- Идентификаторы без кавычек приводятся к нижнему регистру: app_user проще "AppUser".

-- ---------------------------------------------------
-- 📌 Базовые операции и RETURNING
-- ---------------------------------------------------
INSERT INTO app_user (email, display_name)
VALUES ('ada@example.com', 'Ada')
RETURNING id, created_at;

SELECT id, email
FROM app_user
WHERE active IS TRUE
ORDER BY id
LIMIT 20;

UPDATE app_user
SET display_name = 'Ada L.'
WHERE email = 'ada@example.com'
RETURNING id, display_name;

DELETE FROM app_user
WHERE email = 'ada@example.com'
RETURNING id;

-- Для UPDATE и DELETE сначала проверьте условие через SELECT.

-- ---------------------------------------------------
-- 📌 NULL, приведение типов и полезные выражения
-- ---------------------------------------------------
SELECT
    NULL = NULL AS unknown_result,                 -- NULL, не true
    NULL IS NULL AS explicit_check,                -- true
    COALESCE(NULL::text, 'не задано') AS fallback,
    '42'::integer + 1 AS parsed_number,
    NULLIF(10, 10) AS empty_when_equal;

-- Не сравнивайте с NULL через = или <>; используйте IS [NOT] NULL.
-- Явное ::type полезно, когда литерал или параметр неоднозначен.

-- ---------------------------------------------------
-- 📌 Изменение схемы без потери данных
-- ---------------------------------------------------
ALTER TABLE app_user
ADD COLUMN IF NOT EXISTS locale text NOT NULL DEFAULT 'ru';

CREATE INDEX IF NOT EXISTS app_user_created_at_idx
ON app_user (created_at DESC);

-- DDL в PostgreSQL обычно можно выполнять в транзакции.
-- Перед миграцией большой таблицы оцените блокировки и время построения индекса.
