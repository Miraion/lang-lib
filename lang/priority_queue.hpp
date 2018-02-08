//
// Created by Jeremy S on 2018-02-07.
//

#pragma once

#include "operand.hpp"
#include "heap.hpp"

__LANG_SUBSPACE

    template<typename T>
    struct priority_pair : comparable<priority_pair<T>> {
        T value;
        size_t priority;

        priority_pair(size_t priority, T const &value)
                : value(value), priority(priority)
        {}

        template<typename... Args>
        explicit priority_pair(size_t priority, Args const &... args)
                : value(std::forward<Args>(args)...), priority(priority)
        {}

        priority_pair(priority_pair<T> const &other)
                : value(other.value), priority(other.priority)
        {}

        bool operator==(priority_pair<T> const &p) const override
        { return priority == p.priority; }

        bool operator<(priority_pair<T> const &p) const override
        { return priority < p.priority; }

    };

    template<typename T>
    std::ostream &operator<<(std::ostream &os, priority_pair<T> const &p) {
        os << '{';
        os << p.priority << ", " << p.value;
        os << '}';
        return os;
    }

__LANG_SUBSPACE_END

__LANG_NAMESPACE

    template<typename T>
    class priority_queue {
    public:

        __LANG_OBJECT_TRAITS(T)
        typedef size_t priority_type;

        priority_queue() = default;
        priority_queue(priority_queue<T> const &other) = default;

        void insert(priority_type p, const_reference x);

        template<typename... Args>
        void emplace(priority_type p, Args const &... args);

        value_type extract() { return __heap_.extract().value; }

        size_t size() const { return __heap_.size(); }

        bool empty() const { return __heap_.empty(); }

        heap<__1::priority_pair<T>, less> &raw() { return __heap_; }
        heap<__1::priority_pair<T>, less> const &raw() const { return __heap_; }

    private:
        heap<__1::priority_pair<T>, less> __heap_{};
    };


    template<typename T>
    inline
    void
    priority_queue<T>::insert(priority_type p, const_reference x)
    { __heap_.insert(p, x); }

    template<typename T>
    template<typename... Args>
    inline
    void
    priority_queue<T>::emplace(priority_type p, Args const &...args)
    { __heap_.insert(p, std::forward<Args>(args)...); }

__LANG_NAMESPACE_END
