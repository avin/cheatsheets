## Генерация примитивов на CPU

- Плоскость (quad) для полноэкранного рендера:
  ```js
  const quadVertices = new Float32Array([
    -1, -1, 0, 0,
     1, -1, 1, 0,
    -1,  1, 0, 1,
    -1,  1, 0, 1,
     1, -1, 1, 0,
     1,  1, 1, 1,
  ]);
  // pos.xy, uv.xy
  ```
- Сфера (UV sphere):
  ```js
  function createSphere(latBands, lonBands, radius = 1) {
    const positions = [];
    const normals = [];
    const uvs = [];
    const indices = [];
    for (let lat = 0; lat <= latBands; lat++) {
      const theta = (lat * Math.PI) / latBands;
      const sinTheta = Math.sin(theta);
      const cosTheta = Math.cos(theta);
      for (let lon = 0; lon <= lonBands; lon++) {
        const phi = (lon * 2 * Math.PI) / lonBands;
        const sinPhi = Math.sin(phi);
        const cosPhi = Math.cos(phi);
        const x = cosPhi * sinTheta;
        const y = cosTheta;
        const z = sinPhi * sinTheta;
        positions.push(radius * x, radius * y, radius * z);
        normals.push(x, y, z);
        uvs.push(lon / lonBands, 1 - lat / latBands);
      }
    }
    for (let lat = 0; lat < latBands; lat++) {
      for (let lon = 0; lon < lonBands; lon++) {
        const first = lat * (lonBands + 1) + lon;
        const second = first + lonBands + 1;
        indices.push(first, second, first + 1);
        indices.push(second, second + 1, first + 1);
      }
    }
    return { positions, normals, uvs, indices };
  }
  ```
- Тор, ленты, кривые — вычисляйте позиции, нормали и UV на CPU и затем загружайте в VBO.

## Icosahedron/icosphere

- Начинайте с икосаэдра (12 вершин, 20 граней), рекурсивно подразделяйте.
- Псевдокод:
  ```js
  function subdivide(vertices, indices, iterations) {
    const midpointCache = new Map();
    const newIndices = [];
    function midpoint(i0, i1) {
      const key = i0 < i1 ? `${i0}_${i1}` : `${i1}_${i0}`;
      if (midpointCache.has(key)) return midpointCache.get(key);
      const v0 = vertices.slice(i0 * 3, i0 * 3 + 3);
      const v1 = vertices.slice(i1 * 3, i1 * 3 + 3);
      const m = normalize([
        (v0[0] + v1[0]) / 2,
        (v0[1] + v1[1]) / 2,
        (v0[2] + v1[2]) / 2,
      ]);
      vertices.push(...m);
      const index = vertices.length / 3 - 1;
      midpointCache.set(key, index);
      return index;
    }
    for (let i = 0; i < indices.length; i += 3) {
      let [a, b, c] = indices.slice(i, i + 3);
      const ab = midpoint(a, b);
      const bc = midpoint(b, c);
      const ca = midpoint(c, a);
      newIndices.push(a, ab, ca, b, bc, ab, c, ca, bc, ab, bc, ca);
    }
    return iterations > 1 ? subdivide(vertices, newIndices, iterations - 1) : { vertices, indices: newIndices };
  }
  ```
- Нормали совпадают с позициями, если сфера радиуса 1.

## gl_VertexID и процедурные фигуры

- В WebGL 2 можно создавать геометрию без VBO — fullscreen триангл:
  ```glsl
  #version 300 es
  const vec2 positions[3] = vec2[3](
    vec2(-1.0, -1.0),
    vec2(3.0, -1.0),
    vec2(-1.0, 3.0)
  );

  void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
  }
  ```
- Индексация на основе `gl_VertexID` позволяет создавать решётки, например:
  ```glsl
  vec2 grid(int vertexId, int width) {
    int x = vertexId % width;
    int y = vertexId / width;
    return vec2(float(x) / float(width - 1), float(y) / float(width - 1));
  }
  ```
- Для instancing используйте `gl_InstanceID` и матрицы массива uniform (если нет `ANGLE_instanced_arrays`).

## Instancing без расширений

- Передайте transform-матрицы через текстуру или uniform массив:
  ```glsl
  uniform mat4 uTransforms[MAX_INSTANCES];
  void main() {
    mat4 model = uTransforms[gl_InstanceID];
    gl_Position = projection * view * model * vec4(position, 1.0);
  }
  ```
- На CPU вызывайте `drawArrays`/`drawElements` в цикле и меняйте uniform `uModel`, но объединяйте объекты одинакового материала.
- С `ANGLE_instanced_arrays` (WebGL 1) и WebGL 2 есть `drawArraysInstanced`, `vertexAttribDivisor`.

## Morph targets и skinning

- Храните несколько позиций в разных атрибутах:
  ```glsl
  in vec3 positionA;
  in vec3 positionB;
  uniform float morph;
  void main() {
    vec3 pos = mix(positionA, positionB, morph);
    gl_Position = projection * view * vec4(pos, 1.0);
  }
  ```
- Skinning: храните веса и индексы, если делаете skeletal animation вручную. Для компактности используйте `ivec4` и `float` веса, матрицы костей передавайте в uniform массив.

## Procedural geometry на GPU

- Используйте vertex текстуры:
  ```glsl
  uniform sampler2D heightMap;
  void main() {
    vec2 uv = position.xz;
    float h = texture(heightMap, uv).r;
    vec3 displaced = vec3(position.x, h, position.z);
    gl_Position = projection * view * vec4(displaced, 1.0);
  }
  ```
- Для сложных структур применяйте transform feedback (WebGL 2) — позволяет вычислять новую геометрию в вершинном шейдере и записывать её в буфер.

## Топология и ленты

- Генерируйте линии как треугольники с шириной:
  ```js
  function polyline(points, width) {
    // вычислите нормали сегментов и создайте два треугольника на сегмент
  }
  ```
- Используйте `degenerate triangles` для соединения сегментов в TRIANGLE_STRIP.

## Чек-лист по геометрии

- Нормали должны быть нормализованы (`normalize`).
- Тангенты/битангенты нужны для normal mapping (храните в атрибутах).
- Для UV следите за seamless-разрезами (починить можно через атласы).
- При генерации убедитесь, что winding order согласован с `cullFace`.
