# Web App Manifest

## Пример manifest.json
```json
{
  "name": "Awesome App",
  "short_name": "Awesome",
  "description": "A sample progressive web app.",
  "start_url": "/?source=pwa",
  "scope": "/",
  "display": "standalone",
  "background_color": "#ffffff",
  "theme_color": "#1e90ff",
  "icons": [
    {
      "src": "/icons/icon-192.png",
      "sizes": "192x192",
      "type": "image/png"
    },
    {
      "src": "/icons/icon-512.png",
      "sizes": "512x512",
      "type": "image/png",
      "purpose": "any maskable"
    }
  ],
  "shortcuts": [
    {
      "name": "Open Dashboard",
      "short_name": "Dashboard",
      "description": "Go to dashboard",
      "url": "/dashboard",
      "icons": [{ "src": "/icons/dashboard.png", "sizes": "96x96" }]
    }
  ]
}
```

## Атрибуты
- `name`, `short_name`
- `start_url` — URL после установки.
- `display` — `fullscreen`, `standalone`, `minimal-ui`, `browser`.
- `background_color`, `theme_color` — цвета стартового экрана/toolbar.
- `icons` — минимум 192x192 и 512x512, `purpose: maskable` для адаптивных форм.
- `scope` ограничивает навигацию PWA.
- `description`, `lang`, `dir` (подсказка языку/направлению).
- `categories` — категории в store (Google Play via Trusted Web Activities).

## Shortcuts
- Позволяют добавлять контекстные действия (Android.
- Формат: массив объектов с `name`, `url`, optional icon.

## Отдача манифеста
```html
<link rel="manifest" href="/manifest.json" />
<meta name="theme-color" content="#1e90ff" />
```

## Dynamic manifest
- Можно отдавать сервером (Content negotiation).
- Для multi-language — `manifest@lang` или `start_url`.

## Validation
- Lighthouse (PWA), PWABuilder.

