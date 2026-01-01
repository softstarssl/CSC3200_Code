#ifndef STUDENT_AVL_TREE_H
#define STUDENT_AVL_TREE_H


#include <cstddef>
#include <functional>
#include <algorithm>
#include <iterator>

namespace student_std
{
    template <typename Key, typename Comp = std::less<Key>>
    class avl_tree
    {
        class avl_node
        {
        public:
            using size_type = std::size_t;
            Key const& value() const { return m_key; }
            avl_node const* left() const { return m_left; }
            avl_node const* right() const { return m_right; }
            avl_node const* parent() const { return m_parent; }
            size_type height() const { return m_height; }
            size_type size() const { return m_size; }

        private:
            friend class avl_tree;

            Key m_key;
            avl_node* m_left;
            avl_node* m_right;
            avl_node* m_parent;
            std::ptrdiff_t m_height;
            size_type m_size;

            avl_node(Key const& key,avl_node* p = nullptr)
                : m_key(key), m_left(nullptr), m_right(nullptr), m_parent(p), m_height(0), m_size(1){}
        };

        class iterator
        {
        public:
            using value_type = avl_node;
            using reference = value_type const&;
            using pointer = value_type const*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator(pointer node = nullptr) : m_node(node) ,final_root(nullptr){}

            iterator& operator++() //(++it)
            {
                if(!m_node) return *this;

                if (m_node->right())
                {
                    m_node = m_node->right();
                    while (m_node->left()) m_node = m_node->left();
                }
                else
                {
                    pointer parent = m_node->parent();
                    while (parent && m_node == parent->right())
                    {
                        m_node = parent;
                        parent = parent->parent();
                    }
                    m_node = parent;
                }
                return *this;
            }

            iterator operator++(int) //(it++)
            {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            iterator& operator--() //(--it)
            {
                if(!m_node) return *this;

                if (m_node->left())
                {
                    m_node = m_node->left();
                    while (m_node->right()) m_node = m_node->right();
                }
                else
                {
                    pointer parent = m_node->parent();
                    while (parent && m_node == parent->left())
                    {
                        m_node = parent;
                        parent = parent->parent();
                    }
                    m_node = parent;
                }
                return *this;
            }

            iterator operator--(int) //(it--)
            {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }

            reference operator*() const { return *m_node; }
            pointer operator->() const { return m_node; }

        private:
            friend class avl_tree;
            pointer m_node;
            pointer final_root;

            iterator(pointer node, pointer root) 
                : m_node(node), final_root(root) {}

        };

    public:
        using key_type = Key;
        using node_type = avl_node;
        using size_type = std::size_t;
        using comparison = Comp;
        using const_iterator = iterator;
        using iterator = const_iterator;

        avl_tree() : m_root(nullptr), m_comp(Comp()) {}
        ~avl_tree() { clear(m_root); }

        iterator find(Key const& key) const { return Getiterator(find_node(key)); } //返回这个键值的迭代器
        bool contains(Key const& key) const { return find_node(key) != nullptr ;} //判断该键值是否存在
        size_type size() const //返回整棵树的大小（每个节点维护了其子树的大小，整棵树大小就是根节点的子树大小）
        {
            if(m_root) return m_root->m_size;
            return 0; 
        } 

        size_type height() const //返回整棵树的高度（每个节点维护了其子树的高度，整棵树大小就是根节点的高度）
        {
            if(m_root) return m_root->m_height;
            return 0; 
        }

        iterator begin() const { return Getiterator(leftmost(m_root)); } //寻找中序遍历的起点
        iterator end() const { return iterator(nullptr,m_root   ); }
        iterator root() const { return Getiterator(m_root); }

        iterator insert(Key const& key)
        {
            if(!m_root)
            {
                m_root = new node_type(key,nullptr);
                return Getiterator(m_root);
            }

            node_type* p = m_root;
            node_type* parent = nullptr;
            bool Left = false;

            while(p)
            {
                parent = p;
                if(m_comp(key,p->m_key))
                {
                    p = p->m_left;
                    Left = true;
                }
                else if(m_comp(p->m_key,key))
                {
                    p = p->m_right;
                    Left = false;
                }
                else return Getiterator(parent); //该键值已经存在，直接返回该节点的迭代器
            }

            node_type* new_node = new node_type(key,parent);
            //判断插入到父节点的左子树还是右子树
            if(Left) parent->m_left = new_node; 
            else parent->m_right = new_node;

            all_balance(parent);
            return Getiterator(new_node);
        }

        iterator erase(Key const& key)
        {
            node_type* n = find_node(key);
            if(!n) return end();

            if(!n->m_left || !n->m_right) //度为0或1,删掉之后可以由唯一一棵子树接上
            {
                node_type* child = n->m_left ? n->m_left : n->m_right;
                node_type* parent = n->m_parent;

                update_parent(parent,n,child);
                delete n;
                all_balance(parent);
            }
            else //度为2,用后继节点替代,后继节点是右子树中序遍历的起始点
            {
                node_type* r = n->m_right;
                r = leftmost(r);

                std::swap(n->m_key,r->m_key); //交换键值，避免移动外部Key对象
                //这个r节点必然不会存在左子树

                node_type* child = r->m_right;
                node_type* parent = r->m_parent;
                if(parent->m_left == r) parent->m_left = child; //从父节点摘掉后继节点 
                else parent->m_right = child;

                if(child) child->m_parent = parent;

                delete r;
                all_balance(parent);
            }

            return Getiterator(lower_bound(key)); //返回删除节点的后继节点
        }

    private:
        node_type* m_root;
        comparison m_comp;

        void clear(node_type* node)
        {
            if (node)
            {
                clear(node->m_left);
                clear(node->m_right);
                delete node;
            }
        }

        iterator Getiterator(node_type const* n) const { return iterator(n,m_root);}
        static node_type* leftmost(node_type* n) 
        {
            while(n && n->m_left) n = n->m_left;
            return n;
        }
        static node_type const* leftmost(node_type const* n)
        {
            while(n && n->left()) n = n->left();
            return n; 
        }

        size_type node_size(node_type const* n) {return n ? n->m_size : 0; }
        int node_height(node_type const* n) { return n ? n->m_height : -1; }
        void update(node_type* n)
        {
            if(!n) return ;
            n->m_size = node_size(n->m_left) + node_size(n ->m_right) + 1;
            int l = node_height(n->m_left),r = node_height(n->m_right);
            n->m_height = 1 + std::max(l,r);
        }

        int balance_factor(node_type const* n)
        {
            if(!n) return 0;
            return node_height(n->m_left) - node_height(n->m_right);
        }
        //左旋操作
        //x 右儿子是y,y的左儿子是z,，旋转后 y 的左儿子是x,x的右儿子是z
        node_type* zig(node_type* x)
        {
            node_type* y = x->m_right;
            node_type* z = y->m_left;

            y->m_parent = x->m_parent;
            if(x->m_parent)
            {
                if(x == x->m_parent->m_left) x->m_parent->m_left = y;
                else x->m_parent->m_right = y;
            }
            else m_root = y;

            y->m_left = x;
            x->m_parent = y;

            x->m_right = z;
            if(z) z->m_parent = x;

            update(x);
            update(y);
            return y;   
        }

        //右旋操作
        //y 左儿子是x,x的右儿子是z,旋转后 x 的右儿子是y,y的左儿子是z
        node_type* zag(node_type* y)
        {
            node_type* x = y->m_left;
            node_type* z = x->m_right;

            x->m_parent = y->m_parent;
            if(y->m_parent)
            {
                if(y == y->m_parent->m_left) y->m_parent->m_left = x;
                else y->m_parent->m_right = x;
            }
            else m_root = x;

            x->m_right = y;
            y->m_parent = x;

            y->m_left = z;
            if(z) z->m_parent = y;

            update(y);
            update(x);
            return x;   
        }

        void update_parent(node_type* parent, node_type* oldChild, node_type* newChild) //子节点改变之后更新父节点的信息
        {
            if (!parent)  m_root = newChild;// oldChild 是根：更新整棵树根
            else if (parent->m_left == oldChild) parent->m_left = newChild;     // oldChild 是父亲的左孩子
            else if (parent->m_right == oldChild) parent->m_right = newChild;    // oldChild 是父亲的右孩子

            if (newChild) newChild->m_parent = parent; // 维护 newChild 的父指针
        }

        node_type* keep_balance(node_type* n) //保持该点的平衡
        {
            if(!n) return n;
            update(n);
            int fac = balance_factor(n);

            if(fac > 1) //左重
            {
                if(balance_factor(n->m_left) < 0) //LR型,左重右高
                {
                    node_type* new_left = zig(n->m_left);
                    n->m_left = new_left;
                    new_left->m_parent = n;
                    update(n);
                }
                node_type* tmp = n->m_parent;
                node_type* new_root = zag(n);
                update_parent(tmp,n,new_root);
                return new_root;    
            }
            if(fac < -1) //右重
            {
                if(balance_factor(n->m_right) > 0) //RL型,右重左高
                {
                    node_type* new_right = zag(n->m_right);
                    n->m_right = new_right;
                    new_right->m_parent = n;
                    update(n);
                }
                node_type* tmp = n->m_parent;
                node_type* new_root = zig(n);
                update_parent(tmp,n,new_root);
                return new_root;
            }
            return n;
        }

        void all_balance(node_type* x) //保持从节点x到根节点的平衡
        {
            node_type* now = x;
            while(now)
            {
                node_type* new_now = keep_balance(now);
                now = new_now->m_parent;
            }
        }

        node_type* find_node(Key const& key) const //类似BST的查找键值
        {
            node_type* cur = m_root;
            while(cur)
            {
                if(m_comp(key,cur->m_key)) cur = cur->m_left; //在左子树查找
                else if(m_comp(cur->m_key,key)) cur = cur->m_right; //在右子树查找
                else return cur;
            }
            return nullptr; //没找到
        }

        node_type* lower_bound(Key const& key) const //在BST中二分查找第一个不小于key的节点
        {
            node_type* cur = m_root;
            node_type* ans = nullptr;
            while(cur)
            {
                if(!m_comp(cur->m_key,key)) //cur->key >= key
                {
                    ans = cur;
                    cur = cur->m_left;
                }
                else cur = cur->m_right;
            }
            return ans;
        }
 
    };
}
#endif