# Ограничения и индексы

## Первичный и уникальный ключ
```sql
CREATE TABLE users (
  id bigserial PRIMARY KEY,
  email text UNIQUE
);
-- или
ALTER TABLE users ADD CONSTRAINT uq_users_email UNIQUE (email);
```
- `PRIMARY KEY` = `NOT NULL` + `UNIQUE` + индекс.

## CHECK
```sql
ALTER TABLE orders ADD CONSTRAINT chk_total_nonneg CHECK (total >= 0);
```
- Может ссылаться на выражения/функции; `NOT VALID` для отложенной проверки существующих строк.

## Внешние ключи
```sql
ALTER TABLE orders
ADD CONSTRAINT fk_orders_customer
FOREIGN KEY (customer_id) REFERENCES customers(id)
ON DELETE CASCADE
ON UPDATE RESTRICT;
```
- Опции: `NO ACTION` (по умолчанию), `RESTRICT`, `CASCADE`, `SET NULL`, `SET DEFAULT`.
- `DEFERRABLE INITIALLY DEFERRED` - проверка в конце транзакции.
- Отложить/вернуть немедленность: `SET CONSTRAINTS ALL DEFERRED; ... SET CONSTRAINTS ALL IMMEDIATE;`

## EXCLUDE (для пересечений/диапазонов)
```sql
CREATE TABLE bookings (
  room int,
  period tsrange,
  EXCLUDE USING gist (room WITH =, period WITH &&)
);
```
- Запрещает пересечения по условию; требует поддерживаемого индексного типа (`gist`/`spgist`).

## Индексные типы (частые)
- `btree` - по умолчанию, сортировки, равенство/диапазон.
- `hash` - равенство (редко).
- `GIN` - массивы, `jsonb`, полнотекст (`tsvector`).
- `GiST` - гео, диапазоны, полнотекст варианты.
- `BRIN` - большие таблицы с коррелированными данными.

## Создание индекса
```sql
CREATE INDEX idx_orders_status_created
  ON orders (status, created_at DESC);

CREATE INDEX CONCURRENTLY idx_users_email ON users (email); -- без блокировки записи
```
- `CONCURRENTLY` нельзя внутри транзакции.

## Частичный и условный
```sql
CREATE INDEX idx_orders_active ON orders (created_at) WHERE status = 'active';
```
- Ускоряет частые выборки по условию; занимает меньше места.

## Индекс на выражении
```sql
CREATE INDEX idx_users_lower_email ON users (lower(email));
```
- Требует использовать то же выражение в запросе.

## Covering (INCLUDE)
```sql
CREATE INDEX idx_orders_status_created_inc
  ON orders (status) INCLUDE (created_at, total);
```
- Дополнительные колонки хранятся в индексе для покрытия запросов (без сортировки по ним).

## Обслуживание индексов
- `REINDEX TABLE tbl;` или `REINDEX INDEX idx;` - пересборка.
- `CLUSTER tbl USING idx;` - физическая перестройка таблицы по индексу (хранение отсортировано).

## Удаление/переименование
```sql
DROP INDEX IF EXISTS idx_orders_status_created;
ALTER INDEX idx_orders_status_created RENAME TO idx_orders_status_dt;
```
