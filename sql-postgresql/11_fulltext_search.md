# Полнотекстовый поиск

## Основы
```sql
SELECT to_tsvector('russian', 'Привет мир');
SELECT to_tsquery('russian', 'привет & мир');
```
- `tsvector` - индексируемый текст; `tsquery` - запрос.

## Поиск
```sql
SELECT id, title
FROM docs
WHERE to_tsvector('russian', title || ' ' || body)
      @@ plainto_tsquery('russian', 'быстрый поиск');
```
- `plainto_tsquery` строит запрос из текста (AND); `phraseto_tsquery` для фраз.

## Ранжирование
```sql
SELECT id, ts_rank_cd(
         to_tsvector('russian', title || ' ' || body),
         plainto_tsquery('russian', 'быстрый поиск')
       ) AS rank
FROM docs
WHERE ...
ORDER BY rank DESC;
```
- `ts_rank_cd` - с учетом расстояния; `ts_rank` - без.

## Подсветка
```sql
SELECT ts_headline('russian', body, plainto_tsquery('russian','быстрый поиск'),
                   'StartSel=<b>,StopSel=</b>,MaxFragments=2') AS snippet
FROM docs;
```

## Индексация
```sql
CREATE INDEX idx_docs_fts ON docs
USING gin (to_tsvector('russian', title || ' ' || body));
```
- Для часто обновляемых таблиц - `GIN` по выражению.

## Словари и стоп-слова
- Указание конфигурации: `'russian'`, `'simple'`, `'english'`.
- Проверить конфигурацию: `SELECT * FROM pg_ts_config WHERE cfgname = 'russian';`

## Расширенные операторы
```sql
SELECT 'мир & (привет | салют)'::tsquery;
SELECT to_tsvector('text') @@ to_tsquery('text & !other');  -- отрицание
```
