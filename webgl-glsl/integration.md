## Видео и потоковые текстуры

- Используйте элемент `<video>` или MediaStream:
  ```js
  const video = document.createElement("video");
  video.autoplay = true;
  video.loop = true;
  video.src = "demo.mp4";
  await video.play();

  const texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
  ```
- Обновляйте кадр:
  ```js
  function updateVideoTexture() {
    if (video.readyState >= video.HAVE_CURRENT_DATA) {
      gl.bindTexture(gl.TEXTURE_2D, texture);
      gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, video);
    }
  }
  ```
- Для webcam:
  ```js
  const stream = await navigator.mediaDevices.getUserMedia({ video: true });
  video.srcObject = stream;
  ```
- Используйте `texSubImage2D` + `gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);` для корректной ориентации.

## OffscreenCanvas и Web Workers

- OffscreenCanvas позволяет рендерить вне основного потока:
  ```js
  const offscreen = canvas.transferControlToOffscreen();
  worker.postMessage({ canvas: offscreen }, [offscreen]);
  ```
- В воркере:
  ```js
  self.onmessage = (e) => {
    const gl = e.data.canvas.getContext("webgl2");
    init(gl);
    function render() { /* ... */ self.requestAnimationFrame(render); }
    render();
  };
  ```
- Передавайте uniform через `SharedArrayBuffer` или `postMessage`.
- Ограничения: нет доступа к DOM, нужно вручную обрабатывать события ввода (передавать из main thread).

## Web Audio API интеграция

- Создайте аудио-контекст:
  ```js
  const audioCtx = new AudioContext();
  const analyser = audioCtx.createAnalyser();
  analyser.fftSize = 2048;
  const data = new Uint8Array(analyser.frequencyBinCount);
  ```
- Подключите источник:
  ```js
  const source = audioCtx.createMediaElementSource(audioElement);
  source.connect(analyser);
  analyser.connect(audioCtx.destination);
  ```
- На каждом кадре:
  ```js
  analyser.getByteFrequencyData(data);
  const bass = average(data.slice(0, 32)) / 255;
  const mids = average(data.slice(32, 128)) / 255;
  const highs = average(data.slice(128)) / 255;
  gl.uniform3f(uAudioBands, bass, mids, highs);
  ```
- Для time-domain (`getByteTimeDomainData`) получайте форму волны.
- Используйте beat detection (например, `detect-peaks`) для синхронизации.

## MIDI и внешние контроллеры

- Web MIDI API:
  ```js
  const midiAccess = await navigator.requestMIDIAccess();
  midiAccess.inputs.forEach((input) => {
    input.onmidimessage = (event) => {
      const [status, note, velocity] = event.data;
      // маппинг на uniform
    };
  });
  ```
- Преобразуйте значения (0–127) в [0,1] и передавайте в шейдеры.

## Управление вводом

- Мышь:
  ```js
  const pointer = { x: 0, y: 0, down: false };
  canvas.addEventListener("pointermove", (e) => {
    pointer.x = e.offsetX / canvas.clientWidth;
    pointer.y = 1.0 - e.offsetY / canvas.clientHeight;
  });
  canvas.addEventListener("pointerdown", () => pointer.down = true);
  canvas.addEventListener("pointerup", () => pointer.down = false);
  ```
- Передавайте в uniform `vec4(uMouse);` где `zw` = нажатие.
- Пик-пик (object picking):
  1. Рендерьте сцену в FBO с уникальными цветовыми ID.
  2. Используйте `gl.readPixels(mouseX, height - mouseY, 1, 1, gl.RGBA, gl.UNSIGNED_BYTE, pixel)`.

## UI и панели

- Простые инструменты: dat.GUI, Tweakpane, ctrl-panel — легкие UI для настройки параметров.
- Под капотом: храните объект параметров и обновляйте uniform после изменения.
- Для записи пресетов сохраняйте JSON с параметрами и seed.

## Связь между сценами

- Передавайте результат одного контекста в другой через `ImageBitmap`:
  ```js
  const bitmap = await createImageBitmap(canvas);
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA,
                gl.UNSIGNED_BYTE, bitmap);
  bitmap.close();
  ```
- Используйте `postMessage` для синхронизации нескольких воркеров.

## Доставка контента

- Настраивайте `OffscreenCanvas` + `ReadableStream` для live-видеовывода (WebCodecs).
- Экспорт кадров: `gl.readPixels` → `ImageData` → `createImageBitmap`.
- Для записи видео: `canvas.captureStream()` + `MediaRecorder`, собирать Blob.
