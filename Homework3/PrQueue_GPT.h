#ifndef STUDENT_PRIORITY_QUEUE_H
#define STUDENT_PRIORITY_QUEUE_H

#include <list>
#include <utility> // std::swap（如需）
#include <cstddef>

namespace student_std {

// 一个简单的最大优先队列适配器：维护“最大元素在前端”的不变式。
// - Container 默认为 std::list<T>。
// - 仅使用 front()/pop_front()/size()/empty()/begin()/end()/insert()/swap(Container&)
//   这些操作，从而满足题目对可替换容器的约束。
// - top() O(1)，pop() O(1)（以 list 为底层）；push() O(n) 通过线性插入维护降序。
template <typename T, typename Container = std::list<T>>
class priority_queue {
public:
    using container_type = Container;
    using value_type     = typename Container::value_type;
    using size_type      = typename Container::size_type;

    // 构造/赋值
    priority_queue() = default;
    priority_queue(priority_queue const&) = default;
    priority_queue& operator=(priority_queue const&) = default;

    // 容量
    bool empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }

    // 访问
    // 要求：队列非空
    value_type const& top() const { return c_.front(); }

    // 修改器
    // 维持不变式：c_ 按非增序（降序）存储，即
    // 对任意相邻元素 a(在前), b(在后) 都满足 !(a < b)。
    void push(value_type const& value) {
        auto it = c_.begin();
        // 找到第一个 *it < value 的位置，在其前面插入（使得 value 位于更“靠前”的大元素一侧）
        for (; it != c_.end(); ++it) {
            if (*it < value) {
                break;
            }
        }
        c_.insert(it, value);
    }

    // 弹出最大元素（位于前端）
    // 要求：队列非空
    void pop() {
        c_.pop_front();
    }

    // 交换内容（不拷贝元素）
    void swap(priority_queue& other) noexcept(noexcept(std::declval<Container&>().swap(std::declval<Container&>()))) {
        c_.swap(other.c_);
    }

private:
    Container c_;
};

} // namespace student_std

#endif // STUDENT_PRIORITY_QUEUE_H