#ifndef OPENSIM_PSIM_DYNAMIC_OPTIMIZATION_SOLVER_H_
#define OPENSIM_PSIM_DYNAMIC_OPTIMIZATION_SOLVER_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimDynamicOptimizationSolver.h               *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2014 Stanford University and the Authors                *
 * Author(s): Chris Dembia                                                    *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "PSimSolver.h"

#include <SimTKmath.h>

#include "osimPSimDLL.h"

namespace OpenSim {

class OSIMPSIM_API PSimDynamicOptimizationSolver : public PSimSolver
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimDynamicOptimizationSolver, PSimSolver);

public:

    /// @name Property declarations
    /// @{
    // TODO
    // OpenSim_DECLARE_PROPERTY(optimization_algorithm, std::string,
    //         "Optimization algorithm to use.");
    OpenSim_DECLARE_PROPERTY(optimization_convergence_tolerance, double,
            "Convergence tolerance for the optimizer. The smaller this value, "
            "the deeper the convergence. "
            "Decreasing this number can improve a solution, but will also "
            "likely increase computation time.");
    /// @}

    PSimDynamicOptimizationSolver();

private:

    PSimParameterValueSet extendSolve(const PSimTool& tool) const override;

    void constructProperties();

    /** The system that this solver optimizes. Given parameters from the
     * optimizer, the objective function updates the model and initial state
     * and runs a simulation. Then, the objective function evaluates the
     * PSimTool's PSimGoal terms, and sends the resulting objective function
     * value back to the optimizer.
     */
    class OptimizerSystem : public SimTK::OptimizerSystem
    {
    public:
        OptimizerSystem(const PSimTool& tool);
        int objectiveFunc(const SimTK::Vector& parameters, bool new_parameters,
                SimTK::Real& f) const override;
    
    private:
        const PSimTool& m_pstool;
    };

};

} // namespace OpenSim

#endif // OPENSIM_PSIM_DYNAMIC_OPTIMIZATION_SOLVER_H_



