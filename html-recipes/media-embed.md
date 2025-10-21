# Мультимедиа и встроенный контент

## Изображения
```html
<picture>
  <source srcset="/images/photo.avif 1x, /images/photo@2x.avif 2x" type="image/avif" />
  <source srcset="/images/photo.webp 1x, /images/photo@2x.webp 2x" type="image/webp" />
  <img
    src="/images/photo.jpg"
    srcset="/images/photo.jpg 1x, /images/photo@2x.jpg 2x"
    sizes="(min-width: 768px) 50vw, 100vw"
    alt="Landscape"
    width="800"
    height="600"
    loading="lazy"
  />
</picture>
```
- `srcset` с плотностями (`1x`, `2x`) обеспечивает поддержку Retina/HiDPI экранов.
- `sizes` подсказывает браузеру, какую ширину занимает изображение при разных breakpoints (экономия трафика).
- Используйте `width`/`height` атрибуты для резервирования layout.
- `loading="lazy"` для отложенной загрузки (Chrome/Firefox/Safari).
- WebP/AVIF — более эффективные форматы.

## Видео/аудио
```html
<video controls preload="none" poster="/poster.jpg">
  <source src="/video.mp4" type="video/mp4" />
  <track kind="subtitles" src="/subtitles.vtt" srclang="en" label="English" />
  Your browser does not support the video tag.
</video>
```
- Атрибуты: `controls`, `autoplay` (использовать ограниченно), `muted`, `loop`, `playsinline`.
- Добавляйте субтитры (`<track>`), альтернативный текст.
- Audio: `<audio controls>`.

## SVG
- Inline `<svg>` для иконок (позволяет стилизацию, `currentColor`).
- `role="img"` + `aria-label`/`title` для доступности.
- External: `<img src="icon.svg" alt="...">`.

## iframe / embed
```html
<iframe src="https://example.com/embed" title="Widget" width="600" height="400" loading="lazy" referrerpolicy="no-referrer" sandbox="allow-scripts allow-same-origin"></iframe>
```
- Атрибут `title` обязателен.
- `sandbox` ограничивает возможности (разрешайте только нужные).
- `allowfullscreen`, `allow="fullscreen; clipboard-write"` для расширенных возможностей.

## Canvas & WebGL
- `<canvas>` для 2D/3D; используйте fallback содержимое.
- WebGL: проверка поддержки (`if (!canvas.getContext('webgl')) ...`).

## Lazy load и Intersection Observer
- `loading="lazy"` (Chrome/Firefox/Safari). Для кастомного поведения — Intersection Observer.

## SEO / accessibility
- Предоставляйте альтернативный контент (`fallback`).
- Четко указывайте `alt`/`aria` для медиа.
- Captioning/Transcripts для видео.

