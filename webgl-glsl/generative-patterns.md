## Палитры и цветовые приёмы

- Используйте палитры по Шликсу (IQ):
  ```glsl
  vec3 schlickPalette(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
    return a + b * cos(6.28318 * (c * t + d));
  }
  vec3 color = schlickPalette(dist, vec3(0.5), vec3(0.5), vec3(1.0), vec3(0.263, 0.416, 0.557));
  ```
- HSV ↔ RGB:
  ```glsl
  vec3 hsv2rgb(vec3 c) {
    vec3 p = abs(fract(c.xxx + vec3(0.0, 1.0/3.0, 2.0/3.0)) * 6.0 - 3.0);
    return c.z * mix(vec3(1.0), clamp(p - 1.0, 0.0, 1.0), c.y);
  }
  ```
- Создавайте палитры из beat-синхронизации: `float hue = fract(uTime * 0.1 + beatPhase);`.

## Глич и сканлайны

- Offset RGB:
  ```glsl
  vec2 uv = vUv;
  float glitch = step(0.95, fract(sin(dot(uv * uTime, vec2(12.9898, 78.233))) * 43758.5453123));
  vec3 color;
  color.r = texture(uScene, uv + vec2(glitch * 0.005, 0.0)).r;
  color.g = texture(uScene, uv).g;
  color.b = texture(uScene, uv - vec2(glitch * 0.005, 0.0)).b;
  ```
- Scanlines:
  ```glsl
  float scan = sin(vUv.y * 800.0) * 0.025;
  color *= 1.0 - scan;
  ```
- VHS distort:
  ```glsl
  float noise = texture(uNoise, vec2(vUv.y * 0.5, uTime * 0.1)).r;
  uv.x += noise * 0.02;
  ```

## Частицы и поля

- Частицы через текстуру-состояние (`particlePosTex`) — обновление в шейдере с ping-pong и отображение точками.
- Позиция = texel, цвет = скорость:
  ```glsl
  vec4 particle = texture(uParticleState, vUv);
  vec3 pos = particle.xyz;
  ```
- Градиентное поле:
  ```glsl
  vec3 field(vec3 p) {
    vec3 attractor = vec3(0.0, sin(uTime), 0.0);
    return normalize(attractor - p) * 0.1;
  }
  ```
- Метаболлы:
  ```glsl
  float metaball(vec3 p, vec3 center, float radius) {
    return radius / length(p - center);
  }
  float iso = metaball(p, vec3(0.5, 0.0, 0.0), 1.0) + metaball(p, vec3(-0.5, 0.0, 0.0), 1.0);
  ```

## Реакция-диффузия и клеточные автоматы

- Реакция-диффузия (см. `glsl-creative.md`) — используйте различные параметры feed/kill для разнообразия.
- Game of Life:
  ```glsl
  int neighbors = 0;
  for (int y = -1; y <= 1; y++) {
    for (int x = -1; x <= 1; x++) {
      if (x == 0 && y == 0) continue;
      neighbors += int(texture(uPrevState, vUv + vec2(x, y) * texel).r > 0.5);
    }
  }
  float alive = texture(uPrevState, vUv).r;
  float next = (alive > 0.5 && (neighbors == 2 || neighbors == 3)) || (alive < 0.5 && neighbors == 3) ? 1.0 : 0.0;
  ```
- Используйте случайные seeds и усреднение для плавных эффектов.

## Реалистичные материалы

- Fake PBR:
  ```glsl
  vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
  }

  vec3 cookTorrance(vec3 N, vec3 V, vec3 L, vec3 albedo, float metallic, float roughness) {
    vec3 H = normalize(V + L);
    float NDF = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / 3.14159 + specular) * NdotL;
  }
  ```
- Для демо можно упростить: экспоненты, glint, rim lighting:
  ```glsl
  float rim = pow(1.0 - max(dot(normal, viewDir), 0.0), 4.0);
  color += rim * vec3(0.4, 0.5, 1.0);
  ```

## 2D генеративные шейдеры

- Векторные поля:
  ```glsl
  vec2 flow(vec2 uv) {
    float angle = fbm(uv * 3.0 + uTime * 0.2) * 6.28318;
    return vec2(cos(angle), sin(angle));
  }
  vec2 pos = uv;
  for (int i = 0; i < 20; i++) {
    pos += flow(pos) * 0.005;
  }
  ```
- Lissajous фигуры:
  ```glsl
  vec2 lissajous(float t, float a, float b, float delta) {
    return vec2(sin(a * t + delta), sin(b * t));
  }
  ```
- Аудиореактивные линии: множьте амплитуду на значение частоты, полученное из Web Audio.

## Таймлайн и сцены

- Структурируйте демо:
  ```glsl
  float scene(float t) {
    if (t < 10.0) return sceneIntro();
    if (t < 20.0) return sceneParticles();
    return sceneOutro();
  }
  ```
- Используйте `smoothstep` для blend между сценами.
- Передавайте `uniform float uSceneId` чтобы переключать логики без веток.

## Финальный гайд

- Комбинируйте несколько слоёв: фон → среда → foreground → постпроцесс.
- Работайте с ограничениями: 1 draw call для full-screen quad + шейдеры = мгновенные итерации.
- Сохраняйте параметры (случайные seed, палитры) чтобы воспроизводить конкретные рендеры.
