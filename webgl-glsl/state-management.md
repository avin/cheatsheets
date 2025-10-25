## Управление состояниями

- WebGL хранит глобальный state: минимизируйте переключения.
- Частые состояния:
  - `gl.enable(gl.DEPTH_TEST)` и `gl.depthFunc(gl.LEQUAL)` для 3D.
  - `gl.enable(gl.BLEND)` с `gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)` для прозрачностей.
  - `gl.disable(gl.CULL_FACE)` для двумерной графики; иначе `gl.cullFace(gl.BACK)` + `gl.frontFace(gl.CCW)`.
  - `gl.enable(gl.SCISSOR_TEST)` для частичного обновления экрана.
- Для очистки задавайте маски один раз: `gl.clearColor`, `gl.clearDepth`, `gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)`.
- Храните настройки в собственном объекте (`currentState`) и сравнивайте перед вызовами, чтобы избежать лишних `enable/disable`.

## Сохранение и восстановление стейта

- WebGL не предоставляет стек состояний, но вы можете реализовать менеджер:
  ```js
  const stateStack = [];
  function pushState(gl) {
    stateStack.push({
      program: gl.getParameter(gl.CURRENT_PROGRAM),
      arrayBuffer: gl.getParameter(gl.ARRAY_BUFFER_BINDING),
      elementBuffer: gl.getParameter(gl.ELEMENT_ARRAY_BUFFER_BINDING),
      framebuffer: gl.getParameter(gl.FRAMEBUFFER_BINDING),
      vao: gl.getParameter(gl.VERTEX_ARRAY_BINDING),
    });
  }

  function popState(gl) {
    const state = stateStack.pop();
    if (!state) return;
    gl.useProgram(state.program);
    gl.bindBuffer(gl.ARRAY_BUFFER, state.arrayBuffer);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, state.elementBuffer);
    gl.bindFramebuffer(gl.FRAMEBUFFER, state.framebuffer);
    gl.bindVertexArray?.(state.vao ?? null);
  }
  ```
- Сбрасывайте привязки после рендера: `gl.bindTexture(gl.TEXTURE_2D, null)` чтобы избежать случайных side effects.

## Потеря контекста

- Подписывайтесь на события:
  ```js
  canvas.addEventListener("webglcontextlost", (e) => {
    e.preventDefault(); // запретить браузеру восстанавливать автоматически
    cancelAnimationFrame(loopHandle);
  });
  canvas.addEventListener("webglcontextrestored", () => {
    initResources(); // пересоздание буферов/текстур/программ
    loopHandle = requestAnimationFrame(frame);
  });
  ```
- Храните исходные данные CPU-side, чтобы оперативно пересоздать GPU-объекты.
- Используйте `gl.isContextLost()` внутри рендера для раннего выхода.

## Стратегии batch-рендера

- Группируйте draw calls по:
  - программе (`useProgram`),
  - текстуре (`bindTexture`),
  - состояниям (`blend`, `depth`, `cull`).
- Используйте атласы текстур и uniform массивы, чтобы делать меньше переключений.
- Сортируйте объекты перед рендером: сначала непрозрачные (front-to-back для depth); затем прозрачные (back-to-front).
- Применяйте `gl.finish()` только для детерминированных измерений; обычно достаточно `gl.flush()`.

## Лимиты и капы

- Узнайте аппаратные ограничения заранее:
  ```js
  const limits = {
    attributes: gl.getParameter(gl.MAX_VERTEX_ATTRIBS),
    varying: gl.getParameter(gl.MAX_VARYING_VECTORS),
    textureUnits: gl.getParameter(gl.MAX_COMBINED_TEXTURE_IMAGE_UNITS),
    textureSize: gl.getParameter(gl.MAX_TEXTURE_SIZE),
    drawBuffers: gl.getParameter(gl.MAX_DRAW_BUFFERS) ?? 1,
  };
  ```
- Для мобильных устройств уменьшайте количество `varying` и избегайте больших uniform-массивов.
- Лимиты `gl.MAX_VERTEX_TEXTURE_IMAGE_UNITS` критичны для вершинных шейдеров, работающих с текстурами (например, displacement map).

## Draw call budget

- В браузерах ориентируйтесь на ≤ 1–2 тысячи draw calls в кадр (для сложных сцен — меньше). Переносите логические партии в instancing.
- Для 2D используйте подход «заполнить большой VBO» и `gl.drawArrays(gl.TRIANGLES, offset, count)`.
- Применяйте transform feedback (WebGL 2) для обновления геометрии на GPU без CPU round-trip.

## Логирование и профилирование

- Воспользуйтесь Spector.js для анализа состояния и draw calls.
- Встроенный `EXT_disjoint_timer_query` помогает измерять GPU-время.
- `console.table(limits)` — чтобы видеть важные параметры при старте.

## State leak чек-лист

- После постпроцессинга возвращайте `gl.bindFramebuffer(gl.FRAMEBUFFER, null)`.
- Помните про разные биндинги для `TEXTURE_2D`, `TEXTURE_CUBE_MAP`, `TEXTURE_2D_ARRAY`.
- Если используете VAO, отключение `gl.bindVertexArray(null)` обязательно перед настройкой нового набора атрибутов.
- Не забывайте обновлять viewport при рендере в FBO: `gl.viewport(0, 0, fboWidth, fboHeight)`.
