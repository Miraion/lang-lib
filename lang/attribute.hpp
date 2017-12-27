//
// Created by Jeremy S on 2017-12-06.
//

#pragma once

/*

    attribute.hpp synopsis

namespace lang
{

template<size_t N> // N must be <= 64
struct attribute {

    typedef type; // is either (unsigned) long, int, short, or char depending on N
    type raw_value;

    attribute();
    attribute(attribute<N>const &);
    template<size_t M>
    explicit attribute(attribute<M> const &);
    virtual ~attribute();

    void set(type);
    void unset(type);
    bool get(type);

};

template<size_t I, typename T = typename attribute<I>::type>
constexpr inline T attr_index();

template<size_t N> // N must be <= 64
class attribute_t {
public:

    typedef typename attribute<N>::type attr_type;

    attribute_t();
    virtual ~attribute_t();

    void attr_set(attr_type);
    void attr_unset(attr_type);
    bool attr_get(attr_type);
};

}

*/

#include "foundation/__base.hpp"

__LANG_NAMESPACE

    // _N may not be > 64
    template<size_t _N>
    struct attribute {

        typedef typename std::conditional<(_N > 32),
            unsigned long,
            typename std::conditional<(_N > 16),
                    unsigned int,
                    typename std::conditional<(_N > 8),
                            unsigned short,
                            unsigned char>::type>::type>::type type;

        type raw_value = 0;

        inline attribute() = default;

        inline attribute(attribute<_N> const &__a) = default;

        template<size_t _M>
        explicit attribute(attribute<_M> const &__a);

        void set (type __i);

        void unset (type __i);

        bool get (type __i);

        void null();

    };

    template<size_t _N>
    template<size_t _M>
    inline
    attribute<_N>::attribute(attribute<_M> const &__a) {
        if (_M > _N)
            throw std::runtime_error("lang::attribute: bad cast, reference attribute too large");
        raw_value = __a.raw_value;
    }

    template<size_t _N>
    inline
    void
    attribute<_N>::set(type __i)
    {
        raw_value = raw_value | __i;
    }

    template<size_t _N>
    inline
    void
    attribute<_N>::unset(type __i)
    {
        raw_value = raw_value & (~__i);
    }

    template<size_t _N>
    inline
    bool
    attribute<_N>::get(type __i)
    {
        return (raw_value & __i) != 0;
    }

    template<size_t _N>
    inline
    void
    attribute<_N>::null()
    {
        raw_value = 0;
    }


    template<size_t _I, typename _T = typename attribute<_I>::type>
    constexpr _T attr_index() noexcept {
        return (_T)lang::pow(2, _I);
    }

    // Wrapper object for lang::attribute with renamed methods.
    // To be inherited by other objects.
    template<size_t _N>
    class attribute_t : private attribute<_N> {
    private:

        typedef attribute<_N>               __attr_base;

    public:

        typedef typename __attr_base::type  attr_type;

        inline attribute_t() = default;

        inline void attr_set(attr_type __i) { this->set(__i); }

        inline void attr_unset(attr_type __i) { this->unset(__i); }

        inline bool attr_get(attr_type __i) { return this->get(__i); }

        inline void attr_null() {this->null(); }

    };

__LANG_NAMESPACE_END
