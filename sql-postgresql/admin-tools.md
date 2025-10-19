# Администрирование и инструменты

## Пользователи и роли
```sql
CREATE ROLE app_user WITH LOGIN PASSWORD 'strongpass';
GRANT CONNECT ON DATABASE app_db TO app_user;
GRANT USAGE ON SCHEMA public TO app_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public TO app_user;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT, INSERT, UPDATE, DELETE ON TABLES TO app_user;
```
- Используйте **роль** для групп, **user** для логинов.
- `ALTER ROLE ... SET search_path` для окружения.

## Конфигурация (postgresql.conf)
- Основные параметры:
  - `shared_buffers`, `work_mem`, `maintenance_work_mem`.
  - `max_connections`, `effective_cache_size`.
  - `wal_level`, `max_wal_size`, `checkpoint_timeout`.
  - `log_statement`, `log_min_duration_statement`.
- Используйте PGTune/analyze workload для предложений.

## pg_hba.conf
- Настройки доступа (host, md5, scram-sha-256, cert).
- Ограничивайте доступ по IP, используйте SCRAM (`password_encryption=scram-sha-256`).

## Backup/Restore
- `pg_dump`, `pg_dumpall`, `pg_restore`.
- Base backup + WAL (Continuous Archiving) → `pg_basebackup`.
- Инкрементальные бэкапы через WAL (wal-g, Barman).

## Репликация
- Streaming replication (primary → standby).
- Hot standby: `hot_standby = on`.
- Logical replication: `CREATE PUBLICATION`, `CREATE SUBSCRIPTION`.

## Maintenance
- `VACUUM`, `VACUUM FULL`, `ANALYZE`.
- Autovacuum настройки: `autovacuum_naptime`, `autovacuum_vacuum_cost_limit`.
- `REINDEX CONCURRENTLY` для перестройки.

## Tools
- `psql`, `pgcli` (CLI с автокомплитом).
- GUI: pgAdmin, DataGrip, TablePlus.
- pg_stat_statements extension (query stats).
- Monitoring: Prometheus exporter (postgres_exporter), pganalyze, pgwatch2.

