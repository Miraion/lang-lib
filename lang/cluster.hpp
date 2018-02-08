//
// Created by Jeremy S on 2017-12-18.
//

#pragma once

#include <memory>
#include "foundation/__base.hpp"
#include "list.hpp"

__LANG_NAMESPACE

    template<class... Params>
    class cluster_element;

    template<class... Params>
    class cluster_core {
    private:

        typedef cluster_element<Params...> elem_t;

        _list<std::weak_ptr<elem_t>> elements{};

    public:

        cluster_core() = default;

        void add_element(std::shared_ptr<elem_t> e);

        void call(Params&&... params);

    };

    template<class... Params>
    inline
    void
    cluster_core<Params...>::add_element(std::shared_ptr<elem_t> e)
    {
        elements.emplace_back(e);
    }

    template<class... Params>
    inline
    void
    cluster_core<Params...>::call(Params&&... params)
    {
        _list<size_t> invalid_indices{};
        loop until(elements.size()) {
            if (auto e = elements[i].lock()) {
                e->__on_call(params...);
            } else {
                invalid_indices.push_back(i);
            }
        }

        // remove elements in reverse order as to not change indices
        for (size_t idx : invalid_indices.reverse()) {
            elements.pop_at(idx);
        }
    }


    template<class... Params>
    class cluster_element {
    private:

        typedef cluster_element<Params...> self_t;

        // internal reference to this with null deleter
        // will ensure that there is at least one shared
        // reference for the duration of this object's
        // life span
        std::shared_ptr<self_t> self;

    public:

        typedef self_t cluster_type;

        explicit cluster_element(cluster_core<Params...> &parent_core);

        virtual void __on_call(Params...) = 0;

    };

    template<class... Params>
    inline
    cluster_element<Params...>::cluster_element(cluster_core<Params...> &parent_core)
            : self(lang::make_quiet_ptr(this))
    {
        parent_core.add_element(self);
    }

__LANG_NAMESPACE_END
