#ifndef STUDENT_VECTOR_H
#define STUDENT_VECTOR_H

#include<cstddef>
#include<cassert>
#include<stdexcept>
#include<iterator>

namespace student_std
{
    template<typename global>
    inline void swap(global& a,global& b) {global tmp = a; a = b; b = tmp;}
    
    template<typename T>
    class vector
    {
    public:
        using size_type = std::size_t; //size
        using difference_type = std::ptrdiff_t; //迭代器差值
        using value_type = T; // type

        class iterator
        {

        public:
            using difference_type = std::ptrdiff_t; //迭代器差值
            using value_type = T; // type
            using pointer = T*; //pointer type
            using reference = T&; //引用类型
            using iterator_category = std::random_access_iterator_tag;
            // 迭代器范畴：随机访问
        private: pointer p;

        public:
            iterator() : p(nullptr) {}
            explicit iterator(T* pt) : p(pt) {}// 从 T* 构造：允许从裸指针创建迭代器
            iterator(const iterator&) = default;       // 拷贝构造：默认语义即可
            iterator& operator=(const iterator&) = default; // 赋值：默认语义即可

            reference operator* () const {return *p;} //返回值
            pointer operator-> () const {return p;} //返回指针

            iterator& operator++() {++p; return *this;}
            iterator operator++(int) {iterator it(*this); ++(*this); return it;}
            iterator& operator--() {--p; return *this;}
            iterator operator--(int) {iterator it(*this); --(*this);return it;}

            iterator& operator+=(difference_type n) {p += n;return *this;}
            iterator operator+(difference_type n) {return iterator(p + n);}
            iterator& operator-=(difference_type n) {p -= n;return *this;}
            iterator operator-(difference_type n) {return iterator(p - n);}
            difference_type operator-(const iterator& other) {return p - other.p;}
            
            bool operator<(const iterator& other) const {return p < other.p;}
            bool operator>(const iterator& other) const {return p > other.p;}
            bool operator<=(const iterator& other) const {return p <= other.p;}
            bool operator>=(const iterator& other) const {return p >= other.p;}
            bool operator==(const iterator& other) const {return p == other.p;}
            bool operator!=(const iterator& other) const {return p != other.p;}
        };


        vector() : data_(nullptr),siz(0),space(0) {}
        //当前底层指针,vec有效元素个数，最大内存上限
        vector(const vector& other) : data_(nullptr),siz(0),space(0)
        {
            if(other.siz)
            {
                data_ = new T [other.siz];
                for(size_type i = 0;i < other.siz;++i) data_[i] = other.data_[i];
                siz = other.siz;
                space = other.space;
            }
        }

        vector& operator=(const vector& other)
        {
            if(this != &other)
            {
                vector tmp(other);
                swap(tmp); //swap 函数在后面
            }
            return *this;
        }

        ~vector() {delete[] data_;}

        size_type size() const {return siz;}
        size_type capacity() const {return space;}
        bool empty() const {return siz == 0;}
        const T* data() const {return data_;}
        const T& at(size_type i) const 
        {
            // size_t 类型不会有负数，故无需判断是否 < 0
            if(i >= siz) throw std::out_of_range("out of range");
            return data_[i];
        }
        const T& operator[](size_type i) const {return data_[i];}
        const T& front() const {return data_[0];}
        const T& back() const {return data_[siz - 1];}

        void push_back(const T& val)
        {
            check_space(1);
            data_[siz++] = val;
        }
        void pop_back() {--siz;}
        void reserve(size_type new_space) //新分配内存
        {
            if(new_space <= space) return ; //无需新的分配
            T* newdata = new T [new_space];
            for(size_type i = 0;i < siz;++i) newdata[i] = data_[i];
            delete[] data_; //删除保证优秀的空间管理
            data_ = newdata;
            space = new_space;
        }

        //resize 接口1：给resize后的数组全部赋值T的默认值
        void resize(size_type newsize)
        {
            if(newsize <= siz) {siz = newsize; return ;}
            reserve(newsize); //分配新的内存
            for(size_type i = siz;i < newsize;++i) data_[i] = T{};
            siz = newsize;
        }
        void resize(size_type newsize,const T& val)
        {
            if(newsize <= siz) {siz = newsize; return ;}
            reserve(newsize); //分配新的内存
            for(size_type i = siz;i < newsize;++i) data_[i] = val;
            siz = newsize;
        }

        void swap(vector& other)
        {
            T* tmp = data_; data_ = other.data_, other.data_ = data_;
            size_type tmpsize = siz ; siz = other.siz, other.siz = tmpsize;
            size_type tmpspace = space; space = other.space, other.space = tmpspace; 
            ;
        }

        void clear() {siz = 0;}
        T* data() {return data_;}
        T& at(size_type i)
        {
            if(i >= siz) throw std::out_of_range("out of range");
            return data_[i];
        }
        T& operator[](size_type i) {return data_[i];}
        T& front() {return data_[0];}
        T& back() {return data_[siz - 1];}

        iterator begin() {return iterator(data_);}
        iterator end() {return iterator(data_ + siz);}

        iterator insert(iterator it,const T& val)
        {
            size_type idx = (size_type)(it - begin());
            // assert(idx <= siz);
            check_space(1);
            for(size_type i = siz;i > idx;--i) data_[i] = data_[i - 1];
            data_[idx] = val;
            ++siz;
            return iterator(data_ + idx);
        }

        iterator erase(iterator it)
        {
            size_type idx = (size_type)(it - begin());
            // assert(idx < siz);
            for(size_type i = idx;i + 1 < siz;++i) data_[i] = data_[i + 1];
            --siz;
            return iterator(data_ + idx);
        }
        iterator erase(iterator it1,iterator it2)
        {
            size_type idx1 = (size_type)(it1 - begin());
            size_type idx2 = (size_type)(it2 - begin());
            // assert(idx1 <= idx2 && idx2 <= siz);
            size_type len = idx2 - idx1;

            for(size_type i = idx1;i + len < siz;++i) data_[i] = data_[i + len];
            siz -= len;
            return iterator(data_ + idx1);
        }

    private:
        T* data_;
        size_type siz;
        size_type space;

        void check_space(size_type need)
        {
            if(siz + need <= space) return ;
            size_type newspace;
            if(space) newspace = space;
            else newspace = 1;

            while(newspace < siz + need) newspace <<= 1;
            reserve(newspace);
        }

    };
}
#endif 