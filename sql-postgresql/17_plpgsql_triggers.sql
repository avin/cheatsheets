-- ---------------------------------------------------
-- 📌 Функция PL/pgSQL: проверка и возвращаемое значение
-- ---------------------------------------------------
CREATE OR REPLACE FUNCTION discounted_total(
    p_amount numeric, p_percent numeric
) RETURNS numeric
LANGUAGE plpgsql
IMMUTABLE
AS $$
BEGIN
    IF p_percent < 0 OR p_percent > 100 THEN
        RAISE EXCEPTION 'percent must be between 0 and 100'
            USING ERRCODE = '22023';
    END IF;

    RETURN round(p_amount * (100 - p_percent) / 100, 2);
END;
$$;

SELECT discounted_total(120, 15);
-- Для одного SELECT-выражения обычно хватит LANGUAGE sql без PL/pgSQL.

-- ---------------------------------------------------
-- 📌 Возврат набора строк и одноразовый DO-блок
-- ---------------------------------------------------
CREATE OR REPLACE FUNCTION recent_orders(p_user_id bigint)
RETURNS TABLE(order_id bigint, amount numeric)
LANGUAGE plpgsql
STABLE
AS $$
BEGIN
    RETURN QUERY
    SELECT o.id, o.total
    FROM orders AS o
    WHERE o.user_id = p_user_id
    ORDER BY o.created_at DESC
    LIMIT 10;
END;
$$;

DO $$
BEGIN
    RAISE NOTICE 'database: %', current_database();
END;
$$;

-- ---------------------------------------------------
-- 📌 Триггер аудита изменений
-- ---------------------------------------------------
CREATE TABLE user_audit (
    id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    user_id bigint NOT NULL,
    operation text NOT NULL,
    changed_at timestamptz NOT NULL DEFAULT now()
);

CREATE OR REPLACE FUNCTION audit_user_change()
RETURNS trigger
LANGUAGE plpgsql
AS $$
BEGIN
    IF TG_OP = 'DELETE' THEN
        INSERT INTO user_audit (user_id, operation) VALUES (OLD.id, TG_OP);
        RETURN OLD;
    END IF;

    INSERT INTO user_audit (user_id, operation) VALUES (NEW.id, TG_OP);
    RETURN NEW;
END;
$$;

CREATE TRIGGER users_audit
AFTER INSERT OR UPDATE OR DELETE ON users
FOR EACH ROW EXECUTE FUNCTION audit_user_change();

-- Триггер выполняется в транзакции исходной операции; тяжёлая работа замедлит запись.

-- ---------------------------------------------------
-- 📌 SECURITY DEFINER: зафиксируйте доверенный search_path
-- ---------------------------------------------------
CREATE OR REPLACE FUNCTION app.private_order_count()
RETURNS bigint
LANGUAGE sql
SECURITY DEFINER
SET search_path = pg_catalog, app, pg_temp
AS $$
    SELECT count(*) FROM app.private_orders;
$$;

REVOKE ALL ON FUNCTION app.private_order_count() FROM PUBLIC;
GRANT EXECUTE ON FUNCTION app.private_order_count() TO app_reader;

-- Владелец функции должен иметь только нужные права; схему app нельзя
-- отдавать на CREATE недоверенным ролям. Проверяйте RLS и контекст вызова.

-- ---------------------------------------------------
-- 📌 Процедура для явного вызова
-- ---------------------------------------------------
CREATE OR REPLACE PROCEDURE mark_old_orders()
LANGUAGE plpgsql
AS $$
BEGIN
    UPDATE orders
    SET status = 'archived'
    WHERE status = 'completed'
      AND created_at < now() - interval '1 year';
END;
$$;

CALL mark_old_orders();

-- ---------------------------------------------------
-- 📌 Event trigger: аудит DDL (требуются повышенные права)
-- ---------------------------------------------------
CREATE OR REPLACE FUNCTION log_ddl_tag()
RETURNS event_trigger
LANGUAGE plpgsql
AS $$
BEGIN
    RAISE LOG 'DDL command: %', TG_TAG;
END;
$$;

CREATE EVENT TRIGGER ddl_audit
ON ddl_command_end
EXECUTE FUNCTION log_ddl_tag();

-- Event trigger действует на всю базу и может повлиять на миграции.
