# JSON/JSONB и массивы

## JSONB извлечение
```sql
SELECT data->'profile'        AS obj,     -- JSON
       data->>'name'          AS name,    -- текст
       data#>>'{addr,city}'   AS city     -- путь
FROM users;
```

## Обновление JSONB
```sql
UPDATE users
SET data = jsonb_set(data, '{profile,city}', to_jsonb('Paris'));

-- добавить/заменить поле
UPDATE users
SET data = data || jsonb_build_object('vip', true);
```
- `jsonb_set(target, path, value, create_missing := true)`

## Фильтрация по JSONB
```sql
SELECT * FROM users WHERE data->>'status' = 'active';
SELECT * FROM users WHERE data @> '{"roles":["admin"]}';
SELECT * FROM users WHERE data ? 'vip';          -- есть ключ
SELECT * FROM users WHERE data ?| array['vip','pro'];  -- есть любой ключ
```

## Генерация JSON
```sql
SELECT json_build_object('id', id, 'tags', tags) FROM items;
SELECT jsonb_agg(row_to_json(t)) FROM (SELECT id, name FROM users) t;
```

## JSONPath
```sql
SELECT data @@ '$.items[*] ? (@.price > 10)';            -- bool, есть ли совпадение
SELECT jsonb_path_query(data, '$.items[*].name');        -- вернуть найденные элементы
SELECT jsonb_path_exists(data, '$.a ? (@ > 10)');        -- еще один вариант проверки
```

## Массивы: базовое
```sql
SELECT ARRAY[1,2,3] AS a, ARRAY['x','y'] AS b;
SELECT arr[1], arr[2:3] FROM (VALUES (ARRAY[10,20,30,40])) v(arr);
```
- Индексация с 1; срезы сохраняют массив.

## Операции с массивами
```sql
SELECT ARRAY[1,2] || ARRAY[3,4];        -- конкатенация
SELECT ARRAY[1,2,3] @> ARRAY[2];        -- содержит
SELECT ARRAY[1,2] && ARRAY[2,3];        -- пересечение не пусто
SELECT unnest(ARRAY[1,2,3]) AS x;       -- развернуть в строки
```

## Поиск по массивам
```sql
SELECT * FROM books WHERE authors @> ARRAY['John'];      -- содержит автора
SELECT * FROM books WHERE 'John' = ANY (authors);        -- эквивалент contains
```

## Индексы для JSONB/массивов
- Часто `GIN` (`CREATE INDEX ... USING gin (data jsonb_path_ops)` или `jsonb_ops`).
- Для массивов: `CREATE INDEX ... USING gin (authors);`.
