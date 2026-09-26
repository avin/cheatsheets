-- ---------------------------------------------------
-- 📌 Генерация строк, UUID и случайная выборка
-- ---------------------------------------------------
SELECT n FROM generate_series(1, 10) AS numbers(n);
SELECT gen_random_uuid() AS id;
SELECT random() AS sample_between_zero_and_one;

-- random() подходит для выборки/демо, но не для секретов.
-- ORDER BY random() на большой таблице дорог: сортируются все кандидаты.

-- ---------------------------------------------------
-- 📌 Последовательности и identity
-- ---------------------------------------------------
CREATE TABLE invoices (
    id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    issued_at timestamptz NOT NULL DEFAULT now()
);

SELECT pg_get_serial_sequence('invoices', 'id') AS sequence_name;
-- Не вычисляйте новый id через max(id) + 1: конкурентные вставки столкнутся.

-- ---------------------------------------------------
-- 📌 Размеры и служебные представления
-- ---------------------------------------------------
SELECT pg_size_pretty(pg_total_relation_size('orders')) AS orders_with_indexes;

SELECT relname, n_live_tup, n_dead_tup, last_autoanalyze
FROM pg_stat_user_tables
ORDER BY n_dead_tup DESC
LIMIT 10;

SELECT pid, state, wait_event_type, query
FROM pg_stat_activity
WHERE state <> 'idle';

-- Статистика приблизительна; для анализа медленного запроса используйте EXPLAIN.

-- ---------------------------------------------------
-- 📌 Расширения и проверка версии
-- ---------------------------------------------------
SELECT current_setting('server_version') AS server_version;
SELECT extname, extversion FROM pg_extension ORDER BY extname;
CREATE EXTENSION IF NOT EXISTS pg_trgm;

-- Для CREATE EXTENSION нужны соответствующие права и доступность расширения.
