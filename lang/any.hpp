//
// Created by Jeremy S on 2017-11-21.
//

#pragma once

#include <cstdlib>
#include <memory>
#include <exception>

namespace lang {

    class any {
    private:

        std::shared_ptr<void> __ptr_;
        size_t __size_;

        template<class T>
        friend bool is (any const &);

        template<class T>
        friend T as (any const &);

    public:

        any ();

        any (any const &other) = default;

        template<typename T>
        any (T const &x);

        template<typename T>
        any &operator= (T const &x);

        virtual ~any () = default;

        size_t size () const;

        void *unsafe_raw ();

    }; // class any

    inline any::any()
            : __ptr_(nullptr),
              __size_(0)
    {}

    template<typename T>
    inline any::any(const T &x)
            : __ptr_(new T{x}),
              __size_(sizeof(T))
    {}

    inline size_t any::size() const {
        return __size_;
    }

    inline void *any::unsafe_raw() {
        return __ptr_.get();
    }

    template<typename T>
    inline any &any::operator=(const T &x) {
        this->__ptr_ = std::shared_ptr<void>{new T{x}};
        this->__size_ = sizeof(T);
        return *this;
    }


    template <class T>
    inline bool is(any const &a) {
        return a.__size_ == sizeof(T);
    }

    template <class T>
    inline T as(any const &a) {
        if (!is<T>(a))
            throw std::bad_cast{};
        return *(T *)a.__ptr_.get();
    }

} // namespace lang
