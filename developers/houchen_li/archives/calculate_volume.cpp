/**
 * @file calculate_volume.cpp
 * @author Houchen Li (houchen_li@hotmail.com)
 * @brief
 * @version 0.1
 * @date 2026-04-10
 *
 * @copyright Copyright (c) 2024 Kosy Development Team
 *            All rights reserved.
 *
 */

#include <array>
#include <concepts>
#include <print>
#include <random>

template <std::floating_point T>
struct Vec3 final {
    using value_type = T;
    value_type x, y, z;
};

template <
    std::floating_point T, typename Generator = std::mt19937,
    typename Distribution = typename std::uniform_real_distribution<T>>
class [[nodiscard]] Scatter final {
  public:
    using value_type = T;
    using generator_type = Generator;
    using distribution_type = Distribution;

    Scatter(const Scatter& other) noexcept = delete;
    auto operator=(const Scatter& other) noexcept -> Scatter& = delete;
    Scatter(Scatter&& other) noexcept = delete;
    auto operator=(Scatter&& other) noexcept -> Scatter& = delete;
    ~Scatter() noexcept = default;

    [[using gnu: always_inline]]
    auto operator()() noexcept -> Vec3<value_type> {
        return Vec3<value_type>{
            .x{m_distribution_x(m_generator)},
            .y{m_distribution_y(m_generator)},
            .z{m_distribution_z(m_generator)}
        };
    }

    explicit Scatter(std::random_device& rd, const std::array<value_type, 6>& bounds) noexcept
        : m_generator{rd()}, m_distribution_x{bounds[0], bounds[1]},
          m_distribution_y{bounds[2], bounds[3]}, m_distribution_z{bounds[4], bounds[5]} {}

    explicit Scatter(const std::array<value_type, 6>& bounds) noexcept
        : m_generator{std::random_device{}()}, m_distribution_x{bounds[0], bounds[1]},
          m_distribution_y{bounds[2], bounds[3]}, m_distribution_z{bounds[4], bounds[5]} {}

  private:
    generator_type m_generator;
    distribution_type m_distribution_x, m_distribution_y, m_distribution_z;
};

template <std::floating_point T>
struct Ellipsoid final {
    using value_type = T;
    static constexpr std::size_t kGrossSampling{1000000};
    value_type a, b, c;

    auto volume() const noexcept -> value_type {
        Scatter<value_type> scatter{{-a, a, -b, b, -c, c}};
        std::size_t count{0};
        for (std::size_t i{0}; i < kGrossSampling; ++i) {
            const Vec3<value_type> point{scatter()};
            if (point.x * point.x / (a * a) + point.y * point.y / (b * b) +
                    point.z * point.z / (c * c) <=
                1.0) {
                ++count;
            }
        }
        return static_cast<value_type>(count) / static_cast<value_type>(kGrossSampling) * a * b *
               c * 8.0;
    }
};

template <std::floating_point T>
struct Cone final {
    using value_type = T;
    static constexpr std::size_t kGrossSampling{1000000};
    value_type a, b, c;

    auto volume() const noexcept -> value_type {
        Scatter<value_type> scatter{{-a, a, -b, b, 0.0, c}};
        std::size_t count{0};
        for (std::size_t i{0}; i < kGrossSampling; ++i) {
            const Vec3<value_type> point{scatter()};
            if (point.x * point.x / (a * a) + point.y * point.y / (b * b) <=
                point.z * point.z / (c * c)) {
                ++count;
            }
        }
        return static_cast<value_type>(count) / static_cast<value_type>(kGrossSampling) * a * b *
               c * 4.0;
    }
};

auto main() noexcept -> int {
    Ellipsoid<double> ellipsoid{3.0, 2.0, 1.0};
    std::println("Volume of the ellipsoid: {}", ellipsoid.volume());
    return 0;
}
