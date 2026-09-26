-- ---------------------------------------------------
-- 📌 Временная таблица для промежуточных данных
-- ---------------------------------------------------
BEGIN;
CREATE TEMP TABLE import_ids (
    id bigint PRIMARY KEY
) ON COMMIT DROP;

INSERT INTO import_ids (id) VALUES (1), (2), (3);
ANALYZE import_ids;
SELECT id FROM import_ids ORDER BY id;
COMMIT;

-- ON COMMIT DROP удаляет таблицу при COMMIT.
-- Для крупной временной таблицы ANALYZE помогает планировщику.

-- ---------------------------------------------------
-- 📌 CREATE TABLE AS и представления
-- ---------------------------------------------------
CREATE TABLE monthly_sales AS
SELECT date_trunc('month', created_at) AS month, sum(total) AS amount
FROM orders
GROUP BY 1;

CREATE OR REPLACE VIEW active_users AS
SELECT id, email FROM users WHERE active;

CREATE MATERIALIZED VIEW sales_by_user AS
SELECT user_id, sum(total) AS amount FROM orders GROUP BY user_id;

CREATE UNIQUE INDEX sales_by_user_user_id_idx ON sales_by_user (user_id);
REFRESH MATERIALIZED VIEW CONCURRENTLY sales_by_user;

-- Materialized view хранит снимок данных; REFRESH обновляет его.
-- CONCURRENTLY требует подходящий UNIQUE индекс и не работает для первого наполнения.

-- ---------------------------------------------------
-- 📌 Секционирование по диапазону дат
-- ---------------------------------------------------
CREATE TABLE events (
    id bigint NOT NULL,
    occurred_at timestamptz NOT NULL,
    payload jsonb NOT NULL,
    PRIMARY KEY (id, occurred_at)
) PARTITION BY RANGE (occurred_at);

CREATE TABLE events_2026_09 PARTITION OF events
FOR VALUES FROM ('2026-09-01 00:00+00') TO ('2026-10-01 00:00+00');

-- Создавайте будущие секции заранее; без подходящей секции INSERT завершится ошибкой.
-- Уникальное ограничение секционированной таблицы должно включать ключ секционирования.

-- ---------------------------------------------------
-- 📌 Серия строк как источник таблицы
-- ---------------------------------------------------
SELECT day::date
FROM generate_series(
    DATE '2026-09-01', DATE '2026-09-07', INTERVAL '1 day'
) AS dates(day);
