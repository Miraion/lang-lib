//
// Created by Jeremy S on 2018-01-08.
//

#pragma once

#include "../parsing.hpp"
#include "../foundation/__conditional_keyword.hpp"

__LANG_NAMESPACE

    con_inline void remove_trailing_ws(std::string &str) {
        remove_trailing(str, ' ', '\n', '\t', '\r');
    }

    con_inline void remove_leading_ws(std::string &str) {
        remove_leading_str(str, ' ', '\n', '\t', '\r');
    }

    con_inline void remove_ws(std::string &str) {
        remove_all(str, ' ', '\n', '\t', '\r');
    }

    con_inline lang::_list<std::string> breakup_space_separated_string(std::string const &s) {
        lang::_list<std::string> array{};
        std::string str_builder = "";
        for (char c : s) {
            if (c == ' ') {
                array.push_back(str_builder);
                str_builder = "";
            } else {
                str_builder += c;
            }
        }

        if (!str_builder.empty())
            array.push_back(str_builder);
        return array;
    }

    con_inline std::string to_space_separated_string(lang::_list<std::string> const &a) {
        std::string s = "";
        for (auto it = a.begin(); it != a.end(); ++it) {
            s += *it;
            if (it != a.end() - 1)
                s += " ";
        }
        return s;
    }

    con_inline lang::_list<char> to_char_array(std::string const &str) {
        lang::_list<char> a{};
        for (char c : str) {
            a.push_back(c);
        }
        return a;
    }

    con_inline std::string as_string(lang::_list<char> const &array) {
        std::string s{};
        for (char c : array) {
            s.push_back(c);
        }
        return s;
    }

__LANG_NAMESPACE_END
