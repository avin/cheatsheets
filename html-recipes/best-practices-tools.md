# Best practices и инструменты

## Общие рекомендации
- Валидный HTML5 (`<!DOCTYPE html>`).
- Используйте семантические элементы вместо `<div>` где возможно.
- Минимизируйте лишние атрибуты и inline стили.
- Разделяйте контент и презентацию (CSS).

## Performance
- Указывайте `defer` для скриптов, которые не нужны сразу.
- Inline критический CSS в `<head>`, остальное грузите асинхронно.
- Избегайте `document.write`, лишних reflow/DOM манипуляций.
- Используйте `rel="preload"`, `rel="prefetch"`, `rel="dns-prefetch"` для оптимизации загрузки.
- Monitor bundle size и HTTP requests.

## SEO
- `meta` description, OpenGraph (`og:title`, `og:image`), Twitter Cards.
- Heading structure: один `h1`, затем `h2`, `h3` и т.д.
- Sitemap, robots.txt.
- ЧПУ URLs (`/blog/how-to-css`).
- Schema markup (JSON-LD).

## Accessibility checks
- Семантика: landmark roles (`<header>`, `<main>`).
- Focus management, keyboard navigation.
- Valid `aria` атрибуты, skip links.

## Testing/Validation
- HTML validator (W3C, https://validator.w3.org/).
- Lighthouse (Performance/SEO/Accessibility).
- Jest/React Testing Library для компонентов (если SPA).
- Visual regression testing (Percy, Chromatic).

## Editor tooling
- Emmet (VS Code, JetBrains) для быстрой верстки (`div.container>ul>li*3`).
- Prettier/ESLint с HTML плагинами.
- Stylelint (с поддержкой CSS-in-HTML).

## Deployment
- Minify HTML (Terser? HTMLMinifier).
- HTTP headers: `Content-Type: text/html; charset=UTF-8`, security headers.
- Brotli/Gzip compression.
- Monitoring: error reporting, uptime.

