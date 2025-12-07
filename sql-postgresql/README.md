# PostgreSQL SQL шпаргалки

Мини-набор файлов с короткими примерами. Держи весь SQL под рукой; подробности и примеры внутри соответствующих файлов.

## Оглавление
### [00_basics.md](00_basics.md)
Подключение, базовый синтаксис, типы данных, NULL/TRUE/FALSE, алиасы.

### [01_select_filter.md](01_select_filter.md)
`SELECT`, `DISTINCT`, фильтрация (`WHERE`, `IN`, `BETWEEN`, `LIKE`, `NULL`, `CASE`), сортировка и лимиты.

### [02_joins.md](02_joins.md)
`INNER|LEFT|RIGHT|FULL JOIN`, `CROSS JOIN`, `USING`, самосоединения.

### [03_aggregates_grouping.md](03_aggregates_grouping.md)
агрегаты, `GROUP BY`, `HAVING`, `GROUPING SETS`, `ROLLUP`, `CUBE`.

### [04_subqueries_sets.md](04_subqueries_sets.md)
скалярные/табличные подзапросы, коррелированные запросы, `EXISTS`, `ANY|SOME|ALL`, объединения множеств (`UNION`, `INTERSECT`, `EXCEPT`).

### [05_cte_window.md](05_cte_window.md)
`WITH`/рекурсивные CTE, оконные функции, фреймы, ранжирование, скользящие агрегаты.

### [06_modification.md](06_modification.md)
`INSERT`/`ON CONFLICT`, `UPDATE`, `DELETE`, `RETURNING`, массовые операции.

### [07_constraints_indexes.md](07_constraints_indexes.md)
ключи, уникальность, проверки, внешние ключи, индексные типы, выражения, partial/covering.

### [08_functions_ops.md](08_functions_ops.md)
арифметика, строки, даты/время, интервалы, преобразования типов, `COALESCE`/`NULLIF`, регулярки, `CASE`.

### [09_dates_timezones.md](09_dates_timezones.md)
таймзоны, `AT TIME ZONE`, извлечение частей даты, округление, календарные трюки.

### [10_json_array.md](10_json_array.md)
JSON/JSONB операторы, извлечение, модификация, массивы, функции работы с массивами.

### [11_fulltext_search.md](11_fulltext_search.md)
`tsvector`/`tsquery`, ранжирование, словари, выделение совпадений.

### [12_permissions_utils.md](12_permissions_utils.md)
пользователи/роли, гранты, схемы, служебные команды `\d*`, диагностика планов.

### [13_perf_explain.md](13_perf_explain.md)
`EXPLAIN`/`ANALYZE`, планы, типы сканов, буферы, ключевые настройки, common pitfalls.

### [14_transactions_locks.md](14_transactions_locks.md)
транзакции, уровни изоляции, блокировки, `FOR UPDATE` варианты, deadlocks.

### [15_temp_ddl_tools.md](15_temp_ddl_tools.md)
временные таблицы, `CREATE TABLE AS`, `SELECT INTO`, матпредставления, секционирование.

### [16_misc.md](16_misc.md)
генерация серий, случайные значения, `COPY`, `psql` мелочи, полезные системные представления.

### [17_plpgsql_triggers.md](17_plpgsql_triggers.md)
функции/процедуры, `LANGUAGE plpgsql`, триггеры, event triggers, `SECURITY DEFINER`.

По мере заполнения файлов добавляй/корректируй список. Каждый файл лаконичен и опирается на примеры действующего SQL.
