#ifndef STUDENT_PRIORITY_QUEUE_H
#define STUDENT_PRIORITY_QUEUE_H

#include <list>
#include <cstddef>

namespace student_std
{
    template<typename T,typename Container = std::list<T>>
    class priority_queue
    {
    private: 
        Container q;

    public:
        using container_type = Container;
        using value_type  = typename Container :: value_type;
        using size_type = typename Container :: size_type;

        priority_queue() = default; //默认构造和赋值
        priority_queue(priority_queue const&) = default;
        priority_queue& operator=(priority_queue const&) = default;

        bool empty() const { return q.empty(); }
        size_type size() const { return q.size(); }
        value_type const& top() const { return q.front(); }

        void pop() { q.erase(q.begin()); }
        void push(const value_type& x)
        {
            auto it = q.begin();
            for(;it != q.end();++it)
            {
                if(*it < x) break ;
            }
            q.insert(it,x);
        }
        void swap(priority_queue& others) { q.swap(others.q); }
    };
}

#endif
