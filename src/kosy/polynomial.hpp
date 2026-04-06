/*!
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
struct RealTypeTrait final {
    static_assert(sizeof(T) == 0, "RealTypeTrait is only defined for arithmetic and complex types");
};

template <Arithmetic T>
struct RealTypeTrait<T> final {
    using type = T;
};

template <ComplexArithmetic T>
struct RealTypeTrait<T> final {
    using type = typename T::value_type;
};

template <typename T>
using RealTypeTraitT = typename RealTypeTrait<T>::type;

} // namespace detail

template <ScalarArithmetic T, Allocatory Alloc = kosy::AlignedAllocator<T, 32>>
class [[nodiscard]] Polynomial final {
  public:
    using value_type = T;
    using allocator_type = Alloc;
    using real_type = detail::RealTypeTraitT<T>;

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

    auto operator()(value_type z) const noexcept -> value_type { return eval(z); }

    auto eval(value_type z) const noexcept -> value_type {
        value_type result{m_coeffs[0]};
        value_type power{T{1}};
        for (std::size_t i{1}; i < m_coeffs.size(); ++i) {
            power *= z;
            result += m_coeffs[i] * power;
        }
        return result;
    }

    auto derivative(value_type z) const noexcept -> value_type {
        if (m_coeffs.size() < 2) {
            return T{0};
        }
        value_type result{m_coeffs[1]};
        value_type power{1.0};
        real_type factor{1.0};
        for (std::size_t i{2}; i < m_coeffs.size(); ++i) {
            power *= z;
            factor += static_cast<real_type>(1.0);
            result += m_coeffs[i] * power * factor;
        }
        return result;
    }

    auto newtonRaphson(
        value_type z0, real_type abs_tol = 1e-12, real_type rel_tol = 1e-12,
        std::int64_t max_iter = 400
    ) const noexcept -> std::tuple<value_type, value_type, std::int64_t> {
        value_type z{z0};
        value_type dz{T{0}};
        std::int64_t i{0};
        for (; i < max_iter; ++i) {
            dz = -eval(z) / derivative(z);
            z += dz;
            const real_type dz_norm_sq{std::norm(dz)};
            const real_type z_norm_sq{std::norm(z)};
            if (dz_norm_sq < abs_tol * abs_tol || dz_norm_sq < z_norm_sq * rel_tol * rel_tol) {
                break;
            }
        }
        return {z, dz, i};
    }

    auto coeffs() const noexcept -> std::span<const value_type> {
        return std::span<const value_type>{m_coeffs};
    }

  private:
    std::vector<value_type, allocator_type> m_coeffs;
};

} // namespace kosy
