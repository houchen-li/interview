/**
 * @file polynomial_nb.cpp
 * @author Houchen Li (houchen_li@hotmail.com)
 * @brief nanobind bindings for the Polynomial class. Exposes
 *        kosy::bindings::bind_polynomial, invoked from _core.cpp.
 * @version 0.1
 * @date 2026-04-10
 *
 * @copyright Copyright (c) 2026 Kosy Development Team
 *            All rights reserved.
 *
 */

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>

#include <complex>
#include <cstdint>
#include <span>
#include <vector>

#include "kosy/polynomial.hpp"

namespace nb = nanobind;
using nb::literals::operator""_a;

namespace {

template <typename T>
void bindPolynomial(nb::module_& m, const char* name) {
    using Poly = kosy::Polynomial<T>;
    using NormT = typename Poly::norm_type;
    using AllocT = typename Poly::allocator_type;

    nb::class_<Poly>(m, name)
        .def(
            "__init__",
            [](Poly* self, const std::vector<T>& coeffs) noexcept -> void {
                new (self) Poly(coeffs);
            },
            "coeffs"_a
        )
        .def(
            "__init__",
            [](
                Poly* self,
                const nb::ndarray<const T, nb::ndim<1>, nb::c_contig, nb::device::cpu>& coeffs
            ) noexcept -> void {
                new (self) Poly(std::span<const T>(coeffs.data(), coeffs.shape(0)));
            },
            "coeffs"_a
        )
        // Scalar overloads
        .def(
            "__call__", [](const Poly& self, T z) noexcept { return self(z); }, "z"_a
        )
        .def(
            "eval", [](const Poly& self, T z) noexcept { return self.eval(z); }, "z"_a
        )
        .def(
            "derivative", [](const Poly& self, T z) noexcept { return self.derivative(z); }, "z"_a
        )
        .def(
            "newton_raphson",
            [](const Poly& self, T z0, NormT abs_tol, NormT rel_tol,
               std::int64_t max_iter) noexcept -> std::tuple<T, T, std::int64_t> {
                return self.newtonRaphson(z0, abs_tol, rel_tol, max_iter);
            },
            "z0"_a, "abs_tol"_a = 1e-12, "rel_tol"_a = 1e-12, "max_iter"_a = 400
        )
        // Array overloads — accept N-D C-contiguous numpy arrays, return arrays of the same shape
        .def(
            "__call__",
            [](const Poly& self, nb::ndarray<const T, nb::c_contig, nb::device::cpu> zs) noexcept
                -> nb::ndarray<nb::numpy, T> {
                std::vector<
                    std::size_t,
                    typename std::allocator_traits<AllocT>::template rebind_alloc<std::size_t>>
                    shape(zs.ndim(), self.get_allocator());
                for (std::size_t i{0}; i < zs.ndim(); ++i) {
                    shape[i] = zs.shape(i);
                }
                auto* result{
                    new std::vector<T, AllocT>(self.eval(std::span<const T>(zs.data(), zs.size())))
                };
                nb::capsule owner(result, [](void* p) noexcept -> void {
                    delete static_cast<std::vector<T, AllocT>*>(p);
                });
                return nb::ndarray<nb::numpy, T>(result->data(), zs.ndim(), shape.data(), owner);
            },
            "zs"_a
        )
        .def(
            "eval",
            [](const Poly& self, nb::ndarray<const T, nb::c_contig, nb::device::cpu> zs) noexcept
                -> nb::ndarray<nb::numpy, T> {
                std::vector<
                    std::size_t,
                    typename std::allocator_traits<AllocT>::template rebind_alloc<std::size_t>>
                    shape(zs.ndim(), self.get_allocator());
                for (std::size_t i{0}; i < zs.ndim(); ++i) {
                    shape[i] = zs.shape(i);
                }
                auto* result{
                    new std::vector<T, AllocT>(self.eval(std::span<const T>(zs.data(), zs.size())))
                };
                nb::capsule owner(result, [](void* p) noexcept -> void {
                    delete static_cast<std::vector<T, AllocT>*>(p);
                });
                return nb::ndarray<nb::numpy, T>(result->data(), zs.ndim(), shape.data(), owner);
            },
            "zs"_a
        )
        .def(
            "derivative",
            [](const Poly& self, nb::ndarray<const T, nb::c_contig, nb::device::cpu> zs) noexcept
                -> nb::ndarray<nb::numpy, T> {
                std::vector<
                    std::size_t,
                    typename std::allocator_traits<AllocT>::template rebind_alloc<std::size_t>>
                    shape(zs.ndim(), self.get_allocator());
                for (size_t i{0}; i < zs.ndim(); ++i) {
                    shape[i] = zs.shape(i);
                }
                auto* result{new std::vector<T, AllocT>(
                    self.derivative(std::span<const T>(zs.data(), zs.size()))
                )};
                nb::capsule owner(result, [](void* p) noexcept -> void {
                    delete static_cast<std::vector<T, AllocT>*>(p);
                });
                return nb::ndarray<nb::numpy, T>(result->data(), zs.ndim(), shape.data(), owner);
            },
            "zs"_a
        )
        .def(
            "newton_raphson",
            [](const Poly& self, nb::ndarray<const T, nb::c_contig, nb::device::cpu> z0s,
               NormT abs_tol, NormT rel_tol, std::int64_t max_iter) noexcept
                -> std::tuple<
                    nb::ndarray<nb::numpy, T>, nb::ndarray<nb::numpy, T>,
                    nb::ndarray<nb::numpy, std::int64_t>> {
                std::vector<
                    std::size_t,
                    typename std::allocator_traits<AllocT>::template rebind_alloc<std::size_t>>
                    shape(z0s.ndim(), self.get_allocator());
                for (std::size_t i{0}; i < z0s.ndim(); ++i) {
                    shape[i] = z0s.shape(i);
                }
                auto [zs, dzs, iters]{self.newtonRaphson(
                    std::span<const T>(z0s.data(), z0s.size()), abs_tol, rel_tol, max_iter
                )};

                auto* zs_ptr{new std::vector<T, AllocT>(std::move(zs), self.get_allocator())};
                nb::capsule zs_owner(zs_ptr, [](void* p) noexcept -> void {
                    delete static_cast<std::vector<T, AllocT>*>(p);
                });
                auto zs_arr{
                    nb::ndarray<nb::numpy, T>(zs_ptr->data(), z0s.ndim(), shape.data(), zs_owner)
                };

                auto* dzs_ptr{new std::vector<T, AllocT>(std::move(dzs), self.get_allocator())};
                nb::capsule dzs_owner(dzs_ptr, [](void* p) noexcept -> void {
                    delete static_cast<std::vector<T, AllocT>*>(p);
                });
                auto dzs_arr{
                    nb::ndarray<nb::numpy, T>(dzs_ptr->data(), z0s.ndim(), shape.data(), dzs_owner)
                };

                auto* iters_ptr{new std::vector<
                    std::int64_t,
                    typename std::allocator_traits<AllocT>::template rebind_alloc<std::int64_t>>(
                    std::move(iters), self.get_allocator()
                )};
                nb::capsule iters_owner(iters_ptr, [](void* p) noexcept -> void {
                    delete static_cast<std::vector<
                        std::int64_t, typename std::allocator_traits<AllocT>::template rebind_alloc<
                                          std::int64_t>>*>(p);
                });
                auto iters_arr{nb::ndarray<nb::numpy, std::int64_t>(
                    iters_ptr->data(), z0s.ndim(), shape.data(), iters_owner
                )};

                return std::make_tuple(zs_arr, dzs_arr, iters_arr);
            },
            "z0s"_a, "abs_tol"_a = 1e-12, "rel_tol"_a = 1e-12, "max_iter"_a = 400
        )
        .def("coeffs", &kosy::Polynomial<T>::coeffs);
}

} // namespace

namespace kosy::bindings {

auto bind_polynomial(nb::module_& m) -> void {
    bindPolynomial<float>(m, "_PolynomialF32");
    bindPolynomial<double>(m, "_PolynomialF64");
    bindPolynomial<std::complex<float>>(m, "_PolynomialC64");
    bindPolynomial<std::complex<double>>(m, "_PolynomialC128");
}

} // namespace kosy::bindings
