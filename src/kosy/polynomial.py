# Copyright (c) 2026 Kosy Development Team. All rights reserved.

from typing import Any, ClassVar

import numpy as np
import numpy.typing as npt

from kosy._core import (
    _PolynomialC64,
    _PolynomialC128,
    _PolynomialF32,
    _PolynomialF64,
)

_DType = type[np.generic] | type[float] | type[complex]


class Polynomial:
    """Generic Polynomial dispatcher.

    Coefficients can be a Python list or any array-like object (e.g. numpy
    array). Use ``Polynomial[dtype]`` to obtain the typed C++ class, then
    instantiate it::

        f = Polynomial[np.complex128]([-1 + 0j, 0j, 0j, 1 + 0j])
        f = Polynomial[float]([1.0, -3.0, 2.0])
        f = Polynomial[np.float64](np.array([1.0, -3.0, 2.0]))

    Direct instantiation infers the dtype from the coefficient array::

        f = Polynomial([-1 + 0j, 0j, 0j, 1 + 0j])  # -> _PolynomialC128
        f = Polynomial(np.array([1.0, -3.0, 2.0]))  # -> _PolynomialF64
    """

    _type_map: ClassVar[dict[_DType, type]] = {
        np.float32: _PolynomialF32,
        np.float64: _PolynomialF64,
        float: _PolynomialF64,
        np.complex64: _PolynomialC64,
        np.complex128: _PolynomialC128,
        complex: _PolynomialC128,
    }

    def __class_getitem__(cls, dtype: _DType) -> type:
        t = cls._type_map.get(dtype)
        if t is None:
            raise TypeError(f"Polynomial: unsupported dtype {dtype!r}")
        return t

    def __new__(cls, coeffs: npt.ArrayLike) -> Any:
        arr = np.asarray(coeffs)
        t = cls._type_map.get(arr.dtype.type)
        if t is None:
            raise TypeError(
                f"Polynomial: cannot infer type from dtype {arr.dtype!r}; "
                f"use Polynomial[dtype](coeffs) instead"
            )
        return t(np.ascontiguousarray(arr))
