#include <vector>
#include <algorithm>

// ---------------------------------------------------
// 📌 Генерация всех подмножеств (power set)
// ---------------------------------------------------
std::vector<std::vector<int>> subsets(const std::vector<int>& arr) {
    std::vector<std::vector<int>> res = {{}};
    for (int el : arr) {
        int n = static_cast<int>(res.size());
        for (int i = 0; i < n; ++i) {
            std::vector<int> subset = res[i];
            subset.push_back(el);
            res.push_back(subset);
        }
    }
    return res;
}

// ---------------------------------------------------
// 📌 Генерация всех перестановок (permutations)
// ---------------------------------------------------
std::vector<std::vector<int>> permutations(std::vector<int> arr) {
    std::vector<std::vector<int>> res;
    std::sort(arr.begin(), arr.end());
    do {
        res.push_back(arr);
    } while (std::next_permutation(arr.begin(), arr.end()));
    return res;
}

// ---------------------------------------------------
// 📌 Генерация всех комбинаций (по k элементов из n)
// ---------------------------------------------------
void backtrack_combinations(const std::vector<int>& arr, int k, int start,
                            std::vector<int>& path, std::vector<std::vector<int>>& res) {
    if (static_cast<int>(path.size()) == k) {
        res.push_back(path);
        return;
    }
    for (int i = start; i < static_cast<int>(arr.size()); ++i) {
        path.push_back(arr[i]);
        backtrack_combinations(arr, k, i + 1, path, res);
        path.pop_back();
    }
}

std::vector<std::vector<int>> combinations(const std::vector<int>& arr, int k) {
    std::vector<std::vector<int>> res;
    std::vector<int> path;
    backtrack_combinations(arr, k, 0, path, res);
    return res;
}

// ---------------------------------------------------
// 📌 Генерация всех парных сочетаний (двойной цикл)
// ---------------------------------------------------
std::vector<std::pair<int,int>> allPairs(const std::vector<int>& arr) {
    std::vector<std::pair<int,int>> res;
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(arr.size()); ++j) {
            res.emplace_back(arr[i], arr[j]);
        }
    }
    return res;
}
