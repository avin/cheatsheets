-- ---------------------------------------------------
-- 📌 Транзакция и точка сохранения
-- ---------------------------------------------------
BEGIN;
UPDATE accounts SET balance = balance - 100 WHERE id = 1;
UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;

BEGIN;
SAVEPOINT before_optional_step;
UPDATE accounts SET balance = balance - 10 WHERE id = 3;
-- Если шаг не нужен или завершился ошибкой, отмените только его:
ROLLBACK TO SAVEPOINT before_optional_step;
COMMIT;

-- При ошибке транзакция остаётся прерванной до ROLLBACK или ROLLBACK TO SAVEPOINT.

-- ---------------------------------------------------
-- 📌 Изоляция и блокировка строки
-- ---------------------------------------------------
BEGIN ISOLATION LEVEL SERIALIZABLE;
SELECT balance FROM accounts WHERE id = 1 FOR UPDATE;
UPDATE accounts SET balance = balance - 100 WHERE id = 1;
COMMIT;

-- READ COMMITTED — уровень по умолчанию. SERIALIZABLE может вернуть
-- SQLSTATE 40001: повторяйте всю транзакцию, а не один оператор.
-- Блокируйте строки в одинаковом порядке, чтобы снизить риск deadlock.

-- ---------------------------------------------------
-- 📌 Забрать задачу из очереди без ожидания чужой блокировки
-- ---------------------------------------------------
WITH next_job AS (
    SELECT id
    FROM jobs
    WHERE status = 'ready'
    ORDER BY priority DESC, id
    FOR UPDATE SKIP LOCKED
    LIMIT 1
)
UPDATE jobs AS j
SET status = 'running'
FROM next_job
WHERE j.id = next_job.id
RETURNING j.*;

-- Делайте это внутри транзакции вместе с записью владельца/аренды задачи.
-- SKIP LOCKED подходит для очереди, но не для согласованного общего списка.

-- ---------------------------------------------------
-- 📌 Advisory lock и диагностика ожидания
-- ---------------------------------------------------
BEGIN;
SELECT pg_try_advisory_xact_lock(42) AS acquired;
-- Выполняйте защищённую работу только если acquired = true.
COMMIT; -- transaction-level lock освобождается автоматически

SELECT pid, wait_event_type, wait_event, state, query
FROM pg_stat_activity
WHERE wait_event_type = 'Lock';

-- Не удерживайте транзакцию открытой во время сетевого вызова.

-- ---------------------------------------------------
-- 📌 Явная блокировка таблицы: редкий инструмент для миграции
-- ---------------------------------------------------
BEGIN;
LOCK TABLE accounts IN SHARE ROW EXCLUSIVE MODE;
-- Короткая операция, которой нужен устойчивый набор строк.
COMMIT;

-- Долгая явная блокировка задерживает другие сессии; задавайте lock_timeout.
