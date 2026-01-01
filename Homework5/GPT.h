// student_avl_tree.h
#pragma once  // 单头文件防多重包含

#include <algorithm>   // std::max
#include <cassert>     // assert
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <functional>  // std::less
#include <iterator>    // 迭代器标签
#include <utility>     // std::swap

namespace student_std {

// 模板参数：Key 为键类型，Comp 为比较器（缺省为严格弱序 std::less<Key>）
template <typename Key, typename Comp = std::less<Key>>
class avl_tree {
public:
    // 节点类型（嵌套在树里，外部通过 node_type 引用）
    class avl_node {
    public:
        using size_type = std::size_t;  // 便捷别名

        // 只读访问节点里存放的键（测试里 Key=allocation_counter）
        Key const& value() const noexcept { return m_key; }  // 常量版本
        Key const& value()       noexcept { return m_key; }  // 非常量版本（题目接口即如此）

        // 常量访问父/左/右指针（供只读检查，如 is_bst/height_valid）
        avl_node const* parent() const noexcept { return m_parent; }
        avl_node const* left()   const noexcept { return m_left; }
        avl_node const* right()  const noexcept { return m_right; }

        // 非常量访问父/左/右指针（内部维护用）
        avl_node* parent() noexcept { return m_parent; }
        avl_node* left()   noexcept { return m_left; }
        avl_node* right()  noexcept { return m_right; }

        // 子树大小（包含自身）
        size_type size()   const noexcept { return m_size; }
        // 节点高度：按测试约定，叶子=0；因此空指针高度=-1（见辅助函数）
        size_type height() const noexcept { return static_cast<size_type>(m_height); }

    private:
        friend class avl_tree;  // 允许外部类访问私有成员以实现树操作

        // 构造：新节点为叶子，父指针设为给定父
        explicit avl_node(Key const& k, avl_node* p)
            : m_key(k), m_parent(p), m_left(nullptr), m_right(nullptr),
              m_size(1), m_height(0) {}  // 叶子高度=0、子树大小=1

        Key m_key{};             // 键值
        avl_node* m_parent{nullptr}; // 父指针
        avl_node* m_left{nullptr};   // 左子
        avl_node* m_right{nullptr};  // 右子
        size_type m_size{1};     // 以该节点为根的子树大小
        int m_height{0};         // 节点高度（非负，叶子=0；空节点高度在辅助函数里返回-1）
    };

    // 只读双向迭代器（测试要求的是 const 接口：operator->() 返回 const 节点指针）
    class iterator {
    public:
        using value_type        = avl_node;                      // 迭代器解引用得到的类型（节点）
        using reference         = value_type const&;             // 引用为 const
        using pointer           = value_type const*;             // 指针为 const
        using difference_type   = std::ptrdiff_t;                // 差值类型
        using iterator_category = std::bidirectional_iterator_tag; // 双向迭代器

        // 缺省构造：空迭代器（等同 end()）
        explicit iterator(pointer node = nullptr) noexcept
            : m_node(node), m_owner_root(nullptr) {}

        // 前置++：中序遍历到下一个节点
        iterator& operator++() noexcept {
            if (!m_node) return *this;               // end()++ 无操作
            if (auto r = m_node->right()) {          // 若有右子，则去右子树最左
                m_node = r;
                while (m_node->left()) m_node = m_node->left();
            } else {                                  // 否则向上，直到当前节点是父亲的左孩子
                auto cur = m_node;
                auto p = m_node->parent();
                while (p && cur == p->right()) {     // 一直向上，跳过已经访问过的右链
                    cur = p;
                    p = p->parent();
                }
                m_node = p;                           // 可能为 nullptr（到达 end）
            }
            return *this;
        }
        // 后置++：保存副本后前进
        iterator operator++(int) noexcept { auto t = *this; ++(*this); return t; }

        // 前置--：中序遍历到上一个节点
        iterator& operator--() noexcept {
            if (!m_node) {                            // 从 end() 往回：跳到整棵树的最大节点
                if (m_owner_root) {
                    m_node = m_owner_root;
                    while (m_node && m_node->right()) m_node = m_node->right();
                }
                return *this;
            }
            if (auto l = m_node->left()) {            // 若有左子，则去左子树最右
                m_node = l;
                while (m_node->right()) m_node = m_node->right();
            } else {                                  // 否则向上，直到当前节点是父亲的右孩子
                auto cur = m_node;
                auto p = m_node->parent();
                while (p && cur == p->left()) {
                    cur = p;
                    p = p->parent();
                }
                m_node = p;                           // 可能为 nullptr（到达 rend 前一位）
            }
            return *this;
        }
        // 后置--：保存副本后后退
        iterator operator--(int) noexcept { auto t = *this; --(*this); return t; }

        // 解引用（测试用 it->value()）
        reference operator*()  const noexcept { assert(m_node); return *m_node; }
        pointer   operator->() const noexcept { assert(m_node); return m_node; }

        // 迭代器比较
        friend bool operator==(iterator const& a, iterator const& b) noexcept {
            return a.m_node == b.m_node;
        }
        friend bool operator!=(iterator const& a, iterator const& b) noexcept {
            return !(a == b);
        }

    private:
        friend class avl_tree;  // 允许树为迭代器设置 root

        // 内部构造：携带指向所属树根的指针，以便从 end() -- 能定位到最大值
        iterator(pointer node, pointer root) noexcept
            : m_node(node), m_owner_root(root) {}

        pointer m_node;         // 当前节点（const 指针）
        pointer m_owner_root;   // 整棵树的根（const 指针），仅用于 end()--
    };

public:
    // 公开别名，便于测试文件书写
    using key_type       = Key;
    using node_type      = avl_node;
    using size_type      = std::size_t;
    using comparison     = Comp;
    using const_iterator = iterator;  // 对外暴露只读迭代器

    avl_tree() = default;             // 默认构造：空树
    ~avl_tree() { destroy_subtree(m_root); }  // 析构：递归释放所有节点

    // 插入键 key；若已存在则返回已存在节点的迭代器；否则插入新叶并自下而上重平衡
    iterator insert(Key const& key) {
        if (!m_root) {                                  // 空树：新建根
            m_root = new node_type(key, nullptr);
            return make_iterator(m_root);
        }
        node_type* p = m_root;                          // p 用于向下查找插入位置
        node_type* parent = nullptr;                    // 记录最后的父节点
        bool goLeft = false;                            // 记录应插入到父亲的左/右

        while (p) {                                     // 常规 BST 查找插入点
            parent = p;
            if (m_comp(key, p->m_key)) {               // key < p
                p = p->m_left;
                goLeft = true;
            } else if (m_comp(p->m_key, key)) {        // p < key
                p = p->m_right;
                goLeft = false;
            } else {
                return make_iterator(parent);          // 已存在：直接返回该节点
            }
        }

        node_type* n = new node_type(key, parent);      // 新建叶子
        if (goLeft) parent->m_left = n; else parent->m_right = n; // 接到父亲

        rebalance_up(parent);                           // 从父亲开始一路向上重平衡
        return make_iterator(n);                        // 返回新插入节点
    }

    // 按键删除；若不存在返回 end()，若存在返回其后继（与 std::set::erase(it) 行为一致）
    iterator erase(Key const& key) {
        node_type* z = find_node(key);                  // 查找要删的节点
        if (!z) return end();                           // 没找到：返回 end()

        erase_node(z);                                  // 执行删除 + 重平衡
        return make_iterator(lower_bound_node(key));    // 返回删除键的“下界”（即后继或 end）
    }

    // 查找键；返回指向该键的迭代器（不存在则 end()）
    iterator find(Key const& key) const { return make_iterator(find_node(key)); }

    // 是否包含某键
    bool contains(Key const& key) const { return find_node(key) != nullptr; }

    // 树的元素数量（根为空为 0）
    size_type size() const noexcept { return m_root ? m_root->m_size : 0u; }

    // 树的高度（根为空记为 0；与测试里 t.height() 用法对齐）
    size_type height() const noexcept { return m_root ? static_cast<size_type>(m_root->m_height) : 0u; }

    // 中序遍历起点（最小值）
    iterator begin() const noexcept { return make_iterator(min_node(m_root)); }

    // 尾后迭代器（携带 root 以便 --）
    iterator end()   const noexcept { return iterator(nullptr, m_root); }

    // 根节点的“迭代器”包装（测试用 t.root()->size()/height()）
    iterator root()  const noexcept { return make_iterator(m_root); }

private:
    node_type* m_root{nullptr}; // 根指针
    comparison m_comp{};        // 比较器对象

    // 将 const 节点指针打包成迭代器（同时携带当前树根，以支持 end()--）
    iterator make_iterator(node_type const* n) const noexcept {
        return iterator(n, m_root);
    }

    // 安全获取子树大小（空子树大小为 0）
    static size_type node_size(node_type const* n) noexcept { return n ? n->m_size : 0u; }

    // 安全获取高度：空指针高度按 -1（这是测试的约定）
    static int height_or_neg1(node_type const* n) noexcept { return n ? n->m_height : -1; }

    // 从叶到根更新单个节点的 size 与 height（高度定义：叶=0，空=-1）
    static void update(node_type* n) noexcept {
        if (!n) return;  // 空指针保护
        n->m_size   = 1u + node_size(n->m_left) + node_size(n->m_right);  // 1 + 左子树大小 + 右子树大小
        int lh = height_or_neg1(n->m_left);   // 左子高度（空=-1）
        int rh = height_or_neg1(n->m_right);  // 右子高度（空=-1）
        n->m_height = 1 + (lh > rh ? lh : rh); // 高度公式：1 + max(lh   , rh)
    }

    // 计算平衡因子（左高 - 右高）；空子树高度视为 -1
    static int balance_factor(node_type const* n) noexcept {
        if (!n) return 0;  // 空指针无所谓
        return height_or_neg1(n->m_left) - height_or_neg1(n->m_right);
    }

    // 找到以 n 为根的最小节点（一路向左）
    static node_type* min_node(node_type* n) noexcept {
        while (n && n->m_left) n = n->m_left;
        return n;
    }
    // 常量版本重载
    static node_type const* min_node(node_type const* n) noexcept {
        while (n && n->left()) n = n->left();
        return n;
    }

    // 工具：在已知 parent 的前提下，把 parent 的 oldChild 指针替换为 newChild
    // 注意：同时维护 newChild->m_parent，若 parent 为空则更新 m_root
    void replace_parent_link(node_type* parent, node_type* oldChild, node_type* newChild) noexcept {
        if (!parent) {                     // oldChild 是根：更新整棵树根
            m_root = newChild;
        } else if (parent->m_left == oldChild) {
            parent->m_left = newChild;     // oldChild 是父亲的左孩子
        } else if (parent->m_right == oldChild) {
            parent->m_right = newChild;    // oldChild 是父亲的右孩子
        }
        if (newChild) newChild->m_parent = parent; // 维护 newChild 的父指针
    }

    // 左旋（x 必有右子 y）：返回新的子树根 y
    // 旋转内部只改动 x/y/T2 三者的父子关系，并正确更新高度/大小；
    // 至于 y 与 x 原父亲的连接，由调用方通过 replace_parent_link 完成
    static node_type* rotate_left(node_type* x) noexcept {
        node_type* y  = x->m_right;      // 右孩子（非空）
        node_type* T2 = y->m_left;       // y 的左子（旋转后成为 x 的右子）

        y->m_left  = x;                  // y 的左子改为 x
        x->m_right = T2;                 // x 的右子改为 T2（可能为空）

        y->m_parent = x->m_parent;       // y 接到 x 原父亲（调用方负责把父亲指向 y）
        x->m_parent = y;                 // x 的父亲变为 y
        if (T2) T2->m_parent = x;        // T2 的父亲变为 x（若 T2 不为空）

        update(x);                       // 更新低层节点 x
        update(y);                       // 再更新高层节点 y
        return y;                        // 返回新子树根
    }

    // 右旋（y 必有左子 x）：返回新的子树根 x
    static node_type* rotate_right(node_type* y) noexcept {
        node_type* x  = y->m_left;       // 左孩子（非空）
        node_type* T2 = x->m_right;      // x 的右子（旋转后成为 y 的左子）

        x->m_right = y;                  // x 的右子改为 y
        y->m_left  = T2;                 // y 的左子改为 T2（可能为空）

        x->m_parent = y->m_parent;       // x 接到 y 原父亲
        y->m_parent = x;                 // y 的父亲变为 x
        if (T2) T2->m_parent = y;        // T2 的父亲变为 y（若 T2 不为空）

        update(y);                       // 更新低层节点 y
        update(x);                       // 再更新高层节点 x
        return x;                        // 返回新子树根
    }

    // 在节点 n 处进行一次“局部重平衡”，
    // 如果 |bf(n)|<=1 则只更新并返回 n；
    // 若左重或右重则做单/双旋，并返回该处新的子树根。
    node_type* rebalance_at(node_type* n) noexcept {
        if (!n) return n;                // 空指针保护
        update(n);                       // 先基于当前孩子信息更新 n 的 size/height
        int bf = balance_factor(n);      // 计算平衡因子

        if (bf > 1) {                    // 左重
            // 若为 LR 型（左子右高），需先对左孩子做左旋，化为 LL
            if (balance_factor(n->m_left) < 0) {
                node_type* newLeft = rotate_left(n->m_left); // 对左孩子左旋
                n->m_left = newLeft;                         // 接回 n 的左孩子
                newLeft->m_parent = n;                       // 维护父指针
                update(n);                                   // 左孩子改变后，先更新 n
            }
            node_type* p0 = n->m_parent;   // 旋转前保存父节点
            node_type* newRoot = rotate_right(n); // 再对 n 做右旋（LL 型）
            replace_parent_link(p0, n, newRoot);  // 把 newRoot 接回到 p0
            return newRoot;               // 返回该子树新的根
        }

        if (bf < -1) {                   // 右重
            // 若为 RL 型（右子左高），先对右孩子右旋，化为 RR
            if (balance_factor(n->m_right) > 0) {
                node_type* newRight = rotate_right(n->m_right); // 对右孩子右旋
                n->m_right = newRight;                          // 接回 n 的右孩子
                newRight->m_parent = n;                          // 维护父指针
                update(n);                                       // 右孩子改变后，先更新 n
            }
            node_type* p0 = n->m_parent;   // 旋转前保存父节点
            node_type* newRoot = rotate_left(n);  // 再对 n 做左旋（RR 型）
            replace_parent_link(p0, n, newRoot);  // 接回到父节点
            return newRoot;               // 返回该子树新的根
        }

        return n;                        // 已平衡：直接返回
    }

    // 自下而上从 start 节点一路重平衡到根
    void rebalance_up(node_type* start) noexcept {
        node_type* cur = start;          // 从给定起点（常为插入/删除处的父节点）开始
        while (cur) {                    // 逐层向上
            node_type* newCur = rebalance_at(cur); // 对当前结点做“局部重平衡”
            cur = newCur->m_parent;      // 上移到其父（注意 newCur 可能是旋转后的新根）
        }
    }

    // 按键查找：返回对应节点指针（不存在返回 nullptr）
    node_type* find_node(Key const& key) const {
        node_type* cur = m_root;         // 从根开始
        while (cur) {                    // 常规 BST 查找
            if (m_comp(key, cur->m_key)) cur = cur->m_left;
            else if (m_comp(cur->m_key, key)) cur = cur->m_right;
            else return cur;             // 找到
        }
        return nullptr;                  // 未找到
    }

    // lower_bound：返回“第一个不小于 key 的节点”
    node_type* lower_bound_node(Key const& key) const {
        node_type* cur = m_root;         // 从根开始
        node_type* ans = nullptr;        // 暂存候选答案
        while (cur) {
            if (!m_comp(cur->m_key, key)) { // cur->key >= key
                ans = cur;               // 更新候选
                cur = cur->m_left;      // 继续向左寻找更小但仍 >= key 的
            } else {
                cur = cur->m_right;     // cur->key < key，则向右
            }
        }
        return ans;                      // 可能为 nullptr（即不存在 >= key 的元素）
    }

    // 删除指定节点 z（保证 z 非空且存在于树中）
    void erase_node(node_type* z) {
        if (!z->m_left || !z->m_right) {      // 度为 0 或 1：直接接上唯一孩子（可能为空）
            node_type* child  = z->m_left ? z->m_left : z->m_right; // 唯一孩子或空
            node_type* parent = z->m_parent;    // 记录父亲（用于从此处开始重平衡）

            replace_parent_link(parent, z, child); // 父亲改指向 child（若 parent 为空则更新根）
            delete z;                               // 释放节点
            rebalance_up(parent);                   // 从 parent 开始向上重平衡
        } else {                                    // 度为 2：用后继替代
            node_type* s = z->m_right;             // 后继在右子树最左
            while (s->m_left) s = s->m_left;

            std::swap(z->m_key, s->m_key);         // 与后继交换键值（避免移动外部 Key 对象）

            node_type* child  = s->m_right;        // 后继最多只有一个右孩子
            node_type* parent = s->m_parent;       // 记录后继的父亲（重平衡起点）

            if (parent->m_left == s) parent->m_left = child;  // 从父亲摘掉后继
            else                      parent->m_right = child;
            if (child) child->m_parent = parent;              // 维护孩子父指针

            delete s;                               // 释放后继节点
            rebalance_up(parent);                   // 从 parent 开始向上重平衡
        }
    }

    // 递归销毁整棵子树（后序）
    static void destroy_subtree(node_type* n) noexcept {
        if (!n) return;
        destroy_subtree(n->m_left);
        destroy_subtree(n->m_right);
        delete n;
    }
};

} // namespace student_std