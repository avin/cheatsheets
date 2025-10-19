package cheatsheets

import (
	"bufio"
	"fmt"
	"log"
	"log/slog"
	"math/rand"
	"os"
	"runtime/pprof"
	"strings"
	"time"
)

// ---------------------------------------------------
// 📌 fmt для быстрого вывода
// ---------------------------------------------------
func logFmtDemo(v any, count int, cost float64, msg string) {
	fmt.Println("value", v)
	fmt.Printf("count=%d cost=%.2f\n", count, cost)
	fmt.Fprintf(os.Stderr, "warn: %s\n", msg)
}

// ---------------------------------------------------
// 📌 Пакет log
// ---------------------------------------------------
func logPackageDemo() {
	logger := log.New(os.Stdout, "[app] ", log.LstdFlags|log.Lshortfile)
	logger.Println("started")
	logger.Printf("processed=%d", 42)
}

// ---------------------------------------------------
// 📌 Структурный slog
// ---------------------------------------------------
func logSlogDemo(err error) {
	handler := slog.NewJSONHandler(os.Stdout, &slog.HandlerOptions{Level: slog.LevelInfo})
	logger := slog.New(handler)
	logger.Info("user login", slog.String("id", "u-1"), slog.Int("retries", 0))
	logger.Error("failed", slog.Any("err", err))
}

// ---------------------------------------------------
// 📌 Замер времени
// ---------------------------------------------------
func logTimingDemo() {
	compute := func() int {
		time.Sleep(10 * time.Millisecond)
		return 42
	}
	start := time.Now()
	result := compute()
	elapsed := time.Since(start)
	log.Printf("compute finished in %s (result=%d)", elapsed, result)
}

// ---------------------------------------------------
// 📌 Профилирование CPU
// ---------------------------------------------------
func logProfileDemo() {
	f, err := os.Create("cpu.prof")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	if err := pprof.StartCPUProfile(f); err != nil {
		log.Fatal(err)
	}
	defer pprof.StopCPUProfile()

	heavyWork := func() {
		total := 0
		for i := 0; i < 1_000_000; i++ {
			total += i
		}
		_ = total
	}
	heavyWork()
}

// ---------------------------------------------------
// 📌 Mock-входные данные
// ---------------------------------------------------
func logMockInput() {
	builder := strings.Builder{}
	for i := 0; i < 10; i++ {
		fmt.Fprintf(&builder, "%d %d\n", i, rand.Intn(1000))
	}
	input := builder.String()
	reader := strings.NewReader(input)
	scanner := bufio.NewScanner(reader)
	for scanner.Scan() {
		fmt.Println(scanner.Text())
	}
}
