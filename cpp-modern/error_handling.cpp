// ============================================
// 📌 Exception Basics
// ============================================
// • try-catch-throw
// • Exception hierarchy
// • Standard exceptions
// • Custom exceptions
// • Catch by reference
// • Rethrowing

// ============================================
// 📌 Exception Safety Guarantees
// ============================================
// • No-throw guarantee
// • Strong exception safety
// • Basic exception safety
// • No exception safety
// • noexcept specifier

// ============================================
// 📌 RAII Pattern
// ============================================
// • Resource acquisition is initialization
// • Smart pointers
// • Lock guards
// • Custom RAII wrappers
// • Scope guards

// ============================================
// 📌 std::expected<T, E> (C++23)
// ============================================

// --- Basics ---
// • Construction
// • has_value() / has_error()
// • value() / error()
// • value_or()
// • operator bool()

// --- Monadic Operations ---
// • and_then (map для value)
// • or_else (map для error)
// • transform (map для value)
// • transform_error (map для error)

// --- Error Propagation ---
// • Chaining operations
// • Early returns
// • Error accumulation

// --- Comparison with Exceptions ---
// • Performance
// • Use cases
// • Migration strategies

// ============================================
// 📌 std::optional<T> (C++17)
// ============================================
// • Nullable values
// • has_value() / value() / value_or()
// • Monadic operations (C++23)
// • and_then / or_else / transform
// • Comparison with pointers

// ============================================
// 📌 Error Codes
// ============================================
// • std::error_code
// • std::error_category
// • std::error_condition
// • system_error
// • Custom error categories

// ============================================
// 📌 Result<T, E> Pattern
// ============================================
// • Implementation
// • Usage patterns
// • Rust-like error handling

// ============================================
// 📌 Comparison of Approaches
// ============================================
// • Exceptions vs expected vs error codes
// • Performance benchmarks
// • When to use which
// • Mixed strategies

// ============================================
// 📌 noexcept и Exception Specifications
// ============================================
// • noexcept specifier
// • noexcept operator
// • Conditional noexcept
// • Move constructors/assignments
// • Performance benefits

// ============================================
// 📌 Best Practices
// ============================================
// • Error handling philosophy
// • Fail-fast vs graceful degradation
// • Logging и monitoring
// • Testing error paths
// • Documentation