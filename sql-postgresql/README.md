# SQL (PostgreSQL) шпаргалка

Основные рецепты и best practices работы с PostgreSQL: базовый синтаксис, DDL/DML, запросы, функции, администрирование, performance tuning и инструменты.

### 📚 [Базовый синтаксис и типы](./basics-types.md)

- Подключение через psql, основные команды
- Типы данных (numeric, text, jsonb, arrays)
- SELECT/INSERT/UPDATE/DELETE, UPSERT
- Соглашения имен и ограничения

### 🏗️ [DDL и управление схемой](./ddl-schema.md)

- CREATE/ALTER TABLE, enum, constraints
- Индексы (btree, gin, partial)
- Foreign keys, check, exclude
- Схемы, views, миграции

### ✏️ [DML: запросы, joins, подзапросы](./dml-queries.md)

- JOIN, WHERE, ORDER BY
- Подзапросы, EXISTS, CTE
- Window функции (rank, lag)
- JSON/JSONB запросы, пагинация

### 🧮 [Функции, агрегаты и операторы](./functions-operators.md)

- Агрегаты, string_agg/array_agg
- Дата/текст/JSON функции
- Операторы сравнения, array, jsonb
- Пользовательские функции и триггеры

### ⚙️ [Администрирование и инструменты](./admin-tools.md)

- Роли, привилегии, search_path
- postgresql.conf и pg_hba.conf
- Backup/restore, репликация
- Инструменты: pgcli, pgAdmin, exporter

### 🚀 [Оптимизация и мониторинг](./performance-monitoring.md)

- EXPLAIN/ANALYZE, индексы
- pg_stat_statements, auto_explain
- Vacuum/bloat, partitioning
- Connection pooling, extensions

### 🔒 [Безопасность и бэкапы](./security-backup.md)

- SCRAM auth, pg_hba.conf, TLS
- Row-Level Security
- pgaudit, логирование
- Backup стратегии, PITR, DR
