#ifndef OPENSIM_PSIM_DYNAMIC_OPTIMIZATION_SOLVER_H_
#define OPENSIM_PSIM_DYNAMIC_OPTIMIZATION_SOLVER_H_

#include "PSimSolver.h"

#include <SimTKmath.h>

namespace OpenSim {

class PSimDynamicOptimizationSolver : public PSimSolver
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



