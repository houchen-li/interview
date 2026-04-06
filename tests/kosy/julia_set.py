from concurrent.futures import ProcessPoolExecutor
from itertools import product
from typing import TypeVar

import matplotlib.pyplot as plt
import numpy as np
import numpy.typing as npt

T = TypeVar("T", np.float32, np.float64, np.complex64, np.complex128)


class Polynomial[T: "Polynomial"]:
    type dtype = T

    def __init__(self, coeffs: npt.ArrayLike[T]) -> None:
        self.coeffs: npt.NDArray[T] = coeffs
        return

    def __call__(self, z: T | npt.ArrayLike[T], /) -> T | npt.NDArray[T]:
        return self.eval(z)

    def eval(self, z: T | npt.ArrayLike[T], /) -> T | npt.NDArray[T]:
        result: T = self.coeffs[0]
        power: T = 1.0
        for c in self.coeffs[1:]:
            power *= z
            result += c * power
        return result

    def derivative(self, z: T | npt.ArrayLike[T], /) -> T | npt.NDArray[T]:
        result: T = self.coeffs[1]
        power: T = 1.0
        factor: np.int64 = 1
        for c in self.coeffs[2:]:
            power *= z
            factor += 1
            result += c * power * factor
        return result

    def _newton_raphson(
        self,
        z0: T,
        *,
        abs_tol: np.float64 = 1e-12,
        rel_tol: np.float64 = 1e-12,
        max_iter: np.int64 = 400,
    ) -> tuple[T, T, np.int64]:
        z: T = z0
        z_norm_sq: np.float64 = 0.0
        dz: T = 0.0
        dz_norm_sq: np.float64 = 0.0
        for i in range(0, max_iter):  # noqa: B007
            dz = -(self.eval(z)) / (self.derivative(z))
            z += dz
            dz_norm_sq = dz.real * dz.real + dz.imag * dz.imag
            z_norm_sq = z.real * z.real + z.imag * z.imag
            if dz_norm_sq < abs_tol * abs_tol or dz_norm_sq < z_norm_sq * rel_tol * rel_tol:
                break
        return z, dz, i

    def newton_raphson(
        self,
        z0: T | npt.ArrayLike[T],
        *,
        abs_tol: np.float64 = 1e-12,
        rel_tol: np.float64 = 1e-12,
        max_iter: np.int64 = 400,
        num_workers: np.int64 = 1,
    ) -> tuple[T, T, np.int64] | tuple[npt.NDArray[T], npt.NDArray[T], npt.NDArray[np.int64]]:
        if isinstance(z0, np.number):
            return self._newton_raphson(z0, abs_tol=abs_tol, rel_tol=rel_tol, max_iter=max_iter)
        elif isinstance(z0, np.ndarray):
            if num_workers == 1:
                zs: npt.NDArray[T] = np.empty(shape=z0.shape, dtype=z0.dtype)
                dzs: npt.NDArray[T] = np.empty(shape=z0.shape, dtype=z0.dtype)
                iters: npt.NDArray[np.int64] = np.empty(shape=z0.shape, dtype=np.int64)
                for i, j in product(range(0, np.shape(z0)[0]), range(0, np.shape(z0)[1])):
                    zs[i, j], dzs[i, j], iters[i, j] = self._newton_raphson(
                        z0[i, j], abs_tol=abs_tol, rel_tol=rel_tol, max_iter=max_iter
                    )
                return zs, dzs, iters
            else:
                results: list[tuple[T, T, np.int64]] = []
                futures: list[ProcessPoolExecutor] = []
                with ProcessPoolExecutor(max_workers=num_workers) as executor:
                    for i, j in product(range(0, np.shape(z0)[0]), range(0, np.shape(z0)[1])):
                        futures.append(
                            executor.submit(
                                self._newton_raphson,
                                z0[i, j],
                                abs_tol=abs_tol,
                                rel_tol=rel_tol,
                                max_iter=max_iter,
                            )
                        )
                    for future in futures:
                        results.append(future.result())
                zs, dzs, iters = zip(*results)
                return (
                    np.asarray(zs).reshape(z0.shape),
                    np.asarray(dzs).reshape(z0.shape),
                    np.asarray(iters).reshape(z0.shape),
                )
        else:
            raise UnImplementedError(f"Unsupported type for z0: {type(z0)}")


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
    f: Polynomial[np.complex128] = Polynomial[np.complex128]([-1.0, 0.0, 0.0, 1.0])

    z0: npt.NDArray[np.complex128] = np.empty(shape=(num, num), dtype=np.complex128)
    z0.real, z0.imag = np.meshgrid(
        np.linspace(real_min, real_max, num), np.linspace(imag_min, imag_max, num)
    )

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
