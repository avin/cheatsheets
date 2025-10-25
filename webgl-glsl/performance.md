## Производительность GPU

- Измеряйте на целевых устройствах: десктопы, мобильные, VR.
- Используйте `EXT_disjoint_timer_query`:
  ```js
  const ext = gl.getExtension("EXT_disjoint_timer_query_webgl2");
  const query = ext.createQueryEXT();
  ext.beginQueryEXT(ext.TIME_ELAPSED_EXT, query);
  // draw...
  ext.endQueryEXT(ext.TIME_ELAPSED_EXT);
  // позже:
  if (ext.getQueryObjectEXT(query, ext.QUERY_RESULT_AVAILABLE_EXT)) {
    const nanos = ext.getQueryObjectEXT(query, ext.QUERY_RESULT_EXT);
    console.log(`${nanos / 1e6} ms GPU`);
  }
  ```
- На WebGL 1 используйте `EXT_disjoint_timer_query`.
- Сравнивайте CPU-время (`performance.now()`).

## Оптимизация шейдеров

- Избегайте дорогих операций: `pow`, `exp`, `log`, `inverse`, `sqrt`. Заменяйте аппроксимациями (`inversesqrt`, `approxPow`).
- Сокращайте ветвления; используйте `mix`, `step`, `smoothstep`.
- Используйте `mediump`, когда high precision не требуется, чтобы ускорить мобильные шейдеры.
- Предварительно вычисляйте значения на CPU и передавайте как uniform.
- Для raymarch уменьшайте число шагов, используйте адаптивные epsilon и `maxDistance`.

## Организация кода

- Держите шейдерные источники в отделяемых файлах (например, `.glsl` подключаемые через bundler) или шаблонах.
- Пишите небольшие функции, избегайте дублирования.
- В JS создавайте хелперы:
  ```js
  function setUniforms(gl, program, uniforms) {
    for (const [name, value] of Object.entries(uniforms)) {
      const loc = gl.getUniformLocation(program, name);
      if (value.length === undefined) gl.uniform1f(loc, value);
      else if (value.length === 2) gl.uniform2fv(loc, value);
      // ...
    }
  }
  ```
- TWGL: `twgl.createProgramInfo(gl, [vsSource, fsSource])`, `twgl.setBuffersAndAttributes`.

## Mobile-friendly подходы

- Уменьшайте разрешение FBO, используйте temporal upscale.
- Избегайте больших текстур (> 2048) — проверяйте `MAX_TEXTURE_SIZE`.
- Заменяйте `highp` на `mediump` во фрагменте, если нет необходимости.
- Используйте `ASTC` или `ETC2` для текстур на Android, `PVRTC` — iOS (через KTX2 + BasisU).
- Ограничивайте количество шейдеров: мобильные драйверы оптимизируют при линковке, экономьте на лишних вариантах.

## Профилирование и инструменты

- Spector.js: инспекция draw calls, состояния, uniform.
- Chrome DevTools > Rendering > Frame viewer: отражает GPU-команды.
- Firefox WebGL-inspector.
- `requestAnimationFrame` callback показывает время кадра и dropped frames.
- Для автоматических тестов используйте `headless-gl` (Node) или Puppeteer + Canvas capture.

## Оптимизация памяти

- Reuse буферы: не создавайте новые `Float32Array` каждый кадр.
- Используйте структурированные `ArrayBuffer` + `DataView` для packing данных.
- Переиспользуйте FBO/текстуры, изменяя `gl.texImage2D` 0-level.
- Для больших сеток храните данные в `Texture Buffer` (WebGL 2) или `Sampler2D` (pack x/y).
- Удаляйте ресурсы (`deleteTexture`) при переходе между сценами.

## Сжатие shader source

- Модульность: `#include` (через препроцессор) для повторно используемых функций (noise, palette).
- Минимизируйте количество `#ifdef`, если их много — генерируйте шейдеры через шаблоны и передавайте параметры uniform.

## CI и тестирование

- Снимайте скриншоты (Puppeteer/Playwright) и сравнивайте с эталоном (Resemble.js).
- Тесты корректности: проверяйте значения в ключевых бафферах через `readPixels`.
- В CI включайте smoke-проверку на headless Chrome, чтобы ловить ошибки компиляции.

## Частые проблемы

- На macOS/Intel возможны `NaN` при `normalize(vec3(0.0))` — всегда проверяйте делители.
- Драйверы Adreno не любят бесконечные циклы и `discard` внутри без условий.
- В Safari/WebKit `gl_FrontFacing` иногда работает нестабильно при нестандартных `frontFace`.
- Плавающие текстуры могут требовать включения `gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1)`.
