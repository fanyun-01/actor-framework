/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011-2013                                                    *
 * Dominik Charousset <dominik.charousset@haw-hamburg.de>                     *
 *                                                                            *
 * This file is part of libcppa.                                              *
 * libcppa is free software: you can redistribute it and/or modify it under   *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation; either version 2.1 of the License,               *
 * or (at your option) any later version.                                     *
 *                                                                            *
 * libcppa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 * See the GNU Lesser General Public License for more details.                *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with libcppa. If not, see <http://www.gnu.org/licenses/>.            *
\******************************************************************************/


#ifndef CPPA_TYPED_ACTOR_HPP
#define CPPA_TYPED_ACTOR_HPP

#include "cppa/replies_to.hpp"
#include "cppa/intrusive_ptr.hpp"
#include "cppa/abstract_actor.hpp"
#include "cppa/typed_event_based_actor.hpp"

namespace cppa {

template<typename... Signatures>
class typed_actor {

 public:

    typedef typed_event_based_actor<Signatures...> actor_type;

    typedef util::type_list<Signatures...> signatures;

    typed_actor() = default;
    typed_actor(typed_actor&&) = default;
    typed_actor(const typed_actor&) = default;
    typed_actor& operator=(typed_actor&&) = default;
    typed_actor& operator=(const typed_actor&) = default;

    template<typename... OtherSignatures>
    typed_actor(const typed_actor<OtherSignatures...>& other) {
        set(std::move(other));
    }

    template<typename... OtherSignatures>
    typed_actor& operator=(const typed_actor<OtherSignatures...>& other) {
        set(std::move(other));
        return *this;
    }

    template<template<typename...> class Impl, typename... OtherSignatures>
    typed_actor(intrusive_ptr<Impl<OtherSignatures...>> other) {
        set(other);
    }

 private:

    template<class ListA, class ListB>
    inline void check_signatures() {
        static_assert(util::tl_is_strict_subset<ListA, ListB>::value,
                      "'this' must be a strict subset of 'other'");
    }

    template<typename... OtherSignatures>
    inline void set(const typed_actor<OtherSignatures...>& other) {
        check_signatures<signatures, util::type_list<OtherSignatures...>>();
        m_ptr = other.m_ptr;
    }

    template<template<typename...> class Impl, typename... OtherSignatures>
    inline void set(intrusive_ptr<Impl<OtherSignatures...>>& other) {
        check_signatures<signatures, util::type_list<OtherSignatures...>>();
        m_ptr = std::move(other);
    }

    abstract_actor_ptr m_ptr;

};

} // namespace cppa

#endif // CPPA_TYPED_ACTOR_HPP
