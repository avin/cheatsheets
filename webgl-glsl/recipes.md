## Матрицы камеры и проекции

- lookAt:
  ```js
  function lookAt(eye, center, up) {
    const z = normalize(subtract(eye, center));
    const x = normalize(cross(up, z));
    const y = cross(z, x);
    return [
      x[0], y[0], z[0], 0,
      x[1], y[1], z[1], 0,
      x[2], y[2], z[2], 0,
      -dot(x, eye), -dot(y, eye), -dot(z, eye), 1,
    ];
  }
  ```
- Perspective:
  ```js
  function perspective(fov, aspect, near, far) {
    const f = 1.0 / Math.tan(fov / 2);
    const nf = 1 / (near - far);
    return [
      f / aspect, 0, 0, 0,
      0, f, 0, 0,
      0, 0, (far + near) * nf, -1,
      0, 0, (2 * far * near) * nf, 0,
    ];
  }
  ```
- Orbit камера:
  ```js
  function orbit(radius, theta, phi) {
    const x = radius * Math.sin(phi) * Math.cos(theta);
    const y = radius * Math.cos(phi);
    const z = radius * Math.sin(phi) * Math.sin(theta);
    return [x, y, z];
  }
  ```

## Глубина и блендинг

- Настройка depth:
  ```js
  gl.enable(gl.DEPTH_TEST);
  gl.depthFunc(gl.LEQUAL);
  gl.clearDepth(1.0);
  gl.depthMask(true);
  ```
- Для прозрачных слоёв:
  ```js
  gl.enable(gl.BLEND);
  gl.blendFuncSeparate(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA, gl.ONE, gl.ONE_MINUS_SRC_ALPHA);
  gl.depthMask(false);
  // draw transparent
  gl.depthMask(true);
  gl.disable(gl.BLEND);
  ```
- Order-independent transparency (ping-pong accumulation):
  ```glsl
  // accum pass
  out vec4 accum;
  out vec4 reveal;
  accum = vec4(color.rgb * alpha, alpha);
  reveal = vec4(alpha);
  ```
- Используйте weighted blended OIT (McGuire):
  ```glsl
  float weight = clamp(pow(alpha + 0.01, 3.0) * 1e3, 1e-2, 3e3);
  accum.rgb += color.rgb * alpha * weight;
  accum.a += alpha;
  revealFactor *= 1.0 - alpha;
  ```

## JS ↔ GLSL хелперы

- Унифицированная загрузка шейдеров:
  ```js
  async function loadShader(url) {
    const res = await fetch(url);
    return res.text();
  }
  ```
- Компиляция:
  ```js
  function createProgram(gl, vsSource, fsSource) {
    function compile(type, source) {
      const shader = gl.createShader(type);
      gl.shaderSource(shader, source);
      gl.compileShader(shader);
      if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        throw new Error(gl.getShaderInfoLog(shader) ?? "shader error");
      }
      return shader;
    }
    const program = gl.createProgram();
    gl.attachShader(program, compile(gl.VERTEX_SHADER, vsSource));
    gl.attachShader(program, compile(gl.FRAGMENT_SHADER, fsSource));
    gl.linkProgram(program);
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
      throw new Error(gl.getProgramInfoLog(program) ?? "link error");
    }
    return program;
  }
  ```
- Униформ сеттер:
  ```js
  function setUniform(gl, program, name, value) {
    const loc = gl.getUniformLocation(program, name);
    if (typeof value === "number") gl.uniform1f(loc, value);
    else if (value.length === 2) gl.uniform2fv(loc, value);
    else if (value.length === 3) gl.uniform3fv(loc, value);
    else if (value.length === 4) gl.uniform4fv(loc, value);
    else if (value.length === 9) gl.uniformMatrix3fv(loc, false, value);
    else if (value.length === 16) gl.uniformMatrix4fv(loc, false, value);
  }
  ```

## Работа с текстурами

- Загрузка из canvas/bitmap:
  ```js
  async function textureFromImage(gl, url) {
    const image = await loadImage(url);
    const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
    gl.generateMipmap(gl.TEXTURE_2D);
    return texture;
  }
  ```
- LUT 2D → 3D:
  ```glsl
  vec3 lookup2DLut(sampler2D lut, vec3 color, float size) {
    float sliceSize = 1.0 / size;
    float slicePixelSize = sliceSize / size;
    float sliceInnerSize = slicePixelSize * (size - 1.0);
    float zSlice = floor(color.b * (size - 1.0));
    float zOffset = zSlice * sliceSize;
    vec2 uv = vec2(
      zOffset + color.r * sliceInnerSize + slicePixelSize * 0.5,
      color.g * sliceInnerSize + slicePixelSize * 0.5
    );
    return texture(lut, uv).rgb;
  }
  ```

## WebGL ES версии

- GLSL ES 1.00 (WebGL 1):
  ```glsl
  attribute vec3 position;
  varying vec2 vUv;
  void main() {
    vUv = uv;
    gl_Position = projectionMatrix * vec4(position, 1.0);
  }
  ```
- GLSL ES 3.00 (WebGL 2):
  ```glsl
  #version 300 es
  layout(location = 0) in vec3 position;
  layout(location = 1) in vec2 uv;
  out vec2 vUv;
  void main() {
    vUv = uv;
    gl_Position = projectionMatrix * vec4(position, 1.0);
  }
  ```
- Различия: `texture2D` → `texture`, `gl_FragColor` → пользовательский `out`, возможность `layout` для атрибутов.

## Работа с индексами

- Создавайте `Uint16Array`/`Uint32Array` в зависимости от поддержки.
- Если используете `drawElements`, всегда подключайте `ELEMENT_ARRAY_BUFFER` перед `draw`.

## Конвертация координат

- NDC → экран:
  ```js
  function ndcToScreen(ndc, width, height) {
    return [
      (ndc[0] * 0.5 + 0.5) * width,
      (1.0 - (ndc[1] * 0.5 + 0.5)) * height,
    ];
  }
  ```
- Screen → clip:
  ```js
  function screenToClip(x, y, width, height) {
    return [
      (x / width) * 2 - 1,
      (1 - y / height) * 2 - 1,
    ];
  }
  ```

## Математические утилиты

- `rotateY` матрица:
  ```js
  function rotateY(angle) {
    const c = Math.cos(angle);
    const s = Math.sin(angle);
    return [
      c, 0, s, 0,
      0, 1, 0, 0,
      -s, 0, c, 0,
      0, 0, 0, 1,
    ];
  }
  ```
- Quat → матрица (для вращений):
  ```js
  function quatToMatrix(q) {
    const [x, y, z, w] = q;
    const xx = x * x, yy = y * y, zz = z * z;
    const xy = x * y, xz = x * z, yz = y * z;
    const wx = w * x, wy = w * y, wz = w * z;
    return [
      1 - 2 * (yy + zz), 2 * (xy + wz), 2 * (xz - wy), 0,
      2 * (xy - wz), 1 - 2 * (xx + zz), 2 * (yz + wx), 0,
      2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (xx + yy), 0,
      0, 0, 0, 1,
    ];
  }
  ```

## Проверка ошибок

- В режиме разработки:
  ```js
  const error = gl.getError();
  if (error !== gl.NO_ERROR) console.warn("WebGL error", error);
  ```
- Коды: `1280` (`INVALID_ENUM`), `1281` (`INVALID_VALUE`), `1282` (`INVALID_OPERATION`).

## Минимальный шаблон full-screen quad

```js
const vao = gl.createVertexArray();
gl.bindVertexArray(vao);
gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
```

```glsl
#version 300 es
out vec2 vUv;

void main() {
  vec2 positions[4] = vec2[4](
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
  );
  vec2 uvs[4] = vec2[4](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0)
  );
  vUv = uvs[gl_VertexID];
  gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
```
