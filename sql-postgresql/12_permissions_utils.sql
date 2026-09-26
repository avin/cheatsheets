-- ---------------------------------------------------
-- 📌 Роли и минимальные привилегии
-- ---------------------------------------------------
CREATE ROLE app_reader LOGIN;
CREATE ROLE app_writer LOGIN;
CREATE SCHEMA IF NOT EXISTS app;

GRANT CONNECT ON DATABASE shop TO app_reader, app_writer;
GRANT USAGE ON SCHEMA app TO app_reader, app_writer;
GRANT SELECT ON ALL TABLES IN SCHEMA app TO app_reader;
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA app TO app_writer;

-- ALL TABLES означает существующие таблицы; будущие настраиваются отдельно.
ALTER DEFAULT PRIVILEGES FOR ROLE migration_owner IN SCHEMA app
GRANT SELECT ON TABLES TO app_reader;

-- Настройка default privileges действует только для объектов указанного создателя.
-- Пароль и секреты задавайте через менеджер секретов, не записывайте в шпаргалку.

-- ---------------------------------------------------
-- 📌 Схема и search_path
-- ---------------------------------------------------
ALTER ROLE app_reader SET search_path = app, pg_catalog;
SELECT current_schema(), current_schemas(true);

-- В SECURITY DEFINER задавайте доверенный search_path внутри функции.
-- Не давайте недоверенным ролям CREATE в схемах из search_path.

-- ---------------------------------------------------
-- 📌 Row-Level Security: защита строк по tenant_id
-- ---------------------------------------------------
ALTER TABLE app.documents ENABLE ROW LEVEL SECURITY;

CREATE POLICY documents_tenant ON app.documents
USING (tenant_id = nullif(current_setting('app.tenant_id', true), '')::bigint)
WITH CHECK (tenant_id = nullif(current_setting('app.tenant_id', true), '')::bigint);

BEGIN;
SET LOCAL app.tenant_id = '42';
SELECT id, title FROM app.documents;
COMMIT;

-- Политики не заменяют GRANT. Владелец таблицы обычно обходит RLS,
-- если не включить FORCE ROW LEVEL SECURITY; роль с BYPASSRLS тоже обходит.
-- Значение app.tenant_id может менять клиент с SQL-доступом: для недоверенного
-- прямого доступа нужна другая схема аутентификации и передачи tenant_id.

-- ---------------------------------------------------
-- 📌 Проверка текущих прав
-- ---------------------------------------------------
SELECT current_user,
       has_table_privilege(current_user, 'app.documents', 'SELECT') AS can_read;

SELECT grantee, privilege_type
FROM information_schema.role_table_grants
WHERE table_schema = 'app' AND table_name = 'documents';
