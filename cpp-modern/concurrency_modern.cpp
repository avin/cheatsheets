// ============================================
// 📌 Thread Basics
// ============================================
// • std::thread creation
// • join / detach
// • std::jthread (C++20) - auto-joining
// • Thread IDs
// • Hardware concurrency

// ============================================
// 📌 Synchronization Primitives
// ============================================

// --- Mutexes ---
// • std::mutex
// • std::recursive_mutex
// • std::timed_mutex
// • std::shared_mutex (C++17)

// --- Locks ---
// • std::lock_guard
// • std::unique_lock
// • std::shared_lock (C++17)
// • std::scoped_lock (C++17)
// • std::lock (multiple mutexes)

// --- Condition Variables ---
// • std::condition_variable
// • std::condition_variable_any
// • wait / wait_for / wait_until
// • notify_one / notify_all
// • Spurious wakeups

// --- Semaphores (C++20) ---
// • std::counting_semaphore
// • std::binary_semaphore
// • Use cases

// --- Latches и Barriers (C++20) ---
// • std::latch - single-use barrier
// • std::barrier - reusable barrier
// • Coordination patterns

// ============================================
// 📌 Atomic Operations
// ============================================

// --- std::atomic ---
// • Basic operations
// • Memory ordering (acquire, release, seq_cst, etc.)
// • Compare-exchange (weak/strong)
// • fetch_add / fetch_sub / etc.

// --- Atomic Smart Pointers (C++20) ---
// • std::atomic<std::shared_ptr>
// • std::atomic<std::weak_ptr>

// --- Atomic Wait/Notify (C++20) ---
// • wait / notify_one / notify_all
// • Lightweight synchronization

// ============================================
// 📌 Memory Model
// ============================================
// • Sequential consistency
// • Acquire-Release semantics
// • Relaxed ordering
// • Memory barriers
// • Happens-before relationships
// • Data races

// ============================================
// 📌 Async Programming
// ============================================

// --- Futures и Promises ---
// • std::async
// • std::future / std::shared_future
// • std::promise
// • std::packaged_task
// • Launch policies (async, deferred)

// --- Coroutines (C++20) ---
// • Async coroutines
// • task<T> implementation
// • generator<T> implementation
// • co_await / co_yield / co_return
// • Custom awaitables

// ============================================
// 📌 Thread Pools
// ============================================
// • Basic thread pool implementation
// • Task queues
// • Work stealing
// • Executor concept

// ============================================
// 📌 Lock-Free Programming
// ============================================
// • Lock-free vs wait-free
// • Lock-free stack
// • Lock-free queue
// • ABA problem
// • Hazard pointers

// ============================================
// 📌 Stop Tokens (C++20)
// ============================================
// • std::stop_source
// • std::stop_token
// • Cooperative cancellation
// • std::jthread integration

// ============================================
// 📌 Common Patterns
// ============================================
// • Producer-consumer
// • Reader-writer lock
// • Double-checked locking
// • Monitor pattern
// • Active object
// • Thread-safe singleton

// ============================================
// 📌 Performance и Best Practices
// ============================================
// • Lock contention
// • False sharing
// • Cache coherency
// • NUMA awareness
// • Debugging race conditions