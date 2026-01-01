#include <stack>

// ---------------------------------------------------
// 📌 Рекурсия с базовым случаем
// ---------------------------------------------------
void recur(int n) {
    if (n == 0) return;
    recur(n - 1);
}

// ---------------------------------------------------
// 📌 Итерация через стек (DFS) для бинарного дерева
// ---------------------------------------------------
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v): val(v), left(nullptr), right(nullptr) {}
};

void dfs_iter(TreeNode* root) {
    if (!root) return;
    std::stack<TreeNode*> st;
    st.push(root);
    while (!st.empty()) {
        TreeNode* node = st.top();
        st.pop();
        // обработка node->val
        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left);
    }
}

// ---------------------------------------------------
// 📌 Хвостовая рекурсия (в C++ не гарантируется оптимизация)
// ---------------------------------------------------
long long tailRec(int n, long long acc = 1) {
    if (n == 0) return acc;
    return tailRec(n - 1, acc * n);
}
