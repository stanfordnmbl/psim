#include "OptimizerSystem.h"
#include "StatesCollector.h"
#include "StateTrajectory.h"
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
    Model model = m_pstool.get_base_model();
    if (m_pstool.get_visualize()) model.setUseVisualizer(true);

    // Add Objective's to Model as ModelComponents.
    const auto objectives = m_pstool.addObjectivesToModel(model);

    // Mechanism to record the trajectory of successful states.
    // --------------------------------------------------------
    StatesCollector* statesCollector = new StatesCollector();
    statesCollector->setName("statesCollector");
    model.addAnalysis(statesCollector);
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
    manager.setWriteToStorage(false);
    manager.setInitialTime(m_pstool.get_initial_time());
    manager.setFinalTime(m_pstool.get_final_time());
    manager.integrate(state);

    // Add in objective terms.
    // =======================
    const StateTrajectory& states = statesCollector->getStateTrajectory();
    f = m_pstool.evaluateObjectives(objectives, pvalset, model, states);

    return 0;
}

} // namespace PSim
