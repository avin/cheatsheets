# Фоновые задачи и интеграции

Обработка фоновых задач, очередей и внешних сервисов.

## Очереди задач

### BullMQ (Redis)

```ts
import { Queue, Worker } from 'bullmq';

const emailQueue = new Queue('email', {
  connection: { host: 'localhost', port: 6379 },
});

await emailQueue.add('send-welcome', { userId });

const worker = new Worker(
  'email',
  async (job) => {
    await emailService.sendWelcome(job.data.userId);
  },
  { connection: { host: 'localhost', port: 6379 } },
);
```

- Настраивайте retries, backoff, concurrency (`{ attempts: 5, backoff: { type: 'exponential', delay: 1000 } }`).
- Следите за завершёнными/ошибочными задачами, очищайте очереди.

### RabbitMQ / AMQP

```ts
import amqp from 'amqplib';

const conn = await amqp.connect(process.env.AMQP_URL);
const channel = await conn.createChannel();
const queue = 'notifications';

await channel.assertQueue(queue, { durable: true });
channel.sendToQueue(queue, Buffer.from(JSON.stringify(payload)), { persistent: true });

channel.consume(queue, async (msg) => {
  if (!msg) return;
  await notificationService.process(JSON.parse(msg.content.toString()));
  channel.ack(msg);
});
```

- Используйте prefetch (`channel.prefetch(10)`) для ограничения параллелизма.
- Перекидывайте ошибки в DLX (dead-letter exchange).

## Рассылка email/SMS/Push

- Сервисы: Sendgrid, Postmark, AWS SES, Twilio, Firebase FCM.
- Делайте обёртку (`emailService.sendTemplate(user, template, data)`).
- Логируйте сообщения, храните аудит в БД.

## Интеграция webhooks

- Проверяйте подписи (Stripe `constructEvent`, GitHub `X-Hub-Signature-256`).
- Отвечайте 2xx быстро, основную работу отправляйте в очередь.
- Храните события и статус обработки (duplicate detection).

```ts
router.post('/webhooks/stripe', express.raw({ type: 'application/json' }), async (req, res) => {
  const signature = req.headers['stripe-signature'];
  try {
    const event = stripe.webhooks.constructEvent(req.body, signature, process.env.STRIPE_SECRET);
    await stripeEventService.handle(event);
    res.status(200).json({ received: true });
  } catch (err) {
    res.status(400).json({ message: 'invalid signature' });
  }
});
```

## Взаимодействие с внешними API

- Используйте `axios.create`/`got.extend` со стандартными заголовками, таймаутами, retry.
- Применяйте circuit breaker (`opossum`, `cockatiel`) при нестабильных сервисах.
- Кешируйте ответы (Redis, InMemory) с TTL.

## Фоновая обработка и Express

- Разделяйте HTTP-приложение и worker-приложение.
- Храните общие сервисы в отдельном пакете (`services/`, `lib/`).
- Добавляйте health-check endpoint (`/healthz`), показывающий состояние очередей.
