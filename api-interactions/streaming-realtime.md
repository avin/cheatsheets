# Streaming, SSE и WebSockets

## Streaming responses
- Fetch streaming: `const reader = response.body.getReader();` (ReadableStream).
- JSON streaming: NDJSON (newline-delimited), parse chunk by chunk.
- SSR frameworks (Next.js/React 18): `stream` responses to client.

## Server-Sent Events (SSE)
```js
const source = new EventSource('/events');
source.onmessage = event => {
  const data = JSON.parse(event.data);
  console.log('event', data);
};
source.onerror = () => {
  source.close();
};
```
- Односторонний канал (сервер → клиент).
- Автоматическая reconnection, no custom headers.
- Headers: `Content-Type: text/event-stream`, `Cache-Control: no-cache`, `Connection: keep-alive`.

## WebSockets
```js
const socket = new WebSocket('wss://example.com/ws');
socket.addEventListener('open', () => socket.send(JSON.stringify({ type: 'hello' })));
socket.addEventListener('message', event => {
  const data = JSON.parse(event.data);
  console.log(data);
});
```
- Двусторонняя связь, полезна для real-time обновлений.
- Позаботьтесь о reconnect/backoff.
- Протоколы: WS, WSS (over TLS). В SPA используйте менеджеры (Socket.IO, ws, GraphQL subscriptions).

## WebTransport
- Экспериментальный API (HTTP/3). Reliability/Unreliable datagrams.
- Not widely supported yet.

## Fetch keepalive
- `fetch(url, { keepalive: true, method: 'POST', body })` для beacon/analytics.

## Background sync/notifications
- `background sync` (Chrome), `Push API` для offline updates.

## Real-time frameworks
- Socket.IO, Pusher, Ably, Firebase Realtime Database.
- GraphQL Subscriptions (see GraphQL section).
- LiveQuery (Parse, Hasura), Supabase Realtime.

