package cheatsheets

import (
	"bufio"
	"context"
	"errors"
	"fmt"
	"os"
	"time"
)

// ---------------------------------------------------
// 📌 Классический for
// ---------------------------------------------------
func loopClassic(slice []int) {
	for i := 0; i < len(slice); i++ {
		fmt.Println(i, slice[i])
	}
}

// ---------------------------------------------------
// 📌 for как while
// ---------------------------------------------------
func loopWhile(n int, deadline time.Time) {
	for n > 0 {
		n--
	}

	for {
		if time.Now().After(deadline) {
			break
		}
		time.Sleep(10 * time.Millisecond)
	}
}

// ---------------------------------------------------
// 📌 Итерирование по коллекциям
// ---------------------------------------------------
func loopRange(slice []int, dict map[string]int, text string) {
	for idx, val := range slice {
		fmt.Println(idx, val)
	}
	for key, val := range dict {
		fmt.Printf("%s=%d\n", key, val)
	}
	for pos, r := range text {
		fmt.Printf("%d: %c\n", pos, r)
	}
}

// ---------------------------------------------------
// 📌 Guard-проверки
// ---------------------------------------------------
func loopGuard(path string, slice []int, limit int) error {
	if err := os.MkdirAll(path, 0o755); err != nil {
		return err
	}
	if size := len(slice); size > limit {
		fmt.Println("overflow")
	}
	return nil
}

// ---------------------------------------------------
// 📌 switch с условиями и типами
// ---------------------------------------------------
func loopSwitch(score int, anyValue any) string {
	var grade string
	switch {
	case score >= 90:
		grade = "A"
	case score >= 75:
		grade = "B"
	default:
		grade = "C"
	}

	switch v := anyValue.(type) {
	case string:
		fmt.Println("string", v)
	case int:
		fmt.Println("int", v)
	default:
		fmt.Printf("unknown %T\n", v)
	}

	return grade
}

// ---------------------------------------------------
// 📌 select по каналам
// ---------------------------------------------------
func loopSelect(payload string) {
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
// 📌 Управление циклом
// ---------------------------------------------------
func loopControl(slice []int) error {
	for i := range slice {
		if slice[i]%2 == 0 {
			continue
		}
		if i > 10 {
			break
		}
	}

	const maxRetries = 3
	var retries int
	work := func() error { return errors.New("temporary") }

retry:
	if err := work(); err != nil {
		retries++
		if retries < maxRetries {
			time.Sleep(100 * time.Millisecond)
			goto retry
		}
		return err
	}
	return nil
}

// ---------------------------------------------------
// 📌 Дефер и финализация
// ---------------------------------------------------
func loopDefer(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	scanner := bufio.NewScanner(f)
	handle := func(string) {}
	for scanner.Scan() {
		handle(scanner.Text())
	}
	return scanner.Err()
}
