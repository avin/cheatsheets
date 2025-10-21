# Формы и валидация

## Основы формы
```html
<form action="/submit" method="post">
  <label for="email">Email</label>
  <input type="email" id="email" name="email" required />

  <label>
    Password
    <input type="password" name="password" minlength="8" required />
  </label>

  <button type="submit">Sign in</button>
</form>
```
- `method="get|post"`. Для upload → `enctype="multipart/form-data"`.
- Используйте `<fieldset>` и `<legend>` для группировки.

## Input types
- `text`, `password`, `email`, `tel`, `url`, `number`, `range`, `date`, `time`, `datetime-local`, `color`, `file`.
- Mobile friendly: `inputmode`, `pattern` (например, `pattern="[0-9]{3}-[0-9]{3}"`).

## Select & datalist
```html
<select name="country" required>
  <option value="">Select...</option>
  <option value="us">United States</option>
</select>

<input list="browsers" name="browser" />
<datalist id="browsers">
  <option value="Chrome">
  <option value="Firefox">
</datalist>
```

## Checkbox/Radio groups
```html
<fieldset>
  <legend>Newsletter</legend>
  <label><input type="checkbox" name="topics" value="design"> Design</label>
  <label><input type="checkbox" name="topics" value="dev"> Development</label>
</fieldset>
```
- Для radio: одно имя (`name`), разные `value`.

## Validation
- HTML5 атрибуты: `required`, `min`, `max`, `pattern`, `step`.
- `novalidate` отключает встроенную проверку.
- Custom messages: `oninvalid`, `setCustomValidity`, `reportValidity()`.
- Client-side (JS) + server-side валидация.

## Autocomplete/Autofill
- Атрибут `autocomplete`: `email`, `name`, `address-level1`, `cc-number`, `organization-title`, `one-time-code`.
- Подсказки для autofill помогут пользователям.

## Security
- CSRF token (`<input type="hidden" name="_token" value="...">`).
- `SameSite` для cookie, `X-Requested-With`.
- Sanitization на сервере.

