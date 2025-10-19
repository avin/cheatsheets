package goconcurrency

import (
	"context"
	"fmt"
	"math/rand"
	"sync"
	"time"
)

// ---------------------------------------------------
// 📌 Генератор → фильтр → агрегатор
// ---------------------------------------------------
func pipelineSimple(ctx context.Context, count int) {
	generator := func(ctx context.Context) <-chan int {
		out := make(chan int)
		go func() {
			defer close(out)
			for i := 0; i < count; i++ {
				select {
				case out <- rand.Intn(100):
				case <-ctx.Done():
					return
				}
			}
		}()
		return out
	}

	filterEven := func(ctx context.Context, in <-chan int) <-chan int {
		out := make(chan int)
		go func() {
			defer close(out)
			for v := range in {
				if v%2 == 0 {
					select {
					case out <- v:
					case <-ctx.Done():
						return
					}
				}
			}
		}()
		return out
	}

	sumAll := func(ctx context.Context, in <-chan int) int {
		total := 0
		for {
			select {
			case v, ok := <-in:
				if !ok {
					return total
				}
				total += v
			case <-ctx.Done():
				return total
			}
		}
	}

	ctx, cancel := context.WithTimeout(ctx, 2*time.Second)
	defer cancel()

	total := sumAll(ctx, filterEven(ctx, generator(ctx)))
	fmt.Println("sum of even numbers:", total)
}

// ---------------------------------------------------
// 📌 Fan-out / Fan-in c балансировкой
// ---------------------------------------------------
func pipelineFanOutFanIn(ctx context.Context, jobs []int, workers int) <-chan int {
	input := make(chan int)
	go func() {
		defer close(input)
		for _, job := range jobs {
			select {
			case input <- job:
			case <-ctx.Done():
				return
			}
		}
	}()

	process := func(ctx context.Context, in <-chan int) <-chan int {
		out := make(chan int)
		go func() {
			defer close(out)
			for job := range in {
				// имитация сложной операции
				select {
				case <-time.After(100 * time.Millisecond):
				case <-ctx.Done():
					return
				}
				select {
				case out <- job * job:
				case <-ctx.Done():
					return
				}
			}
		}()
		return out
	}

	var outputs []<-chan int
	for i := 0; i < workers; i++ {
		outputs = append(outputs, process(ctx, input))
	}
	return mergeChannels(ctx, outputs...)
}

func mergeChannels(ctx context.Context, chans ...<-chan int) <-chan int {
	out := make(chan int)
	var wg sync.WaitGroup
	wg.Add(len(chans))

	for _, ch := range chans {
		ch := ch
		go func() {
			defer wg.Done()
			for val := range ch {
				select {
				case out <- val:
				case <-ctx.Done():
					return
				}
			}
		}()
	}

	go func() {
		wg.Wait()
		close(out)
	}()
	return out
}

// ---------------------------------------------------
// 📌 Backpressure: ограниченный буфер и ack
// ---------------------------------------------------
func pipelineWithBackpressure(ctx context.Context, jobs <-chan int, buffer int) <-chan int {
	out := make(chan int, buffer)
	go func() {
		defer close(out)
		for job := range jobs {
			select {
			case out <- job:
			case <-ctx.Done():
				return
			}
		}
	}()
	return out
}

// ---------------------------------------------------
// 📌 Heartbeat: мониторинг пайплайна
// ---------------------------------------------------
func pipelineHeartbeat(ctx context.Context, interval time.Duration) (<-chan time.Time, context.CancelFunc) {
	ctx, cancel := context.WithCancel(ctx)
	ch := make(chan time.Time)

	go func() {
		defer close(ch)
		ticker := time.NewTicker(interval)
		defer ticker.Stop()

		for {
			select {
			case t := <-ticker.C:
				select {
				case ch <- t:
				case <-ctx.Done():
					return
				}
			case <-ctx.Done():
				return
			}
		}
	}()

	return ch, cancel
}
