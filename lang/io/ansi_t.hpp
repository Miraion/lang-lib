//
// Created by Jeremy S on 2017-11-25.
//

#pragma once

#include <string>
#include <ostream>

namespace lang {

    class ansi_t {
    private:

        short __val_;

    public:

        explicit ansi_t (short v) noexcept
                : __val_(v)
        {}

        operator std::string () const {
            return as_string();
        }

        std::string as_string () const {
            return "\x1B[" + std::to_string(__val_) + "m";
        }

        unsigned int value () const {
            return static_cast<unsigned int>(__val_);
        }

    };

    inline std::ostream &operator<< (std::ostream &os, ansi_t const &a) {
        os << a.as_string();
        return os;
    }
}

struct ansi {
    static const lang::ansi_t normal        {0};

    static const lang::ansi_t black         {30};
    static const lang::ansi_t red           {31};
    static const lang::ansi_t green         {32};
    static const lang::ansi_t yellow        {33};
    static const lang::ansi_t blue          {34};
    static const lang::ansi_t magenta       {35};
    static const lang::ansi_t cyan          {36};
    static const lang::ansi_t white         {37};

    static const lang::ansi_t black_bg      {40};
    static const lang::ansi_t red_bg        {41};
    static const lang::ansi_t green_bg      {42};
    static const lang::ansi_t yellow_bg     {43};
    static const lang::ansi_t blue_bg       {44};
    static const lang::ansi_t magenta_bg    {45};
    static const lang::ansi_t cyan_bg       {46};
    static const lang::ansi_t white_bg      {47};
};
