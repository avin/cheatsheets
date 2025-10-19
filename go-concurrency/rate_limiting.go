package goconcurrency

import (
	"context"
	"errors"
	"fmt"
	"time"

	"golang.org/x/sync/singleflight"
	"golang.org/x/time/rate"
)

// ---------------------------------------------------
// 📌 Token bucket через time.Ticker
// ---------------------------------------------------
func rateLimiterTicker(ctx context.Context, requests []int, perSecond int) {
	interval := time.Second / time.Duration(perSecond)
	ticker := time.NewTicker(interval)
	defer ticker.Stop()

	for _, req := range requests {
		select {
		case <-ticker.C:
			fmt.Println("handle request", req)
		case <-ctx.Done():
			return
		}
	}
}

// ---------------------------------------------------
// 📌 rate.Limiter с burst
// ---------------------------------------------------
func rateLimiterLibrary(ctx context.Context, requests []int, limit rate.Limit, burst int) {
	limiter := rate.NewLimiter(limit, burst)
	for _, id := range requests {
		if err := limiter.Wait(ctx); err != nil {
			fmt.Println("limiter cancelled:", err)
			return
		}
		fmt.Println("processed", id)
	}
}

// ---------------------------------------------------
// 📌 Экспоненциальный backoff с джиттером
// ---------------------------------------------------
func retryWithBackoff(
	ctx context.Context,
	maxAttempts int,
	baseDelay time.Duration,
	fn func(attempt int) error,
) error {
	delay := baseDelay
	for attempt := 1; attempt <= maxAttempts; attempt++ {
		err := fn(attempt)
		if err == nil {
			return nil
		}

		if attempt == maxAttempts {
			return err
		}

		jitter := time.Duration(float64(delay) * 0.1)
		sleep := delay + time.Duration(randFloat64()*float64(jitter))

		select {
		case <-time.After(sleep):
			delay *= 2
			if delay > 5*time.Second {
				delay = 5 * time.Second
			}
		case <-ctx.Done():
			return ctx.Err()
		}
	}
	return nil
}

func randFloat64() float64 {
	return float64(time.Now().UnixNano()%1000) / 1000.0
}

// ---------------------------------------------------
// 📌 singleflight: предотвращение дублирующих запросов
// ---------------------------------------------------
type cacheLoader struct {
	group singleflight.Group
	cache map[string]string
}

func (c *cacheLoader) Get(ctx context.Context, key string) (string, error) {
	if val, ok := c.cache[key]; ok {
		return val, nil
	}

	val, err := c.group.Do(key, func() (interface{}, error) {
		time.Sleep(50 * time.Millisecond) // имитация IO
		data := fmt.Sprintf("value:%s", key)
		if c.cache == nil {
			c.cache = make(map[string]string)
		}
		c.cache[key] = data
		return data, nil
	})
	if err != nil {
		return "", err
	}
	return val.(string), nil
}

// ---------------------------------------------------
// 📌 Ограничение запросов per user
// ---------------------------------------------------
type userLimiter struct {
	limiters map[string]*rate.Limiter
	limit    rate.Limit
	burst    int
}

func newUserLimiter(limit rate.Limit, burst int) *userLimiter {
	return &userLimiter{
		limiters: make(map[string]*rate.Limiter),
		limit:    limit,
		burst:    burst,
	}
}

func (u *userLimiter) Allow(ctx context.Context, userID string) error {
	limiter, ok := u.limiters[userID]
	if !ok {
		limiter = rate.NewLimiter(u.limit, u.burst)
		u.limiters[userID] = limiter
	}
	if !limiter.Allow() {
		return errors.New("rate limit exceeded for user")
	}
	return ctx.Err()
}
