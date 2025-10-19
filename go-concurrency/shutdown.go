package goconcurrency

import (
	"context"
	"fmt"
	"os"
	"os/signal"
	"sync"
	"syscall"
	"time"
)

// ---------------------------------------------------
// 📌 Graceful shutdown по сигналу
// ---------------------------------------------------
func gracefulServerShutdown(ctx context.Context, stop func(context.Context) error) error {
	ctx, cancel := context.WithCancel(ctx)
	defer cancel()

	signals := make(chan os.Signal, 1)
	signal.Notify(signals, syscall.SIGINT, syscall.SIGTERM)
	defer signal.Stop(signals)

	select {
	case sig := <-signals:
		fmt.Println("received signal", sig)
	case <-ctx.Done():
		fmt.Println("context cancelled")
	}

	shutdownCtx, cancelShutdown := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancelShutdown()
	return stop(shutdownCtx)
}

// ---------------------------------------------------
// 📌 Worker pool с остановкой и drain
// ---------------------------------------------------
type workerPool struct {
	tasks    chan func()
	wg       sync.WaitGroup
	stopOnce sync.Once
	stopped  chan struct{}
}

func newWorkerPool(workers int) *workerPool {
	pool := &workerPool{
		tasks:   make(chan func()),
		stopped: make(chan struct{}),
	}
	pool.wg.Add(workers)
	for i := 0; i < workers; i++ {
		go func(id int) {
			defer pool.wg.Done()
			for task := range pool.tasks {
				task()
			}
			fmt.Println("worker stopped", id)
		}(i)
	}
	return pool
}

func (p *workerPool) Submit(task func()) {
	select {
	case <-p.stopped:
		return
	default:
	}
	p.tasks <- task
}

func (p *workerPool) Shutdown() {
	p.stopOnce.Do(func() {
		close(p.tasks)
		p.wg.Wait()
		close(p.stopped)
	})
}

// ---------------------------------------------------
// 📌 Перезапуск задач при сбоях
// ---------------------------------------------------
func restartUntil(ctx context.Context, maxRetries int, fn func(context.Context) error) error {
	for attempt := 1; attempt <= maxRetries; attempt++ {
		runCtx, cancel := context.WithCancel(ctx)
		err := fn(runCtx)
		cancel()
		if err == nil {
			return nil
		}
		fmt.Println("attempt", attempt, "failed:", err)
		select {
		case <-time.After(time.Second * time.Duration(attempt)):
		case <-ctx.Done():
			return ctx.Err()
		}
	}
	return fmt.Errorf("exceeded retries (%d)", maxRetries)
}

// ---------------------------------------------------
// 📌 Guard против паники в горутине
// ---------------------------------------------------
func guardedGo(wg *sync.WaitGroup, fn func() error, onPanic func(any), onError func(error)) {
	wg.Add(1)
	go func() {
		defer wg.Done()
		defer func() {
			if r := recover(); r != nil {
				onPanic(r)
			}
		}()
		if err := fn(); err != nil && onError != nil {
			onError(err)
		}
	}()
}
