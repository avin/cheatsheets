// ============================================
// 📌 Raw Pointers Problems
// ============================================
// • Memory leaks
// • Dangling pointers
// • Double deletion
// • Ownership ambiguity

// ============================================
// 📌 std::unique_ptr
// ============================================
// • Exclusive ownership
// • make_unique (C++14)
// • Custom deleters
// • Arrays
// • Move semantics
// • Converting to shared_ptr
// • Pimpl idiom
// • Factory functions

// ============================================
// 📌 std::shared_ptr
// ============================================
// • Shared ownership
// • Reference counting
// • make_shared vs new
// • Control block
// • Custom deleters
// • enable_shared_from_this
// • Cyclic references problem
// • Thread safety
// • Atomic operations (C++20)

// ============================================
// 📌 std::weak_ptr
// ============================================
// • Breaking cycles
// • Observing without owning
// • lock() method
// • expired() check
// • Use cases (cache, observer)

// ============================================
// 📌 Memory Management
// ============================================

// --- new и delete ---
// • Placement new
// • Array new/delete
// • Alignment
// • Nothrow new

// --- Allocators ---
// • std::allocator
// • Custom allocators
// • Polymorphic allocators (C++17)
// • std::pmr namespace
// • Memory pools

// --- Smart Pointers Comparison ---
// • Performance overhead
// • When to use which
// • Conversion between types

// ============================================
// 📌 RAII Patterns
// ============================================
// • File handles
// • Lock guards
// • Database connections
// • Network sockets
// • Custom RAII wrappers
// • Scope guards

// ============================================
// 📌 Object Lifetime
// ============================================
// • Stack vs Heap
// • Automatic storage duration
// • Dynamic storage duration
// • Static storage duration
// • Thread storage duration

// ============================================
// 📌 Move Semantics
// ============================================
// • Rvalue references
// • Move constructor/assignment
// • std::move
// • std::forward
// • Perfect forwarding
// • Move-only types

// ============================================
// 📌 Advanced Topics
// ============================================
// • Type-erased unique_ptr
// • Intrusive smart pointers
// • Observer ptr (C++26 proposal)
// • Small buffer optimization
// • Memory alignment
// • Cache-friendly structures