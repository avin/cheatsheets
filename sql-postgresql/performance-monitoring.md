# Оптимизация и мониторинг

## Query tuning
- Используйте `EXPLAIN` / `EXPLAIN ANALYZE` для планов.
- Смотрите на `Seq Scan`, `Index Scan`, `Filter`, `rows`, `loops`.
- Добавляйте индексы при необходимости, избегайте `SELECT *`.
- Профилируйте тяжелые запросы: pg_stat_statements, auto_explain.

## Планировщик
- Статистика в `pg_statistic` обновляется `ANALYZE`.
- При неточных планах — `ANALYZE table`, `ALTER TABLE ... ALTER COLUMN ... SET STATISTICS` (увеличьте).
- Кост-модели: `random_page_cost`, `cpu_operator_cost`.

## Метрики
- `pg_stat_activity` — текущие запросы.
- `pg_stat_database`, `pg_stat_user_tables`, `pg_stat_user_indexes`.
- `pg_locks` — блокировки.
- `pg_stat_replication` — статус реплик.

## Vacuum & Bloat
- `VACUUM ANALYZE` регулярно.
- Проверяйте bloat: `pgstattuple`, `pg_bloat_check`.
- Настраивайте autovacuum (`autovacuum_work_mem`, `autovacuum_vacuum_scale_factor`).

## Caching
- Connection pooling: PgBouncer (transaction pooling) или Pgpool.
- Prepared statements (`PREPARE / EXECUTE`).
- Локальный cache: Redis/Cache сервис.

## Partitioning
- `range partitioning` по дате (`PARTITION BY RANGE (created_at)`).
- `list partitioning` по региону.
- Поддерживайте `default` partition.

## Extensions
- `pg_stat_statements`, `auto_explain`, `pg_trgm`, `HypoPG` (что если индекс).

