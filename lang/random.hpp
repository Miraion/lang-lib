//
// Created by Jeremy S on 2017-12-13.
//

#pragma once

/*

        random.hpp synopsis

 namespace lang
 {

 template<class Distribution,
          class Parameter = typename Distribution::result_type,
          class Generator = std::default_random_engine>
 class random {
 public:

     typedef Generator                                  generator_type;
     typedef Distribution                               distribution_type;
     typedef Parameter                                  param_type;
     typedef typename distribution_type::result_type    result_type;

     generator_type generator;
     distribution_type distribution;

     template<class... Params>
     explicit random(Params&&...);

     result_type operator()();

     void explicit_seed(unsigned int);

 private:

     std::random_device rd;

 }; // class random

 using uniform_random_t         = random<std::uniform_int_distribution<int>>;
 using uniform_real_random_t    = random<std::uniform_real_distribution<double>>;
 using normal_random_t          = random<std::normal_distribution<double>>;

 template<int L, int U>
 inline uniform_random_t uniform_random; // since c++17

 template<int L, int U>
 inline uniform_real_random_t uniform_real_random; // since c++17

 template<int M, int D>
 inline normal_random_t normal_random; // since c++17

 template<unsigned int P>
 inline random<std::bernoulli_distribution, double> boolean_random; // since c++17

 bool random_chance(int);

 } // namespace lang

 */

#include "foundation/__base.hpp"
#include <random>

__LANG_NAMESPACE

    template<class Dis, class Param = typename Dis::result_type, class Gen = std::default_random_engine>
    class random {
    public:

        typedef Gen                                     generator_type;
        typedef Dis                                     distribution_type;
        typedef Param                                   param_type;
        typedef typename distribution_type::result_type result_type;

        generator_type generator;
        distribution_type distribution;

        template<class... Params>
        explicit random(Params&&... params);

        result_type operator()();

        void explicit_seed(unsigned int seed);

    private:
        std::random_device rd;
    };

    template<class Dis, class Param, class Gen>
    template<class... Params>
    inline
    random<Dis, Param, Gen>::random(Params&&... params)
    {
        generator.seed(rd());
        distribution = distribution_type{static_cast<param_type>(params)...};
    }

    template<class Dis, class Param, class Gen>
    inline
    typename random<Dis, Param, Gen>::result_type
    random<Dis, Param, Gen>::operator()()
    {
        return distribution(generator);
    }

    template<class Dis, class Param, class Gen>
    inline
    void
    random<Dis, Param, Gen>::explicit_seed(unsigned int seed)
    {
        generator.seed(seed);
    }


    using uniform_random_t      = random<std::uniform_int_distribution<int>>;
    using uniform_real_random_t = random<std::uniform_real_distribution<double>>;
    using normal_random_t       = random<std::normal_distribution<double>>;


#ifdef __LANG_STD_17
    template<int L, int U>
    inline uniform_random_t uniform_random{L, U};

    template<int L, int U>
    inline uniform_real_random_t uniform_real_random{L, U};

    template<int M, int D>
    inline normal_random_t normal_random{M, D};

    template<unsigned int P>
    inline random<std::bernoulli_distribution, double> boolean_random{double(P) / 100.0};
#endif

#ifdef __LANG_STD_17
    inline bool random_chance(int percent) {
        return uniform_random<1, 100>() <= percent;
    }
#else
    inline bool random_chance(int percent) {
        uniform_random_t r{1, 100};
        return r() <= percent;
    }
#endif

__LANG_NAMESPACE_END
