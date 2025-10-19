// ---------------------------------------------------
// 📌 Promises - создание
// ---------------------------------------------------
const promise = new Promise((resolve, reject) => {
  // Асинхронная операция
  setTimeout(() => {
    const success = true;
    if (success) {
      resolve('Результат');
    } else {
      reject(new Error('Ошибка'));
    }
  }, 1000);
});


// ---------------------------------------------------
// 📌 Promises - использование (then/catch/finally)
// ---------------------------------------------------
promise
  .then(result => {
    console.log('Успех:', result);
    return result + ' обработан';
  })
  .then(processed => {
    console.log(processed);
  })
  .catch(error => {
    console.error('Ошибка:', error);
  })
  .finally(() => {
    console.log('Завершено');
  });


// ---------------------------------------------------
// 📌 Async/Await - базовый синтаксис
// ---------------------------------------------------
async function fetchData() {
  try {
    const response = await fetch('https://api.example.com/data');
    const data = await response.json();
    return data;
  } catch (error) {
    console.error('Ошибка:', error);
    throw error;
  }
}


// ---------------------------------------------------
// 📌 Promise.all - параллельное выполнение
// ---------------------------------------------------
async function fetchMultiple() {
  const [users, posts, comments] = await Promise.all([
    fetch('/api/users').then(r => r.json()),
    fetch('/api/posts').then(r => r.json()),
    fetch('/api/comments').then(r => r.json())
  ]);

  return {users, posts, comments};
}


// ---------------------------------------------------
// 📌 Promise.race - первый завершившийся
// ---------------------------------------------------
async function fetchWithTimeout(url, timeout = 5000) {
  return Promise.race([
    fetch(url),
    new Promise((_, reject) =>
      setTimeout(() => reject(new Error('Timeout')), timeout)
    )
  ]);
}


// ---------------------------------------------------
// 📌 Promise.allSettled - все завершения (даже ошибки)
// ---------------------------------------------------
async function fetchAllSettled() {
  const results = await Promise.allSettled([
    fetch('/api/users'),
    fetch('/api/posts'),
    fetch('/api/invalid-endpoint')
  ]);

  results.forEach((result, index) => {
    if (result.status === 'fulfilled') {
      console.log(`Запрос ${index}: успех`, result.value);
    } else {
      console.log(`Запрос ${index}: ошибка`, result.reason);
    }
  });
}


// ---------------------------------------------------
// 📌 Promise.any - первый успешный
// ---------------------------------------------------
async function fetchFromMirrors() {
  try {
    const response = await Promise.any([
      fetch('https://mirror1.example.com/data'),
      fetch('https://mirror2.example.com/data'),
      fetch('https://mirror3.example.com/data')
    ]);
    return response.json();
  } catch (error) {
    // Все зеркала недоступны
    console.error('Все источники недоступны');
  }
}


// ---------------------------------------------------
// 📌 Последовательное выполнение async функций
// ---------------------------------------------------
async function processSequentially(items) {
  const results = [];

  for (const item of items) {
    const result = await processItem(item);
    results.push(result);
  }

  return results;
}


// ---------------------------------------------------
// 📌 Параллельное выполнение с ограничением
// ---------------------------------------------------
async function processWithLimit(items, limit = 3) {
  const results = [];
  const executing = [];

  for (const item of items) {
    const promise = processItem(item).then(result => {
      executing.splice(executing.indexOf(promise), 1);
      return result;
    });

    results.push(promise);
    executing.push(promise);

    if (executing.length >= limit) {
      await Promise.race(executing);
    }
  }

  return Promise.all(results);
}


// ---------------------------------------------------
// 📌 Retry механизм
// ---------------------------------------------------
async function retry(fn, maxAttempts = 3, delay = 1000) {
  for (let attempt = 1; attempt <= maxAttempts; attempt++) {
    try {
      return await fn();
    } catch (error) {
      if (attempt === maxAttempts) {
        throw error;
      }
      console.log(`Попытка ${attempt} неудачна, повтор через ${delay}ms`);
      await new Promise(resolve => setTimeout(resolve, delay));
    }
  }
}

// Использование
// await retry(() => fetch('/api/data'), 3, 2000);


// ---------------------------------------------------
// 📌 Debounce для async функций
// ---------------------------------------------------
function debounceAsync(fn, delay) {
  let timeout;
  return (...args) => {
    return new Promise((resolve) => {
      clearTimeout(timeout);
      timeout = setTimeout(async () => {
        const result = await fn(...args);
        resolve(result);
      }, delay);
    });
  };
}


// ---------------------------------------------------
// 📌 Throttle для async функций
// ---------------------------------------------------
function throttleAsync(fn, delay) {
  let lastCall = 0;
  return async (...args) => {
    const now = Date.now();
    if (now - lastCall >= delay) {
      lastCall = now;
      return await fn(...args);
    }
  };
}


// ---------------------------------------------------
// 📌 Timeout для Promise
// ---------------------------------------------------
function withTimeout(promise, ms) {
  return Promise.race([
    promise,
    new Promise((_, reject) =>
      setTimeout(() => reject(new Error(`Timeout after ${ms}ms`)), ms)
    )
  ]);
}


// ---------------------------------------------------
// 📌 Очередь асинхронных задач
// ---------------------------------------------------
class AsyncQueue {
  constructor() {
    this.queue = [];
    this.processing = false;
  }

  async add(task) {
    return new Promise((resolve, reject) => {
      this.queue.push({task, resolve, reject});
      if (!this.processing) {
        this.process();
      }
    });
  }

  async process() {
    if (this.queue.length === 0) {
      this.processing = false;
      return;
    }

    this.processing = true;
    const {task, resolve, reject} = this.queue.shift();

    try {
      const result = await task();
      resolve(result);
    } catch (error) {
      reject(error);
    }

    await this.process();
  }
}


// ---------------------------------------------------
// 📌 Абортирование fetch запросов
// ---------------------------------------------------
async function fetchWithAbort() {
  const controller = new AbortController();
  const signal = controller.signal;

  // Автоматическая отмена через 5 секунд
  setTimeout(() => controller.abort(), 5000);

  try {
    const response = await fetch('/api/data', {signal});
    return await response.json();
  } catch (error) {
    if (error.name === 'AbortError') {
      console.log('Запрос отменён');
    } else {
      throw error;
    }
  }
}


// ---------------------------------------------------
// 📌 Кэширование async функций
// ---------------------------------------------------
function memoizeAsync(fn) {
  const cache = new Map();

  return async (...args) => {
    const key = JSON.stringify(args);

    if (cache.has(key)) {
      return cache.get(key);
    }

    const result = await fn(...args);
    cache.set(key, result);
    return result;
  };
}


// ---------------------------------------------------
// 📌 Batch обработка (группировка запросов)
// ---------------------------------------------------
class BatchProcessor {
  constructor(batchFn, delay = 10) {
    this.batchFn = batchFn;
    this.delay = delay;
    this.queue = [];
    this.timeout = null;
  }

  async add(item) {
    return new Promise((resolve, reject) => {
      this.queue.push({item, resolve, reject});

      if (!this.timeout) {
        this.timeout = setTimeout(() => this.flush(), this.delay);
      }
    });
  }

  async flush() {
    if (this.queue.length === 0) return;

    const batch = this.queue.splice(0);
    this.timeout = null;

    try {
      const items = batch.map(b => b.item);
      const results = await this.batchFn(items);

      batch.forEach((b, i) => b.resolve(results[i]));
    } catch (error) {
      batch.forEach(b => b.reject(error));
    }
  }
}


// ---------------------------------------------------
// 📌 Polling (периодический опрос)
// ---------------------------------------------------
async function poll(fn, condition, interval = 1000, maxAttempts = 30) {
  for (let i = 0; i < maxAttempts; i++) {
    const result = await fn();

    if (condition(result)) {
      return result;
    }

    await new Promise(resolve => setTimeout(resolve, interval));
  }

  throw new Error('Polling timeout');
}

// Пример: await poll(() => checkStatus(), status => status === 'ready');


// ---------------------------------------------------
// 📌 Long polling
// ---------------------------------------------------
async function longPoll(url, onData, onError) {
  while (true) {
    try {
      const response = await fetch(url);
      const data = await response.json();

      if (data) {
        onData(data);
      }
    } catch (error) {
      if (onError) {
        onError(error);
      }
      // Задержка перед следующей попыткой
      await new Promise(resolve => setTimeout(resolve, 5000));
    }
  }
}


// ---------------------------------------------------
// 📌 Параллельный map
// ---------------------------------------------------
async function asyncMap(array, asyncFn) {
  return Promise.all(array.map(asyncFn));
}

// Пример: await asyncMap([1,2,3], async x => fetch(`/api/item/${x}`))


// ---------------------------------------------------
// 📌 Последовательный reduce
// ---------------------------------------------------
async function asyncReduce(array, asyncFn, initialValue) {
  let accumulator = initialValue;

  for (const item of array) {
    accumulator = await asyncFn(accumulator, item);
  }

  return accumulator;
}


// ---------------------------------------------------
// 📌 Async filter
// ---------------------------------------------------
async function asyncFilter(array, asyncPredicate) {
  const results = await Promise.all(array.map(asyncPredicate));
  return array.filter((_, index) => results[index]);
}


// ---------------------------------------------------
// 📌 Async forEach
// ---------------------------------------------------
async function asyncForEach(array, asyncFn) {
  for (const item of array) {
    await asyncFn(item);
  }
}


// ---------------------------------------------------
// 📌 Обработка ошибок в Promise chain
// ---------------------------------------------------
promise
  .then(result => processResult(result))
  .catch(error => {
    // Обработка ошибки
    console.error(error);
    return defaultValue; // Возврат fallback значения
  })
  .then(result => {
    // Продолжение с result или defaultValue
  });


// ---------------------------------------------------
// 📌 Async Generator
// ---------------------------------------------------
async function* asyncGenerator() {
  yield await Promise.resolve(1);
  yield await Promise.resolve(2);
  yield await Promise.resolve(3);
}

// Использование
// for await (const value of asyncGenerator()) {
//   console.log(value);
// }


// ---------------------------------------------------
// 📌 Создание отложенного Promise
// ---------------------------------------------------
function createDeferred() {
  let resolve, reject;
  const promise = new Promise((res, rej) => {
    resolve = res;
    reject = rej;
  });
  return {promise, resolve, reject};
}


// ---------------------------------------------------
// 📌 Sleep функция
// ---------------------------------------------------
const sleep = (ms) => new Promise(resolve => setTimeout(resolve, ms));

// Использование: await sleep(1000);


// ---------------------------------------------------
// 📌 Таймаут с очисткой
// ---------------------------------------------------
function delay(ms) {
  let timeoutId;
  const promise = new Promise(resolve => {
    timeoutId = setTimeout(resolve, ms);
  });
  promise.cancel = () => clearTimeout(timeoutId);
  return promise;
}
