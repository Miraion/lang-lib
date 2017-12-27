//
// Created by Jeremy S on 2017-11-24.
//

#pragma once

#include <cmath>
#include "foundation/__base.hpp"
#include "point.hpp"

namespace lang {

    template<size_t __n>
    using vector = point<__n, double>;

    template<size_t __n>
    inline double dot (vector<__n> const &a, vector<__n> const &b) {
        double d = 0;
        loop until(__n)
            d += (a[i] * b[i]);
        return d;
    }

    template<size_t __n>
    inline double magnitude (vector<__n> const &v) {
        double m = 0;
        loop until(__n) {
            m += v[i] * v[i];
        }
        return sqrt(m);
    }

    template<size_t __n>
    inline double distance (vector<__n> const &base, vector<__n> const &ref) {
        return magnitude(base - ref);
    }

    inline vector<3> cross (vector<3> const &v, vector<3> const &u) {
        double i = (v[1] * u[2]) - (v[2] * u[1]);
        double j = (v[0] * u[2]) - (v[2] * u[0]);
        double k = (v[0] * u[1]) - (v[1] * u[0]);
        return vector<3>{i, j, k};
    }

}
