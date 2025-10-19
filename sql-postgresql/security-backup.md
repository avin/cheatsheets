# Безопасность и бэкапы

## Аутентификация и доступ
- Используйте SCRAM-SHA-256 (в Postgres 10+) — `password_encryption = scram-sha-256`.
- Настраивайте `pg_hba.conf`: минимум прав (`hostssl`, конкретные IP, `auth` method).
- Разделяйте роли: `app_read`, `app_write`, `admin`. Не используйте superuser для приложений.
- Включайте `log_connections`, `log_disconnections`, `log_failed_attempts`.

## Шифрование
- TLS (SSL) для подключения: `ssl = on`, `ssl_cert_file`, `ssl_key_file`.
- Client-side encryption (приложение). Для rest — Transparent Data Encryption (Postgres 16 preview, pgcrypto).

## Row-Level Security (RLS)
```sql
ALTER TABLE orders ENABLE ROW LEVEL SECURITY;
CREATE POLICY orders_policy
  ON orders
  FOR SELECT USING (customer_id = current_setting('app.current_user_id')::uuid);
```
- Устанавливайте контекст `SET app.current_user_id = '...'`.

## Audit
- `pgaudit` extension.
- pg_stat_statements + логирование медленных запросов.
- Мониторите DDL (event triggers, audit log).

## Backup стратегии
- **Logical**: `pg_dump` (per-db). Удобно для переносов, но долгие restore.
- **Physical**: base backup + WAL архив (point-in-time recovery).
- **Incremental**: wal-g, Barman, pgBackRest.
- Регулярно тестируйте restore (staging).
- Для критичных систем храните бэкапы в другом регионе/облаке.

## PITR (Point-In-Time Recovery)
- Включите `archive_mode = on`, `archive_command`.
- `restore_command` для standby.
- `recovery.signal` / `standby.signal` файлы (v12+).

## Disaster Recovery
- Планируйте RPO/RTO.
- Репликация + автоматическое переключение (repmgr, Patroni).
- Документируйте процедуры переключения, контакты ответственных.

