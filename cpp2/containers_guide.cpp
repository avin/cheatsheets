// ============================================
// 📌 Decision Tree (когда что использовать)
// ============================================
// • Flowchart в комментариях
// • Performance characteristics table
// • Memory overhead comparison

// ============================================
// 📌 Sequential Containers
// ============================================

// --- std::vector ---
// • Basics и construction
// • Capacity vs size
// • reserve() и shrink_to_fit()
// • Reallocation и iterator invalidation
// • emplace_back vs push_back
// • Best practices

// --- std::array ---
// • Stack allocation
// • Compile-time size
// • vs C arrays
// • Use cases

// --- std::deque ---
// • Double-ended queue
// • When to use vs vector
// • Performance characteristics

// --- std::list / std::forward_list ---
// • Doubly/singly linked lists
// • When to use
// • splice operations
// • Performance pitfalls

// ============================================
// 📌 Associative Containers
// ============================================

// --- std::set / std::multiset ---
// • Ordered sets
// • Custom comparators
// • extract() и merge()
// • node handles (C++17)

// --- std::map / std::multimap ---
// • Ordered maps
// • operator[] vs insert vs emplace
// • try_emplace (C++17)
// • insert_or_assign (C++17)

// ============================================
// 📌 Unordered Containers
// ============================================

// --- std::unordered_set / std::unordered_multiset ---
// • Hash tables
// • Custom hash functions
// • Bucket interface
// • Load factor

// --- std::unordered_map / std::unordered_multimap ---
// • Hash maps
// • Collision handling
// • Performance tuning

// ============================================
// 📌 Flat Containers (C++23)
// ============================================

// --- std::flat_set / std::flat_multiset ---
// • Sorted vector-based
// • Cache-friendly
// • vs std::set performance

// --- std::flat_map / std::flat_multimap ---
// • Two-vector implementation
// • keys() / values() access
// • Use cases и trade-offs

// ============================================
// 📌 Container Adaptors
// ============================================
// • std::stack
// • std::queue
// • std::priority_queue
// • Custom underlying containers

// ============================================
// 📌 Specialized Containers
// ============================================

// --- std::span (C++20) ---
// • Non-owning view
// • Dynamic vs fixed extent
// • Use cases

// --- std::mdspan (C++23) ---
// • Multidimensional views
// • Layouts
// • Accessors

// --- std::string ---
// • SSO (Small String Optimization)
// • string_view (C++17)
// • Performance tips

// ============================================
// 📌 Comparison и Selection Guide
// ============================================
// • Complexity tables (Big-O)
// • Memory layout diagrams
// • Benchmark comparisons
// • Real-world use cases