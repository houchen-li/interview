/**
 * @file polynomial.hpp
 * @author Houchen Li (houchen_li@hotmail.com)
 * @brief Polynomial class template for evaluation, differentiation, and Newton-Raphson root
 *        finding.
 * @version 0.1
 * @date 2026-04-10
 *
 * @copyright Copyright (c) 2026 Kosy Development Team
 *            All rights reserved.
 *
 */

#pragma once

#include <complex>
#include <cstdint>
#include <ranges>
#include <span>
#include <tuple>
#include <vector>

#include "kosy/utils/aligned_allocator.hpp"
#include "kosy/utils/concepts.hpp"

namespace kosy {

namespace detail {

template <typename T>
struct NormTypeTrait final {
    static_assert(sizeof(T) == 0, "RealTypeTrait is only defined for arithmetic and complex types");
};

template <Arithmetic T>
struct NormTypeTrait<T> final {
    using type = T;
};

template <ComplexArithmetic T>
struct NormTypeTrait<T> final {
    using type = typename T::value_type;
};

template <typename T>
using NormTypeTraitT = typename NormTypeTrait<T>::type;

} // namespace detail

template <ScalarArithmetic T, Allocatory Alloc = kosy::AlignedAllocator<T, 32>>
class [[nodiscard]] Polynomial final {
  public:
    using value_type = T;
    using allocator_type = Alloc;
    using norm_type = detail::NormTypeTraitT<T>;

    Polynomial(const Polynomial& other) noexcept = default;
    auto operator=(const Polynomial& other) noexcept -> Polynomial& = default;
    Polynomial(Polynomial&& other) noexcept = default;
    auto operator=(Polynomial&& other) noexcept -> Polynomial& = default;
    ~Polynomial() noexcept = default;

    [[using gnu: pure, always_inline]]
    auto get_allocator() const noexcept -> allocator_type {
        return m_coeffs.get_allocator();
    }

    template <std::ranges::input_range InputRange>
    explicit Polynomial(InputRange&& coeffs, const allocator_type& alloc = {}) noexcept
        : m_coeffs{std::begin(coeffs), std::end(coeffs), alloc} {}

    [[using gnu: pure, always_inline]]
    auto operator()(value_type z) const noexcept -> value_type {
        return eval(z);
    }

    [[using gnu: pure, always_inline]]
    auto operator()(std::span<const value_type> zs) const noexcept
        -> std::vector<value_type, allocator_type> {
        return eval(zs);
    }

    [[using gnu: pure, flatten, leaf, hot]]
    auto eval(value_type z) const noexcept -> value_type {
        value_type result{m_coeffs[0]};
        value_type power{1.0};
        for (std::size_t i{1}; i < m_coeffs.size(); ++i) {
            power *= z;
            result += m_coeffs[i] * power;
        }
        return result;
    }

    [[using gnu: pure]]
    auto eval(std::span<const value_type> zs) const noexcept
        -> std::vector<value_type, allocator_type> {
        std::vector<value_type, allocator_type> results(zs.size(), get_allocator());
        const auto n = static_cast<std::ptrdiff_t>(zs.size());
#pragma omp parallel for schedule(static)
        for (std::ptrdiff_t i = 0; i < n; ++i) {
            results[static_cast<std::size_t>(i)] = eval(zs[static_cast<std::size_t>(i)]);
        }
        return results;
    }

    [[using gnu: pure, flatten, leaf, hot]]
    auto derivative(value_type z) const noexcept -> value_type {
        if (m_coeffs.size() < 2) {
            return T{0};
        }
        value_type result{m_coeffs[1]};
        value_type power{1.0};
        norm_type factor{1.0};
        for (std::size_t i{2}; i < m_coeffs.size(); ++i) {
            power *= z;
            factor += static_cast<norm_type>(1.0);
            result += m_coeffs[i] * power * factor;
        }
        return result;
    }

    [[using gnu: pure]]
    auto derivative(std::span<const value_type> zs) const noexcept
        -> std::vector<value_type, allocator_type> {
        std::vector<value_type, allocator_type> results(zs.size(), get_allocator());
        const auto n = static_cast<std::ptrdiff_t>(zs.size());
#pragma omp parallel for schedule(static)
        for (std::ptrdiff_t i = 0; i < n; ++i) {
            results[static_cast<std::size_t>(i)] = derivative(zs[static_cast<std::size_t>(i)]);
        }
        return results;
    }

    [[using gnu: pure, flatten, leaf, hot]]
    auto newtonRaphson(
        value_type z0, norm_type abs_tol = 1e-12, norm_type rel_tol = 1e-12,
        std::int64_t max_iter = 400
    ) const noexcept -> std::tuple<value_type, value_type, std::int64_t> {
        value_type z{z0};
        value_type dz{T{0}};
        std::int64_t i{0};
        for (; i < max_iter; ++i) {
            dz = -eval(z) / derivative(z);
            z += dz;
            const norm_type dz_norm_sq{std::norm(dz)};
            const norm_type z_norm_sq{std::norm(z)};
            if (dz_norm_sq < abs_tol * abs_tol || dz_norm_sq < z_norm_sq * rel_tol * rel_tol) {
                break;
            }
        }
        return {z, dz, i};
    }

    [[using gnu: pure]]
    auto newtonRaphson(
        std::span<const value_type> z0s, norm_type abs_tol = 1e-12, norm_type rel_tol = 1e-12,
        std::int64_t max_iter = 400
    ) const noexcept
        -> std::tuple<
            std::vector<value_type, allocator_type>, std::vector<value_type, allocator_type>,
            std::vector<
                std::int64_t, typename std::allocator_traits<allocator_type>::template rebind_alloc<
                                  std::int64_t>>> {
        const std::size_t n{z0s.size()};
        std::vector<value_type, allocator_type> zs(n, get_allocator());
        std::vector<value_type, allocator_type> dzs(n, get_allocator());
        std::vector<
            std::int64_t,
            typename std::allocator_traits<allocator_type>::template rebind_alloc<std::int64_t>>
            iters(n, get_allocator());
        const auto sn = static_cast<std::ptrdiff_t>(n);
#pragma omp parallel for schedule(static)
        for (std::ptrdiff_t i = 0; i < sn; ++i) {
            auto [z, dz, iter]{
                newtonRaphson(z0s[static_cast<std::size_t>(i)], abs_tol, rel_tol, max_iter)
            };
            zs[static_cast<std::size_t>(i)] = z;
            dzs[static_cast<std::size_t>(i)] = dz;
            iters[static_cast<std::size_t>(i)] = iter;
        }
        return {std::move(zs), std::move(dzs), std::move(iters)};
    }

    [[using gnu: pure, always_inline]]
    auto coeffs() const noexcept -> std::span<const value_type> {
        return std::span<const value_type>{m_coeffs};
    }

  private:
    std::vector<value_type, allocator_type> m_coeffs;
};

} // namespace kosy
