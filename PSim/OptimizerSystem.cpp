#include "OptimizerSystem.h"
#include "Tool.h"

#include <OpenSim/Simulation/Manager/Manager.h>

namespace PSim {

OptimizerSystem::OptimizerSystem(const Tool& tool)
        : SimTK::OptimizerSystem(tool.numOptimizerParameters()), m_pstool(tool)
{
}

int OptimizerSystem::objectiveFunc(const SimTK::Vector& parameters,
        bool new_parameters, SimTK::Real& f) const
{
    // Initialize model and state.
    // ===========================
    Model model(m_pstool.get_model());
    SimTK::State& state = model.initSystem();

    // Update model and initial state with parameters.
    // ===============================================
    const ParameterValueSet pvalset(
            m_pstool.createParameterValueSet(parameters));
    m_pstool.applyParameters(pvalset, model, state);

    // Simulate.
    // =========
    SimTK::RungeKuttaMersonIntegrator integrator(model.getSystem());
    OpenSim::Manager manager(model, integrator);
    manager.setInitialTime(m_pstool.get_initial_time());
    manager.setFinalTime(m_pstool.get_final_time());
    manager.integrate(state);

    // Add in objective terms.
    // =======================
    // TODO IntegratingObjectives that operate like probes?
    f = m_pstool.evaluateObjectives(pvalset, model, state);

    return 0;
}

} // namespace PSim
