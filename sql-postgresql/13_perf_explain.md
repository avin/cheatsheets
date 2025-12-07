# Производительность и EXPLAIN

## EXPLAIN
```sql
EXPLAIN SELECT * FROM orders WHERE status = 'new';
EXPLAIN (ANALYZE, BUFFERS) SELECT * FROM orders WHERE status = 'new';
```
- `ANALYZE` выполняет запрос, дает фактические строки/время; `BUFFERS` - кеш/диск.

## Типы сканов (частое)
- `Seq Scan` - полный просмотр таблицы.
- `Index Scan` - поиск по индексу, выбор строк.
- `Index Only Scan` - без чтения таблицы (если все в индексе, видимость через VM).
- `Bitmap Index/Heap Scan` - битовые операции для больших диапазонов.

## Соединения
- `Nested Loop` - хорошо для точечных/малых наборов.
- `Hash Join` - эффективно для равенства/больших объемов; требует `work_mem`.
- `Merge Join` - отсортированные входы.

## Актуальность статистики
```sql
ANALYZE orders;
VACUUM (ANALYZE) orders;
```
- После больших вставок/удалений обновляй статистику.

## Индексы: проверки
- Используй `EXPLAIN` и `pg_stat_user_indexes` для видимости:
```sql
SELECT relname, indexrelname, idx_scan
FROM pg_stat_user_indexes
WHERE schemaname = 'public';
```
- Частичный/выражение индекс - если запрос совпадает с условием/выражением.

## Настройки, которые часто влияют
- `work_mem` - для сортировок/хешей (per operation).
- `effective_cache_size` - оценка кеша ОС.
- `shared_buffers` - кеш postgres.
- `random_page_cost`, `seq_page_cost` - стоимость IO.
- `enable_seqscan|indexscan|bitmapscan` можно временно выключить для диагностики плана.

## Частые проблемы
- Фильтр по колонке без индекса -> Seq Scan.
- Функция в условии без соответствующего индекс-выражения.
- `OR` без перекрестных индексов -> Seq Scan; иногда помогает `UNION ALL`.
- Смещение пагинации `OFFSET` большое - дорогой skip; используйте keyset pagination.

## Обслуживание таблиц
- `VACUUM (VERBOSE)` / `VACUUM FULL` (блокирует) / `ANALYZE` - очистка мертвых строк и статистика.
- `REINDEX TABLE tbl;` при bloat индексов; `CLUSTER tbl USING idx;` для физической сортировки.
- Мониторинг автоочистки: `SELECT relname, last_autovacuum FROM pg_stat_all_tables ORDER BY last_autovacuum NULLS FIRST;`

## Поиск горячих запросов
```sql
CREATE EXTENSION IF NOT EXISTS pg_stat_statements;
SELECT query, calls, mean_exec_time, rows
FROM pg_stat_statements
ORDER BY mean_exec_time DESC
LIMIT 10;
```

## Как читать план кратко
- Смотри сверху вниз: порядок исполнения слева направо.
- `Actual vs Planned Rows` - несоответствие = плохая статистика.
- `Recheck Cond` в `Bitmap Heap Scan` - пост-верификация.
