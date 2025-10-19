# Go Concurrency Patterns

Расширенная коллекция приёмов конкурентного программирования на Go: от организации пайплайнов до graceful shutdown и профилирования.

### 🧵 [Горутины и синхронизация](./sync_primitives.go)

* 📌  **`sync.WaitGroup`, `sync.Mutex`, `sync.RWMutex`, `sync.Cond`**
* 📌  **`sync.Map`, атомики и broadcast-сигналы**
* 📌  **Ограничение параллелизма через семафоры**

***

### 🧑‍🤝‍🧑 [Группы задач и отмена](./errgroup_patterns.go)

* 📌  **`errgroup.Group` в составе пайплайна**
* 📌  **Параллельные запросы с таймаутом**
* 📌  **Обработка ошибок и rollback**

***

### 🔀 [Пайплайны и fan-in/fan-out](./pipelines.go)

* 📌  **Ленивые генераторы, фильтры, агрегаторы**
* 📌  **Fan-out/Fan-in, балансировка нагрузки**
* 📌  **Backpressure и управление буферами**

***

### 🚦 [Ограничение скорости и ретраи](./rate_limiting.go)

* 📌  **Token bucket (`time.Ticker`, `rate.Limiter`)**
* 📌  **Паузы с джиттером, экспоненциальный backoff**
* 📌  **`singleflight.Group` для дедупликации**

***

### 📴 [Graceful shutdown и перезапуск](./shutdown.go)

* 📌  **Завершение сервисов по сигналу, `context`**
* 📌  **Организация worker pool с остановкой**
* 📌  **Перезапуск задач и guard-паттерны**

***

### ✅ [Тестирование и отладка](./testing_debug.go)

* 📌  **`httptest` и race detector**
* 📌  **Оптимизация через `pprof` и `trace`**
* 📌  **Fakes/mocks для конкурентного кода**
