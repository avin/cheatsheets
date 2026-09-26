-- ---------------------------------------------------
-- 📌 Ограничения: инварианты храните в базе
-- ---------------------------------------------------
CREATE TABLE bookings (
    id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    room_id bigint NOT NULL REFERENCES rooms(id) ON DELETE RESTRICT,
    guest_email text NOT NULL,
    starts_at timestamptz NOT NULL,
    ends_at timestamptz NOT NULL,
    CHECK (ends_at > starts_at),
    UNIQUE (room_id, starts_at)
);

-- CHECK с результатом NULL не запрещает строку: добавляйте NOT NULL отдельно.
-- В PostgreSQL UNIQUE допускает несколько NULL, если не указать NULLS NOT DISTINCT.

-- ---------------------------------------------------
-- 📌 Индекс под фильтр и сортировку
-- ---------------------------------------------------
CREATE INDEX orders_user_date_idx
ON orders (user_id, created_at DESC, id DESC);

CREATE INDEX orders_open_idx
ON orders (created_at DESC)
WHERE status = 'open';

CREATE INDEX users_email_lower_idx
ON users (lower(email));

CREATE INDEX orders_user_cover_idx
ON orders (user_id, created_at DESC) INCLUDE (status, total);

-- Порядок колонок соотносите с WHERE/ORDER BY конкретного запроса.
-- Частичный индекс используется, когда условие запроса подразумевает предикат индекса.
-- INCLUDE покрывает выборку, но увеличивает индекс и цену записи.

-- ---------------------------------------------------
-- 📌 Индексы без длительной блокировки записи
-- ---------------------------------------------------
CREATE INDEX CONCURRENTLY orders_status_idx ON orders (status);

-- CONCURRENTLY нельзя выполнять внутри явного блока транзакции.
-- Индекс не всегда ускоряет выборку: смотрите EXPLAIN и реальное число строк.
-- Для JSONB/массивов часто подходит GIN, для диапазонов — GiST.

-- ---------------------------------------------------
-- 📌 Запрет пересекающихся интервалов через EXCLUDE
-- ---------------------------------------------------
CREATE EXTENSION IF NOT EXISTS btree_gist;

ALTER TABLE bookings
ADD CONSTRAINT bookings_no_overlap EXCLUDE USING gist (
    room_id WITH =,
    tstzrange(starts_at, ends_at, '[)') WITH &&
);

-- [) разрешает бронирования, которые соприкасаются границами.
-- EXCLUDE полезен, когда проверка перед INSERT не защищает от гонки.
