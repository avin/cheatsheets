package cheatsheets

import (
	"context"
	"errors"
	"fmt"
	"io/fs"
	"log"
	"net/http"
	"os"
	"sync"
	"time"
)

// ---------------------------------------------------
// 📌 Работа со временем
// ---------------------------------------------------
func utilTime() {
	now := time.Now()
	future := now.Add(2 * time.Hour)
	layout := time.RFC3339
	formatted := now.Format(layout)
	parsed, err := time.Parse(layout, "2024-05-01T10:00:00Z")
	fmt.Println(now, future, formatted, parsed, err)
}

// ---------------------------------------------------
// 📌 Таймеры и тикеры
// ---------------------------------------------------
func utilTimers(ctx context.Context) {
	timer := time.NewTimer(5 * time.Second)
	select {
	case <-timer.C:
		fmt.Println("timeout")
	case <-ctx.Done():
		timer.Stop()
		fmt.Println("cancel")
	}

	ticker := time.NewTicker(time.Second)
	defer ticker.Stop()
	for i := 0; i < 3; i++ {
		tick := <-ticker.C
		fmt.Println("tick", tick)
	}
}

// ---------------------------------------------------
// 📌 Контекст и отмена
// ---------------------------------------------------
func utilHTTP(ctx context.Context, url string) error {
	req, err := http.NewRequestWithContext(ctx, http.MethodGet, url, nil)
	if err != nil {
		return err
	}
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()
	fmt.Println("status", resp.StatusCode)
	return nil
}

// ---------------------------------------------------
// 📌 Обработка ошибок
// ---------------------------------------------------
func utilErrors(run func() error) error {
	if err := run(); err != nil {
		var pathErr *fs.PathError
		if errors.As(err, &pathErr) {
			return fmt.Errorf("file error: %w", err)
		}
		return errors.Join(err, context.Canceled)
	}
	return nil
}

// ---------------------------------------------------
// 📌 Работа с файлами
// ---------------------------------------------------
func utilFiles() {
	if err := os.MkdirAll("tmp", 0o755); err != nil {
		log.Fatal(err)
	}
	file, err := os.CreateTemp("tmp", "data-*.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()
	if _, err := file.WriteString("hello\n"); err != nil {
		log.Fatal(err)
	}
}

// ---------------------------------------------------
// 📌 Шаблон defer для отката
// ---------------------------------------------------
func withLock(mu *sync.Mutex, fn func()) {
	mu.Lock()
	defer mu.Unlock()
	fn()
}

func tempDir(prefix string, fn func(path string) error) error {
	dir, err := os.MkdirTemp("", prefix)
	if err != nil {
		return err
	}
	defer os.RemoveAll(dir)
	return fn(dir)
}
