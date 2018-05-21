// nanorange/algorithm/equal.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_ALGORITHM_EQUAL_HPP_INCLUDED
#define NANORANGE_ALGORITHM_EQUAL_HPP_INCLUDED

#include <nanorange/iterator/operations.hpp>
#include <nanorange/range.hpp>

NANO_BEGIN_NAMESPACE

namespace detail {

struct equal_fn {
private:
    template <typename I1, typename S1, typename I2, typename S2, typename Pred,
              typename Proj1, typename Proj2>
    static constexpr bool impl4(I1 first1, S1 last1, I2 first2, S2 last2,
                                Pred pred, Proj1 proj1, Proj2 proj2)
    {
        while (first1 != last1 && first2 != last2) {
            if (!nano::invoke(pred, nano::invoke(proj1, *first1),
                              nano::invoke(proj2, *first2))) {
                return false;
            }
            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 == last2;
    }

    template <typename I1, typename S1, typename I2, typename Pred,
              typename Proj1, typename Proj2>
    static constexpr bool impl3(I1 first1, S1 last1, I2 first2, Pred pred,
                                Proj1 proj1, Proj2 proj2)
    {
        while (first1 != last1) {
            if (!nano::invoke(pred, nano::invoke(proj1, *first1),
                              nano::invoke(proj2, *first2))) {
                return false;
            }
            ++first1;
            ++first2;
        }

        return true;
    }

public:
    // Four-legged, sized sentinels
    template <typename I1, typename S1, typename I2, typename S2,
              typename Pred = equal_to<>, typename Proj1 = identity,
              typename Proj2 = identity>
    constexpr std::enable_if_t<
        InputIterator<I1> && Sentinel<S1, I1> && InputIterator<I2> &&
            Sentinel<S2, I2> &&
            IndirectlyComparable<I1, I2, Pred, Proj1, Proj2> &&
            SizedSentinel<S1, I1> && SizedSentinel<S2, I2>,
        bool>
    operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        if (last1 - first1 != last2 - first2) {
            return false;
        }

        // Ranges are the same size, so call the 3-legged version
        // and save ourselves a comparison
        return equal_fn::impl3(std::move(first1), std::move(last1),
                               std::move(first2), std::move(pred),
                               std::move(proj1), std::move(proj2));
    }

    // Four-legged, unsized sentinels
    template <typename I1, typename S1, typename I2, typename S2,
              typename Pred = equal_to<>, typename Proj1 = identity,
              typename Proj2 = identity>
    constexpr std::enable_if_t<
        InputIterator<I1> && Sentinel<S1, I1> && InputIterator<I2> &&
            Sentinel<S2, I2> &&
            IndirectlyComparable<I1, I2, Pred, Proj1, Proj2> &&
            !(SizedSentinel<S1, I1> && SizedSentinel<S2, I2>),
        bool>
    operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return equal_fn::impl4(std::move(first1), std::move(last1),
                               std::move(first2), std::move(last2),
                               std::move(pred), std::move(proj1),
                               std::move(proj2));
    }

    // Three legged
    template <typename I1, typename S1, typename I2, typename Pred = equal_to<>,
              typename Proj1 = identity, typename Proj2 = identity>
    NANO_DEPRECATED constexpr std::enable_if_t<
        InputIterator<I1> && Sentinel<S1, I1> && InputIterator<I2> &&
            IndirectlyComparable<I1, I2, Pred, Proj1, Proj2>,
        bool>
    operator()(I1 first1, S1 last1, I2 first2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return equal_fn::impl3(std::move(first1), std::move(last1),
                               std::move(first2), std::move(pred),
                               std::move(proj1), std::move(proj2));
    }

    // Two ranges, both sized
    template <typename Rng1, typename Rng2, typename Pred = equal_to<>,
              typename Proj1 = identity, typename Proj2 = identity>
    constexpr std::enable_if_t<
        InputRange<Rng1> && InputRange<Rng2> &&
            IndirectlyComparable<iterator_t<Rng1>, iterator_t<Rng2>, Pred,
                                 Proj1, Proj2> &&
            SizedRange<Rng1> && SizedRange<Rng2>,
        bool>
    operator()(Rng1&& rng1, Rng2&& rng2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        if (nano::distance(rng1) != nano::distance(rng2)) {
            return false;
        }

        return equal_fn::impl3(nano::begin(rng1), nano::end(rng1),
                               nano::begin(rng2), std::move(pred),
                               std::move(proj1), std::move(proj2));
    }

    // Two ranges, not both sized
    template <typename Rng1, typename Rng2, typename Pred = equal_to<>,
              typename Proj1 = identity, typename Proj2 = identity>
    constexpr std::enable_if_t<
        InputRange<Rng1> && InputRange<Rng2> &&
            IndirectlyComparable<iterator_t<Rng1>, iterator_t<Rng2>, Pred,
                                 Proj1, Proj2> &&
            !(SizedRange<Rng1> && SizedRange<Rng2>),
        bool>
    operator()(Rng1&& rng1, Rng2&& rng2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return equal_fn::impl4(nano::begin(rng1), nano::end(rng1),
                               nano::begin(rng2), nano::end(rng2),
                               std::move(pred), std::move(proj1),
                               std::move(proj2));
    }

    // Range and a half
    template <typename Rng1, typename I2, typename Pred = equal_to<>,
              typename Proj1 = identity, typename Proj2 = identity>
    NANO_DEPRECATED constexpr std::enable_if_t<
        InputRange<Rng1> && InputIterator<I2> &&
            IndirectlyComparable<iterator_t<Rng1>, I2, Pred, Proj1, Proj2>,
        bool>
    operator()(Rng1&& rng1, I2 first2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return equal_fn::impl3(nano::begin(rng1), nano::end(rng1),
                               std::move(first2), std::move(pred),
                               std::move(proj1), std::move(proj2));
    }
};

} // namespace detail

NANO_INLINE_VAR(detail::equal_fn, equal)

NANO_END_NAMESPACE

#endif