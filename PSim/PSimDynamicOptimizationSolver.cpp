
#include "PSimDynamicOptimizationSolver.h"

#include "StatesCollector.h"
#include "StateTrajectory.h"
#include "PSimTool.h"

#include <OpenSim/Simulation/Manager/Manager.h>

using namespace OpenSim;

PSimDynamicOptimizationSolver::PSimDynamicOptimizationSolver()
{
    constructProperties();
}

void PSimDynamicOptimizationSolver::constructProperties()
{
    constructProperty_optimization_convergence_tolerance(1e-4);
}

PSimParameterValueSet PSimDynamicOptimizationSolver::extendSolve(
        const PSimTool& tool) const
{
    // Get initial guess, and parameter limits.
    // ========================================
    SimTK::Vector results;
    SimTK::Vector lower;
    SimTK::Vector upper;
    tool.initialOptimizerParameterValuesAndLimits(results, lower, upper);

    // Setup the solver.
    // =================
    OptimizerSystem optsys(tool);
    optsys.setParameterLimits(lower, upper);

    // Create an Optimizer.
    // ====================
    SimTK::Optimizer opt(optsys);
    opt.setConvergenceTolerance(get_optimization_convergence_tolerance());
    opt.useNumericalGradient(true);

    // Optimize!
    // =========
    opt.optimize(results);

    // Return the solution to the optimization.
    // ========================================
    return tool.createParameterValueSet(results);
}

PSimDynamicOptimizationSolver::OptimizerSystem::OptimizerSystem(
        const PSimTool & tool)
: SimTK::OptimizerSystem(tool.numOptimizerParameters()), m_pstool(tool)
{
}

int PSimDynamicOptimizationSolver::OptimizerSystem::objectiveFunc(
        const SimTK::Vector& parameters, bool new_parameters, SimTK::Real& f)
    const
{
    // Initialize model and state.
    // ===========================
    Model model = m_pstool.get_base_model();
    if (m_pstool.get_visualize()) model.setUseVisualizer(true);

    // Add PSimGoal's to Model as ModelComponents.
    const auto objectives = m_pstool.addGoalsToModel(model);

    // Mechanism to record the trajectory of successful states.
    // --------------------------------------------------------
    StatesCollector* statesCollector = new StatesCollector();
    statesCollector->setName("statesCollector");
    model.addAnalysis(statesCollector);
    SimTK::State& state = model.initSystem();

    // Update model and initial state with parameters.
    // ===============================================
    const PSimParameterValueSet pvalset(
            m_pstool.createParameterValueSet(parameters));
    m_pstool.applyParameters(pvalset, model, state);

    // Simulate.
    // =========
    SimTK::RungeKuttaMersonIntegrator integrator(model.getSystem());
    OpenSim::Manager manager(model, integrator);
    manager.setWriteToStorage(false);
    manager.setInitialTime(m_pstool.get_initial_time());
    manager.setFinalTime(m_pstool.get_final_time());
    manager.integrate(state);

    // Add in objective terms.
    // =======================
    const StateTrajectory& states = statesCollector->getStateTrajectory();
    f = m_pstool.evaluateGoals(objectives, pvalset, model, states);

    return 0;
}
