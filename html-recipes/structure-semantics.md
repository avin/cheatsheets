# Структура документа и семантика

## Базовый шаблон
```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Document</title>
  <meta name="description" content="Short description for SEO" />
  <link rel="icon" href="/favicon.ico" />
</head>
<body>
  <!-- content -->
</body>
</html>
```

## Семантические теги
- Заголовки: `<h1>` ... `<h6>` — иерархия содержимого.
- Структура страницы: `<header>`, `<nav>`, `<main>`, `<section>`, `<article>`, `<aside>`, `<footer>`.
- Текст: `<p>`, `<blockquote>`, `<cite>`, `<code>`, `<pre>`, `<figure>`, `<figcaption>`, `<time>` (`datetime="2024-06-01"`).
- Контейнеры: `<div>` для общих блоков, но предпочитайте семантические элементы.
- Inline: `<span>`, `<strong>`, `<em>`, `<mark>`, `<small>`, `<abbr title="">`.

## Document outline
- Каждый `<section>`/`<article>` должен иметь заголовок (`<h2>` и т.д.).
- Не пропускайте уровни заголовков (`h1` → `h2` → `h3`).
- Один `<h1>` на страницу (внутри `main`).

## Meta и SEO
- `lang` атрибут на `<html>`.
- `meta` теги: `description`, `keywords` (устарел), `og:*` (Open Graph), `twitter:card`.
- Canonical: `<link rel="canonical" href="https://example.com/page" />`.
- JSON-LD структурированные данные (schema.org).

## Scripts
- Подключение: `<script src="app.js" defer></script>` (defer для загрузки после парсинга).
- Inline: `<script type="module"> import ...; </script>`.
- `nomodule` fallback для старых браузеров.

## Noscript
- `<noscript>` — контент для пользователей с отключенным JS.

## Base
- `<base href="https://example.com/" target="_blank">` — влияние на относительные URL.

