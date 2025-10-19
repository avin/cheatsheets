package goconcurrency

import (
	"context"
	"errors"
	"fmt"
	"net/http"
	"sync"
	"time"

	"golang.org/x/sync/errgroup"
	"golang.org/x/sync/semaphore"
)

// ---------------------------------------------------
// 📌 Параллельные HTTP-запросы с errgroup
// ---------------------------------------------------
func fetchEndpoints(ctx context.Context, urls []string) error {
	ctx, cancel := context.WithTimeout(ctx, 2*time.Second)
	defer cancel()

	client := &http.Client{Timeout: 1500 * time.Millisecond}
	g, ctx := errgroup.WithContext(ctx)

	results := make([]string, len(urls))
	for i, url := range urls {
		i, url := i, url
		g.Go(func() error {
			req, err := http.NewRequestWithContext(ctx, http.MethodGet, url, nil)
			if err != nil {
				return err
			}
			resp, err := client.Do(req)
			if err != nil {
				return err
			}
			defer resp.Body.Close()
			if resp.StatusCode >= http.StatusBadRequest {
				return fmt.Errorf("unexpected status %d from %s", resp.StatusCode, url)
			}
			results[i] = resp.Status
			return nil
		})
	}

	if err := g.Wait(); err != nil {
		return err
	}
	fmt.Println("responses:", results)
	return nil
}

// ---------------------------------------------------
// 📌 Пайплайн: producer -> transformers -> consumer
// ---------------------------------------------------
func errgroupPipeline(ctx context.Context, jobs []int) error {
	ctx, cancel := context.WithTimeout(ctx, 3*time.Second)
	defer cancel()

	g, ctx := errgroup.WithContext(ctx)
	produced := make(chan int)
	transformed := make(chan int)

	g.Go(func() error {
		defer close(produced)
		for _, job := range jobs {
			select {
			case produced <- job:
			case <-ctx.Done():
				return ctx.Err()
			}
		}
		return nil
	})

	var transformWG sync.WaitGroup
	for i := 0; i < 3; i++ {
		transformWG.Add(1)
		g.Go(func() error {
			defer transformWG.Done()
			for job := range produced {
				select {
				case transformed <- job * job:
				case <-ctx.Done():
					return ctx.Err()
				}
			}
			return nil
		})
	}

	g.Go(func() error {
		transformWG.Wait()
		close(transformed)
		return nil
	})

	g.Go(func() error {
		for result := range transformed {
			fmt.Println("result", result)
		}
		return nil
	})

	return g.Wait()
}

// ---------------------------------------------------
// 📌 Ограниченный параллелизм + отмена
// ---------------------------------------------------
func errgroupWithSemaphore(ctx context.Context, jobs []int, limit int64) error {
	g, ctx := errgroup.WithContext(ctx)
	var sem = semaphore.NewWeighted(limit)

	for _, job := range jobs {
		job := job
		if err := sem.Acquire(ctx, 1); err != nil {
			return err
		}
		g.Go(func() error {
			defer sem.Release(1)
			// имитация работы
			select {
			case <-time.After(200 * time.Millisecond):
				fmt.Println("processed", job)
				return nil
			case <-ctx.Done():
				return ctx.Err()
			}
		})
	}

	if err := g.Wait(); err != nil {
		return err
	}
	return nil
}

// ---------------------------------------------------
// 📌 Rollback через errgroup
// ---------------------------------------------------
func errgroupWithRollback(ctx context.Context, steps []func(context.Context) error, rollback func(error)) error {
	g, ctx := errgroup.WithContext(ctx)
	for _, step := range steps {
		step := step
		g.Go(func() error {
			if err := step(ctx); err != nil {
				return err
			}
			return nil
		})
	}

	if err := g.Wait(); err != nil {
		rollback(err)
		return err
	}
	return nil
}

// ---------------------------------------------------
// 📌 Пример rollback-функции
// ---------------------------------------------------
func sampleRollback(err error) {
	if errors.Is(err, context.DeadlineExceeded) {
		fmt.Println("rollback: deadline exceeded")
		return
	}
	fmt.Println("rollback triggered:", err)
}
