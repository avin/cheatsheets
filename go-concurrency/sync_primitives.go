package goconcurrency

import (
	"context"
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

// ---------------------------------------------------
// 📌 WaitGroup + Mutex для критической секции
// ---------------------------------------------------
func syncMutexExample(tasks []int) {
	var (
		wg    sync.WaitGroup
		mu    sync.Mutex
		total int
	)

	for _, n := range tasks {
		wg.Add(1)
		go func(v int) {
			defer wg.Done()
			result := heavyComputation(v)

			mu.Lock()
			total += result
			mu.Unlock()
		}(n)
	}

	wg.Wait()
	fmt.Println("total", total)
}

func heavyComputation(v int) int {
	time.Sleep(10 * time.Millisecond)
	return v * v
}

// ---------------------------------------------------
// 📌 RWMutex для кэша только-чтение + обновления
// ---------------------------------------------------
type cache struct {
	mu    sync.RWMutex
	store map[string]string
}

func (c *cache) Get(key string) (string, bool) {
	c.mu.RLock()
	defer c.mu.RUnlock()
	val, ok := c.store[key]
	return val, ok
}

func (c *cache) Set(key, value string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	if c.store == nil {
		c.store = make(map[string]string)
	}
	c.store[key] = value
}

// ---------------------------------------------------
// 📌 sync.Cond для широковещательных сигналов
// ---------------------------------------------------
func syncCondQueue(capacity int) {
	queue := make([]int, 0, capacity)
	mu := sync.Mutex{}
	notEmpty := sync.NewCond(&mu)
	notFull := sync.NewCond(&mu)

	enqueue := func(val int) {
		mu.Lock()
		for len(queue) == capacity {
			notFull.Wait()
		}
		queue = append(queue, val)
		notEmpty.Signal()
		mu.Unlock()
	}

	dequeue := func() int {
		mu.Lock()
		for len(queue) == 0 {
			notEmpty.Wait()
		}
		val := queue[0]
		queue = queue[1:]
		notFull.Signal()
		mu.Unlock()
		return val
	}

	go func() {
		for i := 0; i < 5; i++ {
			enqueue(i)
		}
	}()

	for i := 0; i < 5; i++ {
		fmt.Println("dequeued", dequeue())
	}
}

// ---------------------------------------------------
// 📌 sync.Map для lock-free чтения и редких записей
// ---------------------------------------------------
func syncMapExample(keys []string) {
	var m sync.Map
	for _, key := range keys {
		m.Store(key, time.Now())
	}

	m.Range(func(key, value any) bool {
		fmt.Printf("%s updated at %v\n", key, value)
		return true
	})

	if val, ok := m.Load("user:42"); ok {
		fmt.Println("found", val)
	}
}

// ---------------------------------------------------
// 📌 Атомики для счётчиков и флагов
// ---------------------------------------------------
type atomicCounter struct {
	value atomic.Int64
}

func (c *atomicCounter) Inc() {
	c.value.Add(1)
}

func (c *atomicCounter) Value() int64 {
	return c.value.Load()
}

// ---------------------------------------------------
// 📌 Ограничение параллелизма через семафор
// ---------------------------------------------------
func limitedParallel(ctx context.Context, tasks []func(context.Context) error, limit int) error {
	if limit <= 0 {
		limit = 1
	}
	sem := make(chan struct{}, limit)
	var (
		wg    sync.WaitGroup
		once  sync.Once
		first error
	)

	for _, task := range tasks {
		if err := ctx.Err(); err != nil {
			return err
		}
		wg.Add(1)
		go func(fn func(context.Context) error) {
			defer wg.Done()

			select {
			case sem <- struct{}{}:
				defer func() { <-sem }()
			case <-ctx.Done():
				return
			}

			if err := fn(ctx); err != nil {
				once.Do(func() { first = err })
			}
		}(task)
	}

	wg.Wait()
	return first
}
