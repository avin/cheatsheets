## Вершинные буферы и индексация

- Создание VBO:
  ```js
  const positionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
  ```
- Различайте usage: `STATIC_DRAW` для неизменяемых, `DYNAMIC_DRAW` — редкие обновления, `STREAM_DRAW` — частые.
- Индексы:
  ```js
  const indexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);
  ```
- Для больших мешей используйте `OES_element_index_uint` (Uint32 индексы).
- WebGL 2: храните несколько атрибутов в одном буфере, используя stride/offset.

## VAO (Vertex Array Object)

- Упрощает привязку: один вызов `gl.bindVertexArray(vao)` на draw.
  ```js
  const vao = gl.createVertexArray();
  gl.bindVertexArray(vao);

  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  gl.enableVertexAttribArray(0);
  gl.vertexAttribPointer(0, 3, gl.FLOAT, false, 8 * 4, 0);

  gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);
  gl.enableVertexAttribArray(1);
  gl.vertexAttribPointer(1, 3, gl.FLOAT, false, 0, 0);
  ```
- Не забудьте `gl.bindVertexArray(null)` после настройки.
- Для WebGL 1 подключайте расширение `OES_vertex_array_object`.

## Uniform и Uniform Buffer Objects

- Индивидуальные uniform:
  ```js
  const timeLoc = gl.getUniformLocation(program, "uTime");
  gl.uniform1f(timeLoc, time);
  gl.uniformMatrix4fv(matrixLoc, false, matrix);
  ```
- Для массивов используйте `uniform1fv`, `uniform3fv` и т.д.
- WebGL 2: UBO позволяют группировать параметры:
  ```glsl
  layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
  };
  ```
  ```js
  const blockIndex = gl.getUniformBlockIndex(program, "Camera");
  gl.uniformBlockBinding(program, blockIndex, 0);

  const ubo = gl.createBuffer();
  gl.bindBufferBase(gl.UNIFORM_BUFFER, 0, ubo);
  gl.bufferData(gl.UNIFORM_BUFFER, 2 * 16 * 4, gl.DYNAMIC_DRAW);
  gl.bufferSubData(gl.UNIFORM_BUFFER, 0, viewMatrix);
  gl.bufferSubData(gl.UNIFORM_BUFFER, 64, projectionMatrix);
  ```
- Помните про std140 выравнивание: вектор занимает 16 байт, даже если vec3.

## Текстуры: загрузка и форматы

- Классический поток:
  ```js
  const texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture);
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0,
                gl.RGBA, gl.UNSIGNED_BYTE, new Uint8Array([0, 0, 0, 255]));

  const image = new Image();
  image.src = "texture.png";
  image.onload = () => {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA,
                  gl.UNSIGNED_BYTE, image);
    gl.generateMipmap(gl.TEXTURE_2D);
  };
  ```
- Для NPOT (non power of two) текстур отключайте мипы и используйте `gl.CLAMP_TO_EDGE`.
- WebGL 2: `gl.texStorage2D` для immutable storage и `gl.texSubImage2D` для обновлений.
- Float текстуры: требуется `OES_texture_float` (WebGL 1) или `gl.FLOAT` в WebGL 2 + `EXT_color_buffer_float` для рендера в float.
- Линейные фильтры:
  ```js
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
  ```

## Framebuffer и постобработка

- Создание FBO:
  ```js
  const fbo = gl.createFramebuffer();
  gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);

  const colorTex = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, colorTex);
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0,
                gl.RGBA, gl.UNSIGNED_BYTE, null);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
  gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0,
                          gl.TEXTURE_2D, colorTex, 0);

  const depth = gl.createRenderbuffer();
  gl.bindRenderbuffer(gl.RENDERBUFFER, depth);
  gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, width, height);
  gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT,
                             gl.RENDERBUFFER, depth);

  if (gl.checkFramebufferStatus(gl.FRAMEBUFFER) !== gl.FRAMEBUFFER_COMPLETE) {
    throw new Error("FBO incomplete");
  }
  ```
- Используйте `gl.invalidateFramebuffer` (WebGL 2) чтобы подсказать GPU очистить вложенные буферы.
- Для множественных рендер-таргетов (`MRT`) включайте `WEBGL_draw_buffers` или WebGL 2 `drawBuffers`.

## Работа с данными времени исполнения

- Динамическое обновление буфера:
  ```js
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  gl.bufferSubData(gl.ARRAY_BUFFER, 0, updatedArray);
  ```
- Pixel Buffer Object (WebGL 2) ускоряет чтение/запись текстур.
- Чтение пикселей:
  ```js
  const pixels = new Uint8Array(width * height * 4);
  gl.readPixels(0, 0, width, height, gl.RGBA, gl.UNSIGNED_BYTE, pixels);
  ```
- Для скриншотов из FBO переворачивайте по Y (WebGL хранит снизу вверх).

## Организация ресурсов

- Инкапсулируйте создание объектов в классы/функции:
  ```js
  function createProgram(gl, vsSource, fsSource) { /* ... */ }
  function createTexture2D(gl, options) { /* ... */ }
  ```
- Используйте TWGL/Tiny-обёртки для работы с uniform/attrib (`twgl.setBuffersAndAttributes`, `twgl.setUniforms`).
- Отпускайте ресурсы: `gl.deleteBuffer`, `gl.deleteTexture`, `gl.deleteFramebuffer`, когда они больше не нужны.
