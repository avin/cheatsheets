-- ---------------------------------------------------
-- 📌 JSONB: извлечение и проверка ключа
-- ---------------------------------------------------
SELECT id,
       profile -> 'settings' AS settings_json,
       profile #>> '{settings,theme}' AS theme_text
FROM users
WHERE profile ? 'settings';

-- -> возвращает jsonb, ->> и #>> возвращают text.
-- Отсутствующий путь обычно даёт NULL, а не ошибку.

-- ---------------------------------------------------
-- 📌 Фильтрация и обновление документа
-- ---------------------------------------------------
SELECT id FROM users
WHERE profile @> '{"plan":"pro"}'::jsonb;

SELECT id FROM users
WHERE jsonb_path_exists(profile, '$.tags[*] ? (@ == "vip")');

UPDATE users
SET profile = jsonb_set(
    profile, '{settings,theme}', '"dark"'::jsonb, true
)
WHERE id = 42
RETURNING profile;

-- jsonb_set не создаёт отсутствующие промежуточные объекты.
-- Часто фильтруемые поля лучше вынести в обычные колонки.

-- ---------------------------------------------------
-- 📌 Построение JSON для ответа или экспорта
-- ---------------------------------------------------
SELECT jsonb_build_object(
    'id', u.id,
    'email', u.email,
    'orders', COALESCE(
        jsonb_agg(o.id ORDER BY o.created_at) FILTER (WHERE o.id IS NOT NULL),
        '[]'::jsonb
    )
) AS user_json
FROM users AS u
LEFT JOIN orders AS o ON o.user_id = u.id
GROUP BY u.id, u.email;

-- ---------------------------------------------------
-- 📌 Массивы и индексация
-- ---------------------------------------------------
SELECT ARRAY['rust', 'sql'] @> ARRAY['sql'] AS contains_tag;
SELECT id FROM posts WHERE tags && ARRAY['postgres', 'database'];
SELECT id, unnest(tags) AS tag FROM posts;

CREATE INDEX users_profile_gin_idx ON users USING gin (profile);
CREATE INDEX posts_tags_gin_idx ON posts USING gin (tags);
-- GIN ускоряет подходящие операторы, но увеличивает стоимость записи.
