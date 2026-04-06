# Copyright (c) 2026 Kosy Development Team. All rights reserved.


import matplotlib.pyplot as plt
import numpy as np
import numpy.typing as npt
from polynomial_ext import Polynomial


def main() -> int:
    real_min: np.float64 = -2.0
    real_max: np.float64 = 2.0
    imag_min: np.float64 = -2.0
    imag_max: np.float64 = 2.0
    num: np.int64 = 1001
    abs_tol: np.float64 = 1e-12
    rel_tol: np.float64 = 1e-12
    max_iter: np.int64 = 50
    num_cbar_levels: np.int64 = 51

    # f(z) = z * z * z - 1.0.
    f: Polynomial = Polynomial[np.complex128]([-1.0 + 0j, 0.0 + 0j, 0.0 + 0j, 1.0 + 0j])

    z0: npt.NDArray[np.complex128] = np.empty(shape=(num, num), dtype=np.complex128)
    z0.real, z0.imag = np.meshgrid(
        np.linspace(real_min, real_max, num), np.linspace(imag_min, imag_max, num)
    )

    # compute the iteration time for each initial point z0.
    zs, dzs, iters = f.newton_raphson(z0, abs_tol=abs_tol, rel_tol=rel_tol, max_iter=max_iter)

    # plot iteration time versus initial point z0 as a contourf diagram.
    fig, ax = plt.subplots(1, 1)
    ax.set_aspect("equal", adjustable="box")
    ax.set_xlim((real_min, real_max))
    ax.set_ylim((imag_min, imag_max))
    cs = ax.contourf(
        z0.real,
        z0.imag,
        iters,
        levels=np.linspace(0, max_iter, num_cbar_levels, dtype=np.int64),
    )
    fig.colorbar(cs)
    plt.show()

    return 0


if __name__ == "__main__":
    main()
