## Инициализация контекста

- Поддержка WebGL 2 предпочтительна, но проверяйте и WebGL 1:
  ```js
  const canvas = document.querySelector("canvas");
  const gl = canvas.getContext("webgl2") ?? canvas.getContext("webgl");
  if (!gl) throw new Error("WebGL не поддерживается");
  ```
- Ограничивайте размер канваса: `canvas.width = devicePixelRatio * cssWidth`, но учитывайте лимит `gl.getParameter(gl.MAX_RENDERBUFFER_SIZE)`.
- Включите `preserveDrawingBuffer: false` для лучшей производительности, если не нужно сохранять пиксели между кадрами.
- Следите за потерей контекста: подписывайтесь на события `webglcontextlost` и `webglcontextrestored`.

## Структура pipeline

- WebGL — state machine: создаём объекты (`createBuffer`, `createProgram`, `createTexture`) и привязываем их (`bindBuffer`, `useProgram`).
- Основные этапы:
  1. Подготовка данных (вершины, индексы) и загрузка их в GPU.
  2. Компиляция шейдеров (`compileShader`) и линковка программы (`linkProgram`).
  3. Связка атрибутов (`vertexAttribPointer`, `enableVertexAttribArray`).
  4. Передача uniform-параметров (матрицы, время, палитры).
  5. Настройка viewport и состояния (`viewport`, `clearColor`, `enable`).
  6. Вызов `drawArrays` или `drawElements`.
- Контекст WebGL 2 предлагает VAO (`vertex array objects`), UBO (`uniform buffer objects`), integer текстуры, transform feedback.

## Минимальный пример

```html
<canvas id="gl" width="800" height="600"></canvas>
<script type="module">
const canvas = document.getElementById("gl");
const gl = canvas.getContext("webgl2") ?? canvas.getContext("webgl");
if (!gl) throw new Error("WebGL unsupported");

const vsSource = `#version 300 es
in vec2 position;
void main() {
  gl_Position = vec4(position, 0.0, 1.0);
}`;

const fsSource = `#version 300 es
precision highp float;
out vec4 outColor;
void main() {
  outColor = vec4(0.2, 0.6, 0.9, 1.0);
}`;

function compile(type, source) {
  const shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    throw new Error(gl.getShaderInfoLog(shader) ?? "Shader compile error");
  }
  return shader;
}

const program = gl.createProgram();
gl.attachShader(program, compile(gl.VERTEX_SHADER, vsSource));
gl.attachShader(program, compile(gl.FRAGMENT_SHADER, fsSource));
gl.linkProgram(program);
if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
  throw new Error(gl.getProgramInfoLog(program) ?? "Program link error");
}
gl.useProgram(program);

const vertices = new Float32Array([
  -0.5, -0.5,
   0.5, -0.5,
   0.0,  0.5,
]);
const vao = gl.createVertexArray();
const vbo = gl.createBuffer();
gl.bindVertexArray(vao);
gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

const loc = gl.getAttribLocation(program, "position");
gl.enableVertexAttribArray(loc);
gl.vertexAttribPointer(loc, 2, gl.FLOAT, false, 0, 0);

gl.clearColor(0.05, 0.05, 0.07, 1.0);
gl.clear(gl.COLOR_BUFFER_BIT);
gl.drawArrays(gl.TRIANGLES, 0, 3);
</script>
```

- Для WebGL 1 используйте версии шейдеров без префикса `#version` и вместо `in/out` — `attribute`/`varying`.
- Следите за размером массивов: `gl.bufferData` принимает `ArrayBufferView`.

## Обновление рендер-цикла

- Используйте `requestAnimationFrame`:
  ```js
  let start = performance.now();
  function frame(now) {
    const dt = (now - start) * 0.001;
    start = now;
    render(dt, now * 0.001);
    requestAnimationFrame(frame);
  }
  requestAnimationFrame(frame);
  ```
- В `render` обновляйте viewport при resize:
  ```js
  const pixelRatio = window.devicePixelRatio ?? 1;
  const width = Math.floor(canvas.clientWidth * pixelRatio);
  const height = Math.floor(canvas.clientHeight * pixelRatio);
  if (canvas.width !== width || canvas.height !== height) {
    canvas.width = width;
    canvas.height = height;
    gl.viewport(0, 0, width, height);
  }
  ```
- Храните ссылки на часто используемые uniform-локации, чтобы не запрашивать их каждый кадр (`getUniformLocation` вне цикла).

## Управление ошибками и логированием

- На дев-стадии проверяйте ошибки: после ключевых вызовов `const err = gl.getError(); if (err) console.warn(err);`.
- Включайте wrap-библиотеки (например, TWGL `twgl.createProgramInfo`) для унификации ошибок.
- Используйте `KHR_debug` (WebGL 2): `gl.debugMessageCallback` и `gl.enable(gl.DEBUG_OUTPUT)`.

## Частые подводные камни

- Несоответствие типов атрибутов: следите за `size`/`type` в `vertexAttribPointer`.
- Забытые биндинги: WebGL использует текущий `ARRAY_BUFFER` и `ELEMENT_ARRAY_BUFFER`; перед `drawElements` убедитесь, что буфер привязан.
- Неправильные `clear` маски: комбинируйте `gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT`.
- Мусор в матрицах: применяйте библиотеки (gl-matrix) или аккуратно создавайте матрицы вручную.
- Неправильный winding order: по умолчанию фронт-фейсы — CCW (`gl.frontFace(gl.CCW)`).
