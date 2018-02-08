//
// Created by Jeremy S on 2018-02-04.
// Inspired by this blog post: http://madebyevan.com/obscure-cpp-features/?viksra

#pragma once

#include <cstdio>       // printf
#include <string>       // std::string
#include <sstream>      // std::ostringstream
#include <iostream>     // std::cout
#include "int_seq.hpp"
//#include "int_seq.hpp"  // std::tuple, lang::expand_into

/**
 * Definition of 'print' keyword.
 *
 * Creates a _print rvalue and invokes its operator, method
 * with what ever value comes after the keyword.
 *
 * e.g.
 *      print 42;  <- output: 42
 *
 *      print "hello", "world";  <- output: hello world
 */
#define print lang::__1::_print{},

namespace lang {
    namespace __1 {

        /**
         * Wrapper object to hold arguments for formatted output.
         */
        template<typename... Ts>
        struct format_args {
            std::tuple<Ts...> tuple;

            explicit format_args(Ts const &... ts) : tuple(std::make_tuple(ts...)) {}
        };

        /**
         * C++ style wrapper function for printf.
         * @param str : string to format
         * @param args: format arguments
         */
        template<typename... Ts>
        inline void _printf(std::string str, Ts... args) {
            printf(str.c_str(), args...);
        }

        /**
         * Core print object.
         *
         * Prints a series of elements to stdout separated by spaces with a
         * newline character appended at the end.
         * Works similarly to cout but uses operator, instead of operator<<.
         *
         * Usage:
         *      print e1, e2, ..., en, <optional> format(a1, a2, ..., an);
         *
         * Using the comma operator, and unbounded number of elements may be
         * collected to be printed to the standard output. Elements are stored
         * in a std::ostringstream object and printed using std::cout once
         * the _print object is destroyed (which happens as soon as the line
         * is finished executing as it should be an rvalue). The format function
         * may be used as the last element in the comma-separated list to apply
         * printf-style formatting to the string whilst printing.
         */
        class _print {
        public:

            _print() = default;
            ~_print();

            template<typename T>
            _print &operator,(T const &t);

            template<typename... Ts>
            void operator,(format_args<Ts...> const &args);

        private:
            bool space = false;
            bool format_mode = false;
            std::stringstream buffer;
        };


        inline _print::~_print() {
            // If format_mode is true, then the overload of the comma operator
            // will handle printing of the string using printf. Otherwise,
            // printing will occur using std::cout upon object's destruction.
            if (!format_mode) {
                std::cout << buffer.str();
                std::cout << std::endl;
            } else {
                std::cout << std::endl;
            }
        }

        template<typename T>
        inline _print &_print::operator,(T const &t) {
            if (space)
                buffer << ' ';
            else
                space = true;
            buffer << t;
            return *this;
        }

        template<typename... Ts>
        inline void _print::operator,(const format_args<Ts...> &args) {
            format_mode = true;
            std::tuple<std::string, Ts...> t = std::tuple_cat(std::make_tuple(buffer.str()), args.tuple);
            lang::expand_into<void, std::string, Ts...>(_printf, t);
        }

    }
}

/**
 * Function to create a format package for print.
 * Must be the last element that is sent to print.
 *
 * e.g.
 *      print "A point:", "(%d, %d)", format(1, 2);  <- output: A point: (1, 2)
 *
 * Bug: Formatting %s with a string literal is currently not working as expected.
 *      It is possible to work around this issue by casting the string literal to
 *      char* or using std::string and the c_str method.
 *
 *      e.g.
 *          print "%s", format("hello world);  <- will not compile
 *          print "%s", format((char *) "hello world");  <- works fine
 */
template<typename... Ts>
lang::__1::format_args<Ts...> format(Ts const &... args) {
    return lang::__1::format_args<Ts...>(args...);
}
