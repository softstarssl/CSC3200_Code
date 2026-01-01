#ifndef STUDENT_UNORDERED_MAP_H
#define STUDENT_UNORDERED_MAP_H

#include <functional>  // std::hash
#include <vector>      // std::vector
#include <list>        // std::list
#include <utility>     // std::pair
#include <cstddef>     // std::size_t, std::ptrdiff_t

namespace student_std
{
    template<typename Key,typename T,typename Hash = std::hash<Key>>
    class unordered_map
    {
    public:
        using key_type = Key;
        using mapped_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<Key,T>;
        using hasher = Hash;
        using reference = value_type&;
        using const_reference = const value_type&;

    private:
        using bucket_type = std::list<value_type>;
        
        std::vector<bucket_type> buckets;
        size_type siz = 0;
        hasher hashit = hasher();

        size_type bucket_id(Key const &key)
        {
            size_type cnt = buckets.size();
            return (size_type)(hashit(key) % cnt);
        }

        size_type bucket_id(Key const &key,size_type cnt)
        {
            // size_type cnt = buckets.size();
            return (size_type)(hashit(key) % cnt);
        }

        void rehash(size_type newcnt)
        {
            if(newcnt <= 0) newcnt = 1;

            std::vector<bucket_type> new_buckets;
            new_buckets.resize(newcnt);

            for(auto& bucket : buckets)
            {
                for(auto& val : bucket)
                {
                    size_type idx = bucket_id(val.first,newcnt);
                    new_buckets[idx].emplace_back(val);
                }
            }

            buckets.swap(new_buckets);
            // siz 并没有改变因为没有插入新的元素
        }

        void need_rehash()
        {
            if(siz >= 2 * buckets.size()) //负载因子 > 2
            {
                rehash(2 * buckets.size());
            }
        }

    public:
        unordered_map() : buckets(4),siz(0),hashit(hasher()) {}
        explicit unordered_map(size_type cnt,hasher const& h = hasher()):
        buckets(cnt <= 0 ? 1 : cnt),siz(0),hashit(h) {}

        unordered_map(unordered_map const&)            = default;
        unordered_map& operator=(unordered_map const&) = default;
        unordered_map(unordered_map&&) noexcept        = default;
        unordered_map& operator=(unordered_map&&) noexcept = default;
        ~unordered_map() = default;

        size_type size() const {return siz;}
        bool empty() const {return siz == 0;}
        size_type bucket_count() const {return buckets.size();}
        
        bool contains(Key const &key) const
        {
            size_type idx = bucket_id(key);
            for(auto &[k,t] : buckets[idx])
            {
                if(k == key) return true;
            }
            return false;
        }

        T& operator [](Key const& key)
        {
            size_type idx = bucket_id(key);
            for(auto &[k,t] : buckets[idx])
            {
                if(k == key) return t;
            }

            buckets[idx].push_back({key,T{}});  //否则建立新的默认对应
            ++siz; 
            need_rehash(); //判断是否要进行重hash

            idx = bucket_id(key); //重新定位在哪个桶里面
            for(auto &[k,t] : buckets[idx])
            {
                if(k == key) return t;
            }
            
            return buckets[idx].begin()->second;
        }

        size_type erase(Key const& key) //删除成功返回1,否则返回0
        {
            size_type idx = bucket_id(key);
            for(auto it = buckets[idx].begin();it != buckets[idx].end();++it)
            {
                if((*it).first == key)
                {
                    --siz;
                    buckets[idx].erase(it);
                    return (size_type)1;
                }
            }
            return (size_type)0;
        }

        void clear()
        {
            for(auto& bucket : buckets) bucket.clear();
            siz = 0;
        }
    };
}
#endif