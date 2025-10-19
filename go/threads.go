package cheatsheets

import (
	"context"
	"errors"
	"fmt"
	"log"
	"sync"
	"time"
)

// ---------------------------------------------------
// 📌 Запуск горутины и ожидание
// ---------------------------------------------------
func threadGoroutine() {
	work := func() { fmt.Println("done") }
	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		work()
	}()
	wg.Wait()
}

// ---------------------------------------------------
// 📌 Небуферизированный канал
// ---------------------------------------------------
func threadUnbuffered() {
	ch := make(chan int)
	go func() {
		for i := 0; i < 3; i++ {
			ch <- i
		}
		close(ch)
	}()
	for v := range ch {
		fmt.Println("got", v)
	}
}

// ---------------------------------------------------
// 📌 Буферизированный канал
// ---------------------------------------------------
func threadBuffered() {
	ch := make(chan string, 2)
	ch <- "one"
	ch <- "two"
	go func() {
		ch <- "three"
	}()
	fmt.Println(<-ch)
	fmt.Println(<-ch)
	fmt.Println(<-ch)
}

// ---------------------------------------------------
// 📌 select и таймаут
// ---------------------------------------------------
func threadSelect(payload string) {
	ch := make(chan string, 1)
	ch <- "ready"
	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()

	select {
	case msg := <-ch:
		fmt.Println("message", msg)
	case ch <- payload:
		fmt.Println("sent")
	case <-time.After(500 * time.Millisecond):
		fmt.Println("timeout")
	case <-ctx.Done():
		fmt.Println("cancelled")
	}
}

// ---------------------------------------------------
// 📌 Worker pool
// ---------------------------------------------------
type Task struct{ ID int }

func threadWorkerPool() {
	jobs := make(chan int)
	results := make(chan int)
	process := func(job int) int { return job * job }

	var wg sync.WaitGroup
	for w := 0; w < 4; w++ {
		wg.Add(1)
		go func(id int) {
			defer wg.Done()
			for job := range jobs {
				fmt.Println("worker", id, "job", job)
				results <- process(job)
			}
		}(w)
	}

	go func() {
		for i := 0; i < 10; i++ {
			jobs <- i
		}
		close(jobs)
	}()

	go func() {
		wg.Wait()
		close(results)
	}()

	for result := range results {
		fmt.Println("result", result)
	}
}

// ---------------------------------------------------
// 📌 Семафор на канале
// ---------------------------------------------------
func threadSemaphore(tasks []Task) {
	run := func(t Task) { fmt.Println("task", t.ID) }
	sem := make(chan struct{}, 2)
	var wg sync.WaitGroup

	for _, task := range tasks {
		sem <- struct{}{}
		wg.Add(1)
		go func(t Task) {
			defer wg.Done()
			defer func() { <-sem }()
			run(t)
		}(task)
	}

	wg.Wait()
}

// ---------------------------------------------------
// 📌 Контекст и отмена
// ---------------------------------------------------
func threadContext() {
	longRunning := func(ctx context.Context) error {
		<-ctx.Done()
		return ctx.Err()
	}

	ctx, cancel := context.WithCancel(context.Background())
	go func() {
		if err := longRunning(ctx); err != nil && !errors.Is(err, context.Canceled) {
			log.Println("error", err)
		}
	}()
	cancel()
}
