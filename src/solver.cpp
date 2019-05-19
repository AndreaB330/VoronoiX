//
// Created by Andrey on 07-Apr-19.
//

#include <vx/solver.hpp>
#include "core_impl.hpp"
#include "vx/solver.hpp"

vx::VXSolver::VXSolver() {
    core = std::make_unique<core_impl>();
}

void vx::VXSolver::fit(const std::vector<Vec2>& data) {
    core->fit(data);
}
