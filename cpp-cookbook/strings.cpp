#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <iostream>
#include <sstream>

// ---------------------------------------------------
// 📌 Строка -> вектор символов и обратно
// ---------------------------------------------------
void example_split_join(const std::string& str) {
    std::vector<char> arr(str.begin(), str.end());
    std::string str2(arr.begin(), arr.end());
    std::vector<std::string> chars;
    chars.reserve(str.size());
    for (char c : str) chars.push_back(std::string(1, c));
}

// ---------------------------------------------------
// 📌 Поиск подстроки
// ---------------------------------------------------
void example_find_substring(const std::string& str) {
    bool contains = str.find("abc") != std::string::npos;
    size_t pos = str.find("abc");
}

// ---------------------------------------------------
// 📌 Сравнение строк
// ---------------------------------------------------
void example_compare(std::string s1, std::string s2) {
    bool equal = (s1 == s2);
    std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    bool equalIgnoreCase = (s1 == s2);
}

// ---------------------------------------------------
// 📌 Слияние строк
// ---------------------------------------------------
void example_concat(const std::string& str1, const std::string& str2) {
    std::string merged = str1 + str2;
    // или
    std::string merged2 = str1.append(str2);
}

// ---------------------------------------------------
// 📌 Удаление символов
// ---------------------------------------------------
void example_remove_char(std::string& str) {
    str.erase(std::remove(str.begin(), str.end(), 'x'), str.end());
}

// ---------------------------------------------------
// 📌 Замена символов / подстрок
// ---------------------------------------------------
void example_replace(std::string& str) {
    std::replace(str.begin(), str.end(), 'l', 'X'); // заменить все 'l' на 'X'
    size_t pos = str.find("ll");
    if (pos != std::string::npos) {
        str.replace(pos, 2, "XX");
    }
}

// ---------------------------------------------------
// 📌 Регулярные выражения
// ---------------------------------------------------
void example_regex(const std::string& str) {
    std::regex re("l");
    bool found = std::regex_search(str, re);
    auto matches_begin = std::sregex_iterator(str.begin(), str.end(), re);
    auto matches_end = std::sregex_iterator();
    std::vector<std::string> matches;
    for (auto it = matches_begin; it != matches_end; ++it) {
        matches.push_back(it->str());
    }
}

// ---------------------------------------------------
// 📌 Преобразование с помощью std::stringstream: stoi, to_string, substr, split с помощью stringstream
// ---------------------------------------------------
void example_stringstream(const std::string& str) {
    // конвертация строки в число
    int num;
    std::stringstream ss(str);
    ss >> num;

    // разбиение строки по пробелам
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(str);
    while (iss >> token) {
        tokens.push_back(token);
    }

    // получение подстроки
    std::string sub = str.substr(0, std::min<size_t>(str.size(), 5));

    // удаление всех пробелов
    std::string noSpaces = str;
    noSpaces.erase(std::remove(noSpaces.begin(), noSpaces.end(), ' '), noSpaces.end());
}

// ---------------------------------------------------
// 📌 Разделение строки по разделителю (split)
// ---------------------------------------------------
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

// ---------------------------------------------------
// 📌 Удаление пробелов (trim)
// ---------------------------------------------------
std::string trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? std::string(start, end) : std::string();
}

// ---------------------------------------------------
// 📌 Проверка префикса/суффикса (C++20: starts_with, ends_with)
// ---------------------------------------------------
bool hasPrefix(const std::string& str, const std::string& prefix) {
    return str.starts_with(prefix); // C++20
}

bool hasSuffix(const std::string& str, const std::string& suffix) {
    return str.ends_with(suffix); // C++20
}

// Проверка вхождения подстроки (C++23: contains)
bool hasSubstring(const std::string& str, const std::string& substr) {
    return str.contains(substr);  // C++23 - более читаемо чем find
    // Альтернатива: return str.find(substr) != std::string::npos;
}
