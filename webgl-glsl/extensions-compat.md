## Проверка расширений

- Получить список:
  ```js
  const extensions = gl.getSupportedExtensions();
  console.log(extensions);
  ```
- Получить расширение:
  ```js
  const ext = gl.getExtension("OES_element_index_uint");
  if (!ext) {
    // fallback на Uint16 индексы
  }
  ```
- Для WebGL 2 многие расширения встроены (`vertex_array_object`, `instanced_arrays`, `draw_buffers`).
- Проверяйте доступность на старте и конфигурируйте пайплайн (например, отключайте HDR, если нет `EXT_color_buffer_float`).

## Ключевые расширения WebGL 1

- `OES_element_index_uint` — позволяет использовать `Uint32Array` для индексов.
- `OES_standard_derivatives` — доступ к `dFdx`, `dFdy`, `fwidth`. Нужен для нормалей в screen space и anti-aliasing.
- `EXT_shader_texture_lod` — `textureLod` во фрагментных шейдерах.
- `WEBGL_depth_texture` — depth текстуры для shadow mapping.
- `WEBGL_draw_buffers` — множественные render targets.
- `OES_texture_float` и `OES_texture_half_float` — плавающие текстуры. Для линейной фильтрации нужны дополнительные `linear` расширения.
- `ANGLE_instanced_arrays` — instancing.
- `WEBGL_compressed_texture_*` — поддержка форматов BC (S3TC), ETC, PVRTC, ASTC.

## WebGL 2 расширенные возможности

- Встроенные VAO, transform feedback, instanced rendering, multiple render targets (`gl.drawBuffers`), `gl.vertexAttribDivisor`.
- Плавающие текстуры и рендер в них (`EXT_color_buffer_float`) поддерживаются по умолчанию в большинстве браузеров.
- Integer текстуры (`R8UI`, `RGBA8I`), 3D-текстуры (`gl.TEXTURE_3D`), array textures.
- Sampler objects: `gl.createSampler`, `gl.samplerParameteri`.

## Fallback стратегии

- Если нет float текстур → используйте 8-bit и кодируйте значения (сжать диапазон, использовать RGBA каналов).
- Нет `OES_element_index_uint` → разбивайте меши на части с индексами < 65536 или используйте `gl.drawArrays`.
- Нет `OES_standard_derivatives` → приближайте нормали конечными разностями вручную с использованием соседних UV.
- Нет `WEBGL_draw_buffers` → выполняйте постпроцессинг несколькими проходами.
- Нет instancing → используйте `gl.drawArrays` в цикле и изменяйте uniform (подходит для небольших количеств объектов).

## Проверка precision на мобильных

- Некоторые GPU не поддерживают `highp` во фрагментном шейдере:
  ```js
  const precision = gl.getShaderPrecisionFormat(gl.FRAGMENT_SHADER, gl.HIGH_FLOAT);
  if (precision.precision === 0) {
    // fallback на mediump, пересчёт шейдеров
  }
  ```
- Избегайте длинных циклов и сложных веток на мобильных — драйвер может падать в NaN.

## Совместимость WebGL 1 ↔ WebGL 2

- Разделяйте шейдеры: одна версия для `#version 100`, другая для `#version 300 es`.
- Оборачивайте код префиксами:
  ```glsl
  #ifdef GL_ES
  precision mediump float;
  #endif
  ```
- Используйте препроцессор для alias:
  ```glsl
  #ifdef GL_ES
  #define texture texture2D
  #endif
  ```
- Когда компилируете шейдеры, подставляйте `#define WEBGL2 1`, чтобы переключать на `in/out`.

## Снижение требований

- Разрешение: уменьшаем размер FBO в 2 раза (render-to-texture, потом upscale).
- Количество проходов: объединяйте эффекты (например, blur + цветокоррекция).
- Сжатие текстур: используйте KTX/KTX2 с подходящим форматом в зависимости от устройства.
- Ограничение draw calls: объединяйте геометрию заранее.

## Отладка расширений

- `WEBGL_debug_renderer_info` — получить GPU/renderer имя:
  ```js
  const dbg = gl.getExtension("WEBGL_debug_renderer_info");
  if (dbg) {
    console.log(gl.getParameter(dbg.UNMASKED_RENDERER_WEBGL));
  }
  ```
- `WEBGL_lose_context` — вручную терять контекст (для тестов): `gl.getExtension("WEBGL_lose_context")?.loseContext();`.
- Spector.js отображает какие расширения активны и как они используются.
