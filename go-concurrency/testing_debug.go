package goconcurrency

import (
	"context"
	"net/http"
	"net/http/httptest"
	"os"
	"runtime"
	"runtime/pprof"
	"runtime/trace"
	"sync"
	"testing"
	"time"
)

// ---------------------------------------------------
// 📌 Пример юнит-теста конкурентной функции
// ---------------------------------------------------
func TestLimitedParallel(t *testing.T) {
	t.Parallel()
	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()

	tasks := []func(context.Context) error{
		func(ctx context.Context) error { <-time.After(50 * time.Millisecond); return nil },
		func(ctx context.Context) error { <-time.After(60 * time.Millisecond); return nil },
		func(ctx context.Context) error { <-time.After(70 * time.Millisecond); return nil },
	}

	if err := limitedParallel(ctx, tasks, 2); err != nil {
		t.Fatal(err)
	}
}

// ---------------------------------------------------
// 📌 Race detector в CI
// ---------------------------------------------------
// go test -race ./...

// ---------------------------------------------------
// 📌 Профилирование pprof
// ---------------------------------------------------
func benchmarkHandler(b *testing.B) {
	handler := func(w http.ResponseWriter, r *http.Request) {
		time.Sleep(5 * time.Millisecond)
	}

	req := httptest.NewRequest(http.MethodGet, "/", nil)
	rec := httptest.NewRecorder()

	for i := 0; i < b.N; i++ {
		handler(rec, req)
	}
}

func exampleCPUProfile() {
	f, _ := os.Create("cpu.prof")
	defer f.Close()
	pprof.StartCPUProfile(f)
	defer pprof.StopCPUProfile()

	for i := 0; i < 1e6; i++ {
		_ = time.Now()
	}
}

// ---------------------------------------------------
// 📌 Trace для анализа конкуренции
// ---------------------------------------------------
func exampleTrace() {
	f, _ := os.Create("trace.out")
	defer f.Close()
	trace.Start(f)
	defer trace.Stop()

	var wg sync.WaitGroup
	for i := 0; i < runtime.NumCPU(); i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			time.Sleep(20 * time.Millisecond)
		}()
	}
	wg.Wait()
}
