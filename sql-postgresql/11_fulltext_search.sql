-- ---------------------------------------------------
-- 📌 Документ и запрос на одном языке
-- ---------------------------------------------------
SELECT to_tsvector('russian', 'Быстрый поиск по документам')
       @@ plainto_tsquery('russian', 'искать документ') AS matches;

-- Конфигурация влияет на разбор слов и стоп-слова.
-- plainto_tsquery безопасно превращает обычный пользовательский ввод в запрос.

-- ---------------------------------------------------
-- 📌 Поиск, ранжирование и подсветка
-- ---------------------------------------------------
WITH q AS (
    SELECT websearch_to_tsquery('russian', 'база данных') AS query
)
SELECT a.id, a.title,
       ts_rank(
           to_tsvector('russian', coalesce(a.title, '') || ' ' || coalesce(a.body, '')),
           q.query
       ) AS rank,
       ts_headline('russian', a.body, q.query) AS excerpt
FROM articles AS a
CROSS JOIN q
WHERE to_tsvector('russian', coalesce(a.title, '') || ' ' || coalesce(a.body, ''))
      @@ q.query
ORDER BY rank DESC, a.id DESC
LIMIT 20;

-- Не выводите HTML из ts_headline без экранирования/санитизации.

-- ---------------------------------------------------
-- 📌 Индекс для повторяющегося поиска
-- ---------------------------------------------------
CREATE INDEX articles_search_idx
ON articles USING gin (
    to_tsvector('russian', coalesce(title, '') || ' ' || coalesce(body, ''))
);

-- Выражение в WHERE должно соответствовать индексному выражению.
-- Для другого языка и словаря создайте подходящую конфигурацию и индекс.

-- ---------------------------------------------------
-- 📌 Фразовый и явный запрос
-- ---------------------------------------------------
SELECT phraseto_tsquery('russian', 'база данных');
SELECT to_tsquery('russian', 'баз:* & данн:*');
-- to_tsquery принимает синтаксис операторов; не подставляйте в него сырой ввод.
