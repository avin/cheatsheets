// ============================================
// 📌 Memory Management Anti-Patterns
// ============================================
// ❌ Manual new/delete без RAII
// ❌ Mixing new[] и delete (not delete[])
// ❌ Returning raw pointers от new
// ❌ Storing references в containers
// ✅ Правильные альтернативы

// ============================================
// 📌 Exception Safety Anti-Patterns
// ============================================
// ❌ Throwing в destructors
// ❌ catch(...) без rethrow
// ❌ Игнорирование exception safety guarantees
// ❌ Exceptions в noexcept functions
// ✅ Правильные подходы

// ============================================
// 📌 Concurrency Anti-Patterns
// ============================================
// ❌ Data races
// ❌ Deadlocks (lock ordering)
// ❌ Livelock
// ❌ Забытый unlock
// ❌ Using mutex without RAII
// ✅ Правильная синхронизация

// ============================================
// 📌 Template Anti-Patterns
// ============================================
// ❌ Template bloat
// ❌ Unclear error messages (use concepts!)
// ❌ Overusing SFINAE
// ❌ Template в headers без inline
// ✅ Современные решения

// ============================================
// 📌 Performance Anti-Patterns
// ============================================
// ❌ Unnecessary copies
// ❌ std::endl вместо '\n'
// ❌ Passing large objects by value
// ❌ vector<bool>
// ❌ Premature optimization
// ✅ Правильные практики

// ============================================
// 📌 Modern C++ Misuse
// ============================================
// ❌ Not using auto где нужно
// ❌ Not using range-based for
// ❌ Not using std::optional для nullable
// ❌ Using raw loops вместо algorithms
// ❌ Игнорирование std::expected (C++23)
// ✅ Современный стиль

// ============================================
// 📌 API Design Anti-Patterns
// ============================================
// ❌ Boolean parameters
// ❌ Out parameters вместо return
// ❌ Mixing concerns в одной функции
// ❌ God objects
// ✅ Хороший API design

// ============================================
// 📌 Code Smell Patterns
// ============================================
// ❌ Magic numbers
// ❌ Deep nesting
// ❌ Long functions
// ❌ Duplicate code
// ❌ Commented-out code
// ✅ Clean code practices

// ============================================
// 📌 Undefined Behavior Traps
// ============================================
// ❌ Signed integer overflow
// ❌ Using uninitialized variables
// ❌ Dangling references
// ❌ Iterator invalidation
// ❌ Multiple modifications без sequence point
// ✅ Как избежать UB