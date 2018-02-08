//
// Created by Jeremy S on 2018-01-11.
//

#pragma once

#include "foundation/__base.hpp"

__LANG_NAMESPACE

    template<class ComparisonType>
    struct equatable {

        virtual bool operator==(ComparisonType const &__a) const = 0;

        inline bool operator!=(ComparisonType const &__a) const
        { return !(*(this) == __a); }

    };


    /*
     * Fully defines <=, >=, > from < using standard logic rules.
     */
    template<class ComparisonType>
    struct comparable : public equatable<ComparisonType> {

        virtual bool operator<(ComparisonType const &__a) const = 0;

        inline bool operator<=(ComparisonType const &__a) const
        { return ((*this) < __a) || ((*this) == __a); }

        inline bool operator>=(ComparisonType const &__a) const
        { return !((*this) < __a); }

        inline bool operator>(ComparisonType const &__a) const
        { return ((*this) >= __a) && ((*this) != __a); }

    };

__LANG_NAMESPACE_END
