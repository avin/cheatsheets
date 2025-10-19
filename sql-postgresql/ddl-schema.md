# DDL и управление схемой

## Создание таблиц
```sql
CREATE TABLE users (
    id uuid PRIMARY KEY DEFAULT gen_random_uuid(),
    name text NOT NULL,
    email text UNIQUE NOT NULL,
    status user_status NOT NULL DEFAULT 'active',
    created_at timestamptz NOT NULL DEFAULT now()
);
```
- Используйте `DEFAULT` для заполнения дат/значений.

## Enum
```sql
CREATE TYPE user_status AS ENUM ('active', 'inactive', 'blocked');
ALTER TYPE user_status ADD VALUE 'pending';
```

## Изменение схемы
```sql
ALTER TABLE users ADD COLUMN last_login timestamptz;
ALTER TABLE users ALTER COLUMN name TYPE varchar(255);
ALTER TABLE users DROP COLUMN obsolete_col;
```

## Индексы
```sql
CREATE INDEX idx_users_email ON users(email);
CREATE INDEX idx_orders_customer_created ON orders(customer_id, created_at);
CREATE UNIQUE INDEX idx_orders_ref_unique ON orders(reference) WHERE cancelled = false;
```
- `btree` (по умолчанию), `gin` (jsonb, arrays), `gist` (гео), `hash` (равенство).
- Учитывайте **selectivity** и cardinality.

## Foreign Keys
```sql
ALTER TABLE orders
ADD CONSTRAINT orders_customer_fk
FOREIGN KEY (customer_id) REFERENCES users(id)
ON DELETE CASCADE;
```

## Constraints
- `CHECK (quantity >= 0)`.
- `UNIQUE`, `NOT NULL`, `PRIMARY KEY`.
- `EXCLUDE` (для range типов, планировщиков).

## Schema / namespaces
```sql
CREATE SCHEMA analytics;
ALTER ROLE analyst SET search_path TO analytics, public;
```

## Views
```sql
CREATE VIEW active_users AS
SELECT id, name FROM users WHERE status = 'active';
```
- Materialized view: `CREATE MATERIALIZED VIEW ... REFRESH MATERIALIZED VIEW`.

## Миграции
- Используйте инструменты: Alembic, Flyway, Liquibase, Sqitch.
- Храните миграции в VCS, порядок применения важен.
- Для сложных ALTER — `LOCK TABLE` / `CONCURRENTLY` (для индексов).
