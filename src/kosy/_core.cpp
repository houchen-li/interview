/**
 * @file _core.cpp
 * @author Houchen Li (houchen_li@hotmail.com)
 * @brief nanobind module entry for kosy._core. Aggregates per-feature binding
 *        registrars defined in sibling translation units.
 * @version 0.1
 * @date 2026-05-16
 *
 * @copyright Copyright (c) 2026 Kosy Development Team
 *            All rights reserved.
 *
 */

#include <nanobind/nanobind.h>

namespace kosy::bindings {

auto bind_polynomial(nanobind::module_& m) -> void;

} // namespace kosy::bindings

NB_MODULE(_core, m) {
    m.doc() = "Low-level C++ bindings for kosy.";
    kosy::bindings::bind_polynomial(m);
}
