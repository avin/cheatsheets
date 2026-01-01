// ============================================
// 📌 std::string Basics
// ============================================
// • Construction
// • SSO (Small String Optimization)
// • Capacity vs size
// • reserve / shrink_to_fit
// • Concatenation
// • Substring / substr
// • find / rfind / find_first_of / find_last_of
// • starts_with / ends_with (C++20)
// • contains (C++23)
// • Comparisons

// ============================================
// 📌 std::string_view (C++17)
// ============================================
// • Non-owning view
// • Performance benefits
// • Lifetime issues
// • Use cases
// • Conversion to/from string

// ============================================
// 📌 String Formatting
// ============================================
// • std::format (C++20)
// • std::print (C++23)
// • Custom formatters
// • Format string syntax

// ============================================
// 📌 String Manipulation
// ============================================
// • Case conversion (toupper/tolower)
// • Trimming whitespace
// • Splitting strings
// • Joining strings
// • Replacing substrings
// • Padding (left/right/center)
// • Repeating strings

// ============================================
// 📌 Regular Expressions
// ============================================

// --- std::regex ---
// • Pattern syntax (ECMAScript, POSIX, etc.)
// • regex_match vs regex_search
// • regex_replace
// • smatch / cmatch
// • Capture groups
// • Lookahead/lookbehind
// • Performance considerations

// --- Common Patterns ---
// • Email validation
// • URL parsing
// • Phone numbers
// • Date formats
// • IP addresses

// ============================================
// 📌 String Parsing
// ============================================
// • std::stoi / stol / stof / stod
// • std::from_chars (C++17) - fast parsing
// • std::to_chars (C++17) - fast formatting
// • Custom parsers
// • Tokenization
// • CSV parsing
// • JSON parsing basics

// ============================================
// 📌 Wide Strings и Unicode
// ============================================
// • std::wstring / std::u8string / std::u16string / std::u32string
// • char8_t (C++20)
// • UTF-8 / UTF-16 / UTF-32
// • Conversion between encodings
// • std::codecvt (deprecated)
// • Third-party libraries (ICU)
// • Unicode normalization

// ============================================
// 📌 String Literals
// ============================================
// • Raw string literals R"(...)"
// • User-defined literals
// • ""s (std::string literal)
// • ""sv (std::string_view literal)
// • ""i / ""if / etc. (complex numbers)

// ============================================
// 📌 Performance Optimization
// ============================================
// • String pooling
// • Copy-on-write (historical)
// • Move semantics
// • Avoid temporaries
// • In-place modifications
// • Benchmarks