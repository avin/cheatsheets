## Синтаксис и типы

- GLSL ES 1.00 (WebGL 1) и GLSL ES 3.00 (WebGL 2) — строго типизированные языки.
- Скаляры: `float`, `int`, `bool`. Векторы: `vec2`, `vec3`, `vec4`, `ivec`, `bvec`. Матрицы: `mat2`, `mat3`, `mat4`.
- Компоненты доступны через `.x/.y/.z/.w`, `.r/.g/.b/.a`, `.s/.t/.p/.q` — можно делать `vec3 color = textureColor.rgb;`.
- Кастинг: `vec3(position)` преобразует `position` скаляр к вектору.
- Точность (`precision`) обязательна в фрагментном шейдере WebGL 1:
  ```glsl
  precision highp float;
  precision mediump sampler2D;
  ```
- В WebGL 2 версии шейдера начинаются с `#version 300 es`, атрибуты обозначаются `in/out`.

## Структура вершинного и фрагментного шейдера

```glsl
// vertex.glsl
#version 300 es
precision highp float;

in vec3 position;
in vec2 uv;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 vUv;

void main() {
  vUv = uv;
  gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
}
```

```glsl
// fragment.glsl
#version 300 es
precision highp float;

in vec2 vUv;
uniform sampler2D uTexture;
out vec4 outColor;

void main() {
  vec3 color = texture(uTexture, vUv).rgb;
  outColor = vec4(color, 1.0);
}
```

- В WebGL 1 замените `in` на `attribute`, `out` (вершинного) → `varying` и фрагментного `outColor` → `gl_FragColor`.

## Передача данных между шейдерами

- Вершинный → фрагментный:
  ```glsl
  out vec3 vNormal;
  // ...
  vNormal = mat3(uNormalMatrix) * normal;
  ```
  ```glsl
  in vec3 vNormal;
  ```
- Для WebGL 1 — `varying vec3 vNormal;`.
- Количество `varying` ограничено (`MAX_VARYING_VECTORS`).

## Униформы и встроенные функции

- Униформы имеют глобальную область:
  ```glsl
  uniform float uTime;
  uniform vec2 uResolution;
  uniform mat4 uModelViewProjection;
  ```
- Встроенные математические функции: `sin`, `cos`, `tan`, `abs`, `sign`, `floor`, `ceil`, `fract`, `mod`, `min`, `max`, `pow`, `sqrt`.
- Векторные операции работают поэлементно: `vec3 light = normalize(vec3(1.0, 0.8, 0.6));`.
- Смешивание: `mix(a, b, t)`, плавная интерполяция `smoothstep(edge0, edge1, x)`.
- Для работы с шумами — готовые функции (например, от Inigo Quilez) включаются вручную.

## Пространства координат

- Pipeline координаты:
  1. **Model space** — локальные координаты.
  2. **World space** — после умножения на `model`.
  3. **View space** — умножение на матрицу камеры (`view`).
  4. **Clip space** — `projection`, результат попадает в `gl_Position`.
  5. **NDC** — после деления на `w`.
  6. **Screen space** — после viewport transform.
- Для нормалей используйте `mat3(transpose(inverse(mat4)))`.
- Пример:
  ```glsl
  mat4 modelView = uView * uModel;
  vec4 worldPos = uModel * vec4(position, 1.0);
  vec3 normal = normalize(mat3(uNormalMatrix) * inNormal);
  gl_Position = uProjection * modelView * vec4(position, 1.0);
  ```

## Текстуры и координаты

- Семплинг:
  ```glsl
  uniform sampler2D uTex;
  vec4 tex = texture(uTex, vUv);
  ```
- Для WebGL 1 названия функции:
  - `texture2D` вместо `texture`.
  - Для кубических текстур — `textureCube`.
- Координаты `vec2` обычно в диапазоне [0,1]. Для тайлинга используйте `repeat` и `fract`.
- Свертка: `dFdx`, `dFdy`, `fwidth` (WebGL 2) помогают делать антиалиасинг.

## Условия и циклы

- В GLSL доступны `if`, `for`, `while`, но компилятор может разворачивать циклы, если они имеют статическую длину.
- Для WebGL 1 циклы должны иметь фиксированные границы, иначе компилятор может не оптимизировать.
- Пример:
  ```glsl
  vec3 accumulate = vec3(0.0);
  for (int i = 0; i < 4; i++) {
    float t = float(i) / 4.0;
    accumulate += vec3(t, sin(uTime + t), cos(uTime - t));
  }
  ```
- Избегайте ранних выходов из цикла, если целитесь в совместимость со старыми GPU.

## Прецизионные классы

- `lowp` — 8 бит (цвета).
- `mediump` — 16 бит (векторы, UV).
- `highp` — 32 бит (позиции, нормали, вычисления).
- На некоторых мобильных GPU `highp` недоступен во фрагментных шейдерах (используйте `getShaderPrecisionFormat`).

## Правила GLSL ES

- Нельзя использовать динамические массивы без фиксированного размера (WebGL 1).
- Нет рекурсии и указателей.
- Нельзя делать `int` → `bool` неявно.
- `discard` прерывает выполнение фрагмента и отключает запись в буферы.

## Диагностика шейдеров

- Проверяйте логи компиляции:
  ```js
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    console.error(gl.getShaderInfoLog(shader));
  }
  ```
- Для GLSL ES 3.00 указывайте `#version 300 es` первой строкой, иначе компилятор проигнорирует.
- Включайте `#define` для feature flags.
