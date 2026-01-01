#ifndef STUDENT_INORDER_ITERATOR_H
#define STUDENT_INORDER_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace student_std 
{
    template <typename BinaryTree>
    class inorder_iterator 
    {
    public:
        using value_type = typename BinaryTree::value_type;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type const&;
        using pointer = value_type const*;

    private:
        BinaryTree const* now;

        BinaryTree const* left_side(BinaryTree const* node)
        {
            if(!node) return nullptr;
            while(node->left()) node = node->left();
            return node;
        }

    public:

        inorder_iterator() : now(nullptr) {}

        // 从指针构造：自动找到中序遍历起始位置
        explicit inorder_iterator(BinaryTree const* node) 
            : now(node ? left_side(node) : nullptr) {}

        // 解引用
        reference operator*() const { return now->value(); }
        pointer operator->() const { return &(now->value()); }

        inorder_iterator& operator++() //前置++
        {
            if(!now) return *this;

            if(now->right()) now = left_side(now->right()); //存在右子树
            else //不存在，下一个节点在祖先
            {
                BinaryTree const* fa = now->parent();
                while(fa && fa->right() == now)
                {
                    now = fa;
                    fa = fa->parent();
                }
                now = fa;
            }
            return *this;
        }

        inorder_iterator operator++(int) //后置++
        {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const inorder_iterator& other) {return now == other.now;}
        bool operator!=(const inorder_iterator& other) {return now != other.now;}
    };

} // namespace student_std

#endif