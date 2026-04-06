# Copyright (c) 2026 Kosy Development Team. All rights reserved.

import numpy as np
import numpy.typing as npt

from kosy import Polynomial


def test_polynomial_eval_scalar() -> None:
    # p(z) = 2 + 3z + 4z^2
    p = Polynomial[np.float64]([2.0, 3.0, 4.0])
    assert p.eval(np.float64(0.0)) == 2.0
    assert p.eval(np.float64(1.0)) == 9.0
    assert p.eval(np.float64(2.0)) == 24.0


def test_polynomial_eval_array() -> None:
    p = Polynomial[np.float64]([2.0, 3.0, 4.0])
    zs = np.array([0.0, 1.0, 2.0], dtype=np.float64)
    results: npt.NDArray[np.float64] = p(zs)
    np.testing.assert_allclose(results, [2.0, 9.0, 24.0])


def test_polynomial_dtype_inference() -> None:
    # Direct construction infers the underlying C++ specialisation from the
    # coefficient array dtype.
    p_real = Polynomial(np.array([1.0, -3.0, 2.0]))
    assert p_real.eval(np.float64(2.0)) == 1.0 - 3.0 * 2.0 + 2.0 * 4.0

    p_complex = Polynomial(np.array([-1 + 0j, 0j, 0j, 1 + 0j]))
    val = p_complex.eval(np.complex128(1.0 + 0.0j))
    assert np.isclose(val, 0.0)


def test_polynomial_newton_raphson_cube_roots() -> None:
    # f(z) = z^3 - 1 has three roots on the unit circle.
    f = Polynomial[np.complex128]([-1 + 0j, 0j, 0j, 1 + 0j])
    z0s = np.array(
        [0.9 + 0.05j, -0.4 + 0.9j, -0.4 - 0.9j],
        dtype=np.complex128,
    )
    zs, _dzs, iters = f.newton_raphson(z0s, abs_tol=1e-12, rel_tol=1e-12, max_iter=400)

    expected = np.array(
        [1.0 + 0.0j, -0.5 + np.sqrt(3.0) / 2.0 * 1j, -0.5 - np.sqrt(3.0) / 2.0 * 1j],
        dtype=np.complex128,
    )
    np.testing.assert_allclose(zs, expected, atol=1e-8)
    assert (iters < 100).all()


def test_polynomial_unsupported_dtype_raises() -> None:
    import pytest

    with pytest.raises(TypeError):
        Polynomial[np.int64]  # type: ignore[misc]
