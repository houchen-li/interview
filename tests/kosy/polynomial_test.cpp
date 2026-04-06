/**
 * @file polynomial_test.cpp
 * @author Houchen Li (houchen_li@hotmail.com)
 * @brief doctest unit tests for kosy::Polynomial.
 * @version 0.1
 * @date 2026-05-16
 *
 * @copyright Copyright (c) 2026 Kosy Development Team
 *            All rights reserved.
 *
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <array>
#include <complex>
#include <cstdint>
#include <span>
#include <vector>

#include "kosy/polynomial.hpp"

namespace {

constexpr double kAbsTol{1e-10};

template <typename T>
auto approx_equal(T a, T b, double tol = kAbsTol) -> bool {
    if constexpr (std::is_floating_point_v<T>) {
        return std::abs(a - b) <= tol;
    } else {
        return std::abs(a - b) <= static_cast<typename T::value_type>(tol);
    }
}

} // namespace

TEST_CASE("Polynomial<double> evaluation and derivative") {
    // p(z) = 2 + 3z + 4z^2
    const std::vector<double> coeffs{2.0, 3.0, 4.0};
    const kosy::Polynomial<double> p{coeffs};

    SUBCASE("eval scalar") {
        CHECK(approx_equal(p.eval(0.0), 2.0));
        CHECK(approx_equal(p.eval(1.0), 9.0));
        CHECK(approx_equal(p.eval(2.0), 24.0));
        CHECK(approx_equal(p(-1.0), 3.0));
    }

    SUBCASE("eval span") {
        const std::array<double, 3> zs{0.0, 1.0, 2.0};
        const auto results{p.eval(std::span<const double>(zs))};
        REQUIRE(results.size() == 3);
        CHECK(approx_equal(results[0], 2.0));
        CHECK(approx_equal(results[1], 9.0));
        CHECK(approx_equal(results[2], 24.0));
    }

    SUBCASE("derivative scalar") {
        // p'(z) = 3 + 8z
        CHECK(approx_equal(p.derivative(0.0), 3.0));
        CHECK(approx_equal(p.derivative(1.0), 11.0));
        CHECK(approx_equal(p.derivative(2.0), 19.0));
    }

    SUBCASE("coeffs view") {
        const auto view{p.coeffs()};
        REQUIRE(view.size() == 3);
        CHECK(view[0] == doctest::Approx(2.0));
        CHECK(view[1] == doctest::Approx(3.0));
        CHECK(view[2] == doctest::Approx(4.0));
    }
}

TEST_CASE("Polynomial<float> evaluation") {
    // p(z) = 1 - z + z^2
    const std::vector<float> coeffs{1.0F, -1.0F, 1.0F};
    const kosy::Polynomial<float> p{coeffs};
    CHECK(approx_equal(p.eval(0.0F), 1.0F, 1e-5));
    CHECK(approx_equal(p.eval(1.0F), 1.0F, 1e-5));
    CHECK(approx_equal(p.eval(2.0F), 3.0F, 1e-5));
    CHECK(approx_equal(p.derivative(1.0F), 1.0F, 1e-5));
}

TEST_CASE("Polynomial<complex<double>> Newton-Raphson on z^3 - 1") {
    using C = std::complex<double>;
    // p(z) = -1 + 0z + 0z^2 + z^3
    const std::vector<C> coeffs{C{-1.0, 0.0}, C{0.0, 0.0}, C{0.0, 0.0}, C{1.0, 0.0}};
    const kosy::Polynomial<C> p{coeffs};

    const C root1{1.0, 0.0};
    const C root2{-0.5, std::sqrt(3.0) / 2.0};
    const C root3{-0.5, -std::sqrt(3.0) / 2.0};

    SUBCASE("converges to root 1 from nearby start") {
        auto [z, dz, iter] = p.newtonRaphson(C{0.9, 0.05});
        CHECK(approx_equal(z, root1, 1e-8));
        CHECK(iter < 100);
    }

    SUBCASE("converges to root 2 from upper-left start") {
        auto [z, dz, iter] = p.newtonRaphson(C{-0.4, 0.9});
        CHECK(approx_equal(z, root2, 1e-8));
        CHECK(iter < 100);
    }

    SUBCASE("converges to root 3 from lower-left start") {
        auto [z, dz, iter] = p.newtonRaphson(C{-0.4, -0.9});
        CHECK(approx_equal(z, root3, 1e-8));
        CHECK(iter < 100);
    }

    SUBCASE("batched Newton-Raphson span") {
        const std::array<C, 3> z0s{C{0.9, 0.05}, C{-0.4, 0.9}, C{-0.4, -0.9}};
        auto [zs, dzs, iters] = p.newtonRaphson(std::span<const C>(z0s));
        REQUIRE(zs.size() == 3);
        CHECK(approx_equal(zs[0], root1, 1e-8));
        CHECK(approx_equal(zs[1], root2, 1e-8));
        CHECK(approx_equal(zs[2], root3, 1e-8));
    }
}

TEST_CASE("Polynomial<complex<float>> Newton-Raphson on z^2 + 1") {
    using C = std::complex<float>;
    const std::vector<C> coeffs{C{1.0F, 0.0F}, C{0.0F, 0.0F}, C{1.0F, 0.0F}};
    const kosy::Polynomial<C> p{coeffs};

    auto [z, dz, iter] = p.newtonRaphson(C{0.1F, 1.1F});
    CHECK(approx_equal(z, C{0.0F, 1.0F}, 1e-4));
    CHECK(iter < 100);
}
