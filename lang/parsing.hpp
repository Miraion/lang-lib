//
// Created by Jeremy S on 2018-01-08.
//

#pragma once

#include "foundation/__base.hpp"
#include "misc.hpp"
#include "list.hpp"
#include "queue.hpp"

__LANG_NAMESPACE

    /*
     * Removes trailing entities of a set up to the first
     * entity that is not part of a given parameter pack.
     *
     * SetType must have public methods:
     *      back(), pop_back(), empty()
     *
     * Example:
     *      lang::array<int> arr{0,2,5,6,4,8};
     *      lang::remove_trailing(arr, 0, 2, 4, 6, 8); // remove trailing even numbers
     *      lang::println(arr); // prints [1, 2, 5]
     */
    template<typename SetType, typename... ExcludeSet>
    void remove_trailing(SetType &set, ExcludeSet... e);

    /*
     * Removes leading entities of a set up to the first
     * entity that is not part of a given parameter pack.
     *
     * SetType must have public methods:
     *      front(), pop_front(), empty()
     */
    template<typename SetType, typename... ExcludeSet>
    void remove_leading(SetType &set, ExcludeSet... e);

    /*
     * Specialization of above for strings.
     */
    template<typename... ExcludeSet>
    void remove_leading_str(std::string &str, ExcludeSet... e);

    /*
     * Scans a given set and removes all entities that
     * match an element in a given parameter pack.
     */
    template<typename SetType, typename... ExcludeSet>
    void remove_all(SetType &set, ExcludeSet... e);


    void remove_trailing_ws(std::string &str);
    void remove_leading_ws(std::string &str);
    void remove_ws(std::string &str);

    lang::_list<std::string> breakup_space_separated_string(std::string const &s);
    std::string to_space_separated_string(lang::_list<std::string> const &a);

    lang::_list<char> to_char_array(std::string const &str);
    std::string as_string(lang::_list<char> const &array);

__LANG_NAMESPACE_END


/* Implementation */
__LANG_NAMESPACE

    template<typename SetType, typename... ExcludeSet>
    inline void remove_trailing(SetType &set, ExcludeSet... e) {
        while (is_one_of(set.back(), e...) && !set.empty()) {
            set.pop_back();
        }
    }

    template<typename SetType, typename... ExcludeSet>
    inline void remove_leading(SetType &set, ExcludeSet... e) {
        while (is_one_of(set.front(), e...) && !set.empty()) {
            set.pop_front();
        }
    }

    template<typename... ExcludeSet>
    inline void remove_leading_str(std::string &set, ExcludeSet... e) {
        lang::_list<char> as_array{};
        for (char c : set)
            as_array.push_back(c);
        remove_leading(as_array, e...);
        set = "";
        for (char c : as_array)
            set.push_back(c);
    }

    template<typename SetType, typename... ExcludeSet>
    inline void remove_all(SetType &set, ExcludeSet... e) {
        SetType new_set{};
        for (auto elem : set) {
            if (!is_one_of(elem, e...))
                new_set.push_back(elem);
        }
        set = new_set;
    }

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "src/parsing.tcc"
#endif
