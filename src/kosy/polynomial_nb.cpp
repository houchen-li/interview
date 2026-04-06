/*!
 * @file polynomial_nb.cpp
 * @author Houchen Li (houchen_li@hotmail.com)
 * @brief nanobind bindings for the Polynomial class.
 * @version 0.1
 * @date 2026-04-10
 *
 * @copyright Copyright (c) 2026 Kosy Development Team
 *            All rights reserved.
 *
 */

#include <nanobind/eval.h>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>

#include <complex>
#include <span>
#include <vector>

#include "kosy/polynomial.hpp"

namespace nb = nanobind;
using nb::literals::operator""_a;

template <typename T>
static void bindPolynomial(nb::module_& m, const char* name) {
    nb::class_<kosy::Polynomial<T>>(m, name)
        .def(
            "__init__",
            [](kosy::Polynomial<T>* self, const std::vector<T>& coeffs) -> void {
                new (self) kosy::Polynomial<T>(coeffs);
            },
            "coeffs"_a
        )
        .def(
            "__init__",
            [](kosy::Polynomial<T>* self,
               const nb::ndarray<const T, nb::ndim<1>, nb::c_contig, nb::device::cpu>& coeffs)
                -> void {
                new (self) kosy::Polynomial<T>(std::span<const T>(coeffs.data(), coeffs.shape(0)));
            },
            "coeffs"_a
        )
        .def("__call__", &kosy::Polynomial<T>::operator(), "z"_a)
        .def("eval", &kosy::Polynomial<T>::eval, "z"_a)
        .def("derivative", &kosy::Polynomial<T>::derivative, "z"_a)
        .def(
            "newton_raphson", &kosy::Polynomial<T>::newtonRaphson, "z0"_a, "abs_tol"_a = 1e-12,
            "rel_tol"_a = 1e-12, "max_iter"_a = 400
        )
        .def("coeffs", &kosy::Polynomial<T>::coeffs);
}

NB_MODULE(polynomial_ext, m) {
    m.doc() = "C++ Polynomial class with nanobind bindings.";

    bindPolynomial<float>(m, "_PolynomialF32");
    bindPolynomial<double>(m, "_PolynomialF64");
    bindPolynomial<std::complex<float>>(m, "_PolynomialC64");
    bindPolynomial<std::complex<double>>(m, "_PolynomialC128");

    // Define a generic Python-level Polynomial class that dispatches to the
    // correct C++ specialization via __class_getitem__.
    nb::exec(
        R"(
import numpy as _np

class Polynomial:
    """Generic Polynomial class.

    Coefficients can be a Python list or any array-like object (e.g. numpy array).

    Use ``Polynomial[dtype]`` to obtain the typed C++ class, then instantiate it::

        f = Polynomial[_np.complex128]([-1+0j, 0j, 0j, 1+0j])
        f = Polynomial[float]([1.0, -3.0, 2.0])
        f = Polynomial[_np.float64](_np.array([1.0, -3.0, 2.0]))

    Direct instantiation infers the dtype from the coefficient array::

        f = Polynomial([-1+0j, 0j, 0j, 1+0j])           # -> _PolynomialC128
        f = Polynomial(_np.array([1.0, -3.0, 2.0]))      # -> _PolynomialF64
    """

    _type_map = {
        _np.float32:    _PolynomialF32,
        _np.float64:    _PolynomialF64,
        float:          _PolynomialF64,
        _np.complex64:  _PolynomialC64,
        _np.complex128: _PolynomialC128,
        complex:        _PolynomialC128,
    }

    def __class_getitem__(cls, dtype):
        t = cls._type_map.get(dtype)
        if t is None:
            raise TypeError(f"Polynomial: unsupported dtype {dtype!r}")
        return t

    def __new__(cls, coeffs):
        arr = _np.asarray(coeffs)
        t = cls._type_map.get(arr.dtype.type)
        if t is None:
            raise TypeError(
                f"Polynomial: cannot infer type from dtype {arr.dtype!r}; "
                f"use Polynomial[dtype](coeffs) instead"
            )
        return t(_np.ascontiguousarray(arr))
)",
        m.attr("__dict__")
    );
}
