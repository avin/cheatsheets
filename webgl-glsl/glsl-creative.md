## Signed Distance Fields и raymarching

- SDF описывает расстояние до поверхности: `float sdSphere(vec3 p, float r) { return length(p) - r; }`.
- Сцена составляется комбинациями:
  ```glsl
  float opUnion(float d1, float d2) { return min(d1, d2); }
  float opSubtraction(float d1, float d2) { return max(-d1, d2); }
  float opIntersection(float d1, float d2) { return max(d1, d2); }
  ```
- Raymarch:
  ```glsl
  vec3 rayDirection(float fov, vec2 fragCoord, vec2 resolution) {
    vec2 xy = fragCoord - resolution * 0.5;
    float z = resolution.y / tan(radians(fov) * 0.5);
    return normalize(vec3(xy, -z));
  }

  float sceneSDF(vec3 p) {
    float sphere = sdSphere(p, 1.0);
    float box = sdBox(p - vec3(1.5, 0.0, 0.0), vec3(0.6));
    return opUnion(sphere, box);
  }

  vec3 estimateNormal(vec3 p) {
    const vec2 e = vec2(1.0, -1.0) * 0.5773 * 0.0005;
    return normalize(vec3(
      e.xyy * sceneSDF(p + e.xyy) +
      e.yyx * sceneSDF(p + e.yyx) +
      e.yxy * sceneSDF(p + e.yxy) +
      e.xxx * sceneSDF(p + e.xxx)
    ));
  }

  float raymarch(vec3 ro, vec3 rd) {
    float t = 0.0;
    for (int i = 0; i < 128; i++) {
      vec3 p = ro + rd * t;
      float d = sceneSDF(p);
      if (d < 0.0005) return t;
      t += d;
      if (t > 100.0) break;
    }
    return -1.0;
  }
  ```
- Освещение: ambient + diffuse + specular, мягкие тени через `shadow` raymarch.
- Ослабление свечения: `float fog = exp(-distance * 0.02);`.

## Процедурные текстуры и шум

- Фрактальный шум:
  ```glsl
  float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
  }

  float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
  }

  float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    mat2 rot = mat2(0.8, 0.6, -0.6, 0.8);
    for (int i = 0; i < 6; i++) {
      value += amplitude * noise(p);
      p = rot * p * 2.0;
      amplitude *= 0.5;
    }
    return value;
  }
  ```
- Simplex/Worley шум — быстрее и менее сеточный (см. реализации IQ).
- Используйте `smoothstep` на шуме для получения органических границ.
- Цветовые палитры:
  ```glsl
  vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
    return a + b * cos(6.28318 * (c * t + d));
  }
  ```
- Перлин-полосы:
  ```glsl
  float stripes(vec2 uv, float width) {
    return smoothstep(0.0, width, abs(sin(uv.x * 3.1415)));
  }
  ```

## Постпроцессинг

- Блум через двухпроходный blur:
  ```glsl
  vec4 blur(sampler2D tex, vec2 uv, vec2 resolution, vec2 direction) {
    float kernel[5];
    kernel[0] = 0.204164;
    kernel[1] = 0.304005;
    kernel[2] = 0.193783;
    kernel[3] = 0.072993;
    kernel[4] = 0.016662;
    vec3 sum = vec3(0.0);
    for (int i = -4; i <= 4; i++) {
      sum += texture(tex, uv + direction * float(i) / resolution).rgb * kernel[abs(i)];
    }
    return vec4(sum, 1.0);
  }
  ```
- Edge detection: `sobel` ядро на texture fetch.
- Chromatic aberration: семплируйте текстуру с небольшими смещениями для разных каналов.
- Film grain:
  ```glsl
  float grain(vec2 uv, float time) {
    return fract(sin(dot(uv * time, vec2(12.9898, 78.233))) * 43758.5453);
  }
  ```
- Vignette:
  ```glsl
  float vignette(vec2 uv) {
    vec2 dist = abs(uv - 0.5);
    float len = length(dist);
    return smoothstep(0.75, 0.35, len);
  }
  ```

## Фракталы и повторения

- Mandelbrot:
  ```glsl
  vec3 mandelbrot(vec2 c) {
    vec2 z = vec2(0.0);
    float iterations = 0.0;
    const int MAX_ITER = 200;
    for (int i = 0; i < MAX_ITER; i++) {
      vec2 nz = vec2(
        z.x * z.x - z.y * z.y + c.x,
        2.0 * z.x * z.y + c.y
      );
      z = nz;
      if (dot(z, z) > 4.0) {
        iterations = float(i);
        break;
      }
    }
    return palette(iterations / float(MAX_ITER),
                   vec3(0.5),
                   vec3(0.5),
                   vec3(1.0),
                   vec3(0.263, 0.416, 0.557));
  }
  ```
- Повторение пространства:
  ```glsl
  vec3 repeat(vec3 p, float period) {
    return mod(p + period * 0.5, period) - period * 0.5;
  }
  ```
- Twist:
  ```glsl
  vec3 twist(vec3 p, float amount) {
    float c = cos(amount * p.y);
    float s = sin(amount * p.y);
    mat2 m = mat2(c, -s, s, c);
    p.xz = m * p.xz;
    return p;
  }
  ```

## Анимация и тайминг

- Параметризуйте время: `uniform float uTime;`.
- Используйте `fract(uTime * speed)` для циклов.
- Easing:
  ```glsl
  float quintInOut(float t) {
    t *= 2.0;
    if (t < 1.0) return 0.5 * t * t * t * t * t;
    t -= 2.0;
    return 0.5 * (t * t * t * t * t + 2.0);
  }
  ```
- Beat sync: передавайте фазу (`phase = mod(uTime, beatLength)`).
- Morphing между сценами: `mix(sceneA, sceneB, smoothstep(start, end, uTime))`.

## Ping-pong и feedback

- Используйте два FBO, переключая их каждый кадр:
  ```js
  const targets = [createFBO(), createFBO()];
  let readIndex = 0;
  function swap() {
    readIndex = 1 - readIndex;
  }
  function render() {
    const write = targets[readIndex];
    const read = targets[1 - readIndex];
    gl.bindFramebuffer(gl.FRAMEBUFFER, write.fbo);
    gl.viewport(0, 0, write.width, write.height);
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, read.texture);
    // draw
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
    swap();
  }
  ```
- В шейдере:
  ```glsl
  uniform sampler2D uPrevFrame;
  vec4 prev = texture(uPrevFrame, vUv);
  ```
- Не забывайте делать `gl.clear` для предотвращения накопления артефактов, если это не задумано.

## Реакция-диффузия и поля

- Реакция-диффузия (Gray-Scott):
  ```glsl
  vec2 laplace(sampler2D tex, vec2 uv, vec2 texel) {
    vec2 sum = vec2(0.0);
    sum += texture(tex, uv + texel * vec2(-1.0, 0.0)).xy;
    sum += texture(tex, uv + texel * vec2(1.0, 0.0)).xy;
    sum += texture(tex, uv + texel * vec2(0.0, -1.0)).xy;
    sum += texture(tex, uv + texel * vec2(0.0, 1.0)).xy;
    sum += texture(tex, uv + texel * vec2(-1.0, -1.0)).xy;
    sum += texture(tex, uv + texel * vec2(1.0, 1.0)).xy;
    sum += texture(tex, uv + texel * vec2(-1.0, 1.0)).xy;
    sum += texture(tex, uv + texel * vec2(1.0, -1.0)).xy;
    sum -= texture(tex, uv).xy * 8.0;
    return sum;
  }

  vec4 grayScott(vec2 uv, sampler2D tex, vec2 texel, float feed, float kill, vec2 diff) {
    vec2 state = texture(tex, uv).xy;
    vec2 lap = laplace(tex, uv, texel);
    float A = state.x;
    float B = state.y;
    float reaction = A * B * B;
    float dA = diff.x * lap.x - reaction + feed * (1.0 - A);
    float dB = diff.y * lap.y + reaction - (kill + feed) * B;
    A += dA;
    B += dB;
    return vec4(clamp(A, 0.0, 1.0), clamp(B, 0.0, 1.0), 0.0, 1.0);
  }
  ```
- Это вычисляется в ping-pong FBO, результат отображается в финальном проходе.

## Освещение и материалы

- Ламбертово освещение:
  ```glsl
  vec3 lambert(vec3 normal, vec3 lightDir, vec3 lightColor, vec3 albedo) {
    float diff = max(dot(normal, lightDir), 0.0);
    return albedo * lightColor * diff;
  }
  ```
- Фейковый specular (Blinn-Phong):
  ```glsl
  float specular(vec3 normal, vec3 lightDir, vec3 viewDir, float shininess) {
    vec3 halfDir = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfDir), 0.0), shininess);
  }
  ```
- Environment mapping (экспериментальный для SDF): `textureCube(envMap, reflect(-viewDir, normal))`.

## Цветокоррекция и финальный пас

- Gamma correction: `color = pow(color, vec3(1.0 / 2.2));`.
- ACES тонмап:
  ```glsl
  vec3 acesToneMap(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
  }
  ```
- LUT-палитры: загружайте 3D-текстуру (WebGL 2) или 2D LUT и перемаппьте цвет.
