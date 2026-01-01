// ============================================
// 📌 Move Semantics Optimization
// ============================================
// • RVO / NRVO (Return Value Optimization)
// • Move constructors/assignments
// • std::move usage
// • Perfect forwarding
// • Move-only types
// • Benchmarks

// ============================================
// 📌 Memory Optimization
// ============================================
// • Cache-friendly structures
// • Data-oriented design
// • Memory alignment
// • Padding и packing
// • Small buffer optimization (SBO)
// • Object pooling
// • Custom allocators

// ============================================
// 📌 Compile-Time Computation
// ============================================
// • constexpr everything
// • consteval for guaranteed compile-time
// • Compile-time containers
// • Template metaprogramming
// • Zero-overhead abstractions

// ============================================
// 📌 Copy Elision
// ============================================
// • Guaranteed copy elision (C++17)
// • NRVO
// • When it applies
// • Examples

// ============================================
// 📌 Small Object Optimization
// ============================================
// • std::string SSO
// • std::function SFO
// • std::any implementation
// • Custom implementations

// ============================================
// 📌 Inline Optimization
// ============================================
// • inline keyword
// • constexpr implies inline
// • [[gnu::always_inline]]
// • When compiler inlines
// • LTO (Link-Time Optimization)

// ============================================
// 📌 Branch Prediction
// ============================================
// • [[likely]] / [[unlikely]] (C++20)
// • Branch-free algorithms
// • Computed goto (non-standard)
// • Switch optimization

// ============================================
// 📌 SIMD и Vectorization
// ============================================
// • Auto-vectorization
// • Intrinsics (AVX, SSE, NEON)
// • std::simd (C++26 proposal)
// • Alignment requirements
// • AoS vs SoA

// ============================================
// 📌 Parallel Algorithms
// ============================================
// • Execution policies
// • When to parallelize
// • Overhead considerations
// • Thread pool overhead

// ============================================
// 📌 String Optimization
// ============================================
// • string_view usage
// • Avoiding copies
// • In-place operations
// • Small string optimization

// ============================================
// 📌 Container Optimization
// ============================================
// • reserve() для vector
// • emplace vs insert/push
// • Flat containers для small datasets
// • Node-based vs contiguous
// • Custom hash functions

// ============================================
// 📌 Template Optimization
// ============================================
// • Avoiding template bloat
// • Type erasure
// • extern template
// • Concepts для early errors

// ============================================
// 📌 Profiling и Measurement
// ============================================
// • std::chrono для benchmarks
// • CPU profilers (perf, VTune)
// • Memory profilers (Valgrind, sanitizers)
// • Compiler optimization flags
// • PGO (Profile-Guided Optimization)

// ============================================
// 📌 Common Pitfalls
// ============================================
// • Premature optimization
// • Micro-optimizations that hurt
// • False sharing
// • Lock contention
// • Memory fragmentation