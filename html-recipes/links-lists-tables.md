# Линки, списки и таблицы

## Ссылки
```html
<a href="/about" title="About us">About</a>
<a href="tel:+1234567890">Call</a>
<a href="mailto:info@example.com">Email</a>
<a href="#section">Anchor</a>
```
- `target="_blank"` + `rel="noopener noreferrer"` для внешних ссылок.
- Возможность добавления `download` для загрузки файлов.

## Списки
- Нумерованный: `<ol> <li>Item</li> </ol>`.
- Маркированный: `<ul> <li>Item</li> </ul>`.
- Описание: `<dl> <dt>Term</dt> <dd>Description</dd> </dl>`.
- Вложенные списки (используйте отступы для читабельности).

## Навигация
- `<nav>` + `<ul>` для меню.
- breadcrumbs (aria-label="breadcrumb").

## Таблицы
```html
<table>
  <caption>Expense Report</caption>
  <thead>
    <tr>
      <th scope="col">Item</th>
      <th scope="col">Cost</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th scope="row">Coffee</th>
      <td>$5</td>
    </tr>
  </tbody>
</table>
```
- Используйте `<th scope="row|col">` для доступности.
- Объединение: `rowspan`, `colspan`.
- Data tables: `<thead>`, `<tbody>`, `<tfoot>`, `<caption>`, `<summary>`.
- Респонсив: `overflow-x: auto;`, stack layout на мобильных (CSS).

## Lists внутри таблиц
- Избегайте: списки внутри `<td>` — используйте `<ul>`/`<ol>` при необходимости.

