# Рендеринг и пайплайн

## Critical rendering path
- HTML → CSSOM → render tree → layout → paint → composite.
- Reduce render-blocking resources (CSS, synchronous JS).

## CSS
- Inline critical CSS (above-the-fold), остальное через deferred loading.
- Avoid heavy selectors, large `box-shadow`, `filter` на множестве элементов.
- Порядок CSS: важно для CSSOM generation.

## JS
- Long tasks (блок >50 ms) → разбивайте (requestIdleCallback, Web Workers).
- Avoid synchronous XHR.
- Use `requestAnimationFrame` для визуальных обновлений.

## Layout Shift (CLS)
- Reserve space (`width`/`height` на изображения/видео).
- Avoid inserting content above existing content.
- Transitions вместо instant inline styles.

## Paint
- Reduce repaint/reflow: avoid frequent DOM updates, batch changes (documentFragment).
- CSS transforms (`transform`, `opacity`) — GPU-friendly.

## Web Workers
- Offload heavy computation to `Web Worker`/`Worklet` (AudioWorklet, PaintWorklet).
- Streams (ReadableStream) для progressive rendering.

## Modern APIs
- `requestIdleCallback` для low-priority work.
- `navigator.scheduling.isInputPending()` (Chrome) — scheduling tasks.
- `Experimental`: `scheduler.postTask`.

