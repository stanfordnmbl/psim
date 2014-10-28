#ifndef PSIM_INTEGRATING_OBJECTIVE_H_
#define PSIM_INTEGRATING_OBJECTIVE_H_

#include "Objective.h"
#include "StateTrajectory.h"
#include <OpenSim/Simulation/Model/Model.h>

using namespace OpenSim;

namespace PSim {

/// This objective value is obtained by integrating a quantity over the
/// duration of the simulation.
class IntegratingObjective : public Objective
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSim::IntegratingObjective, Objective);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(initial_condition, double,
        "Initial condition for integration.");
    /// @}
    
    IntegratingObjective() {
        constructProperties();
    }

    SimTK::Real evaluate(const ParameterValueSet& pvalset,
            const Model& model,
            const StateTrajectory& states) const override final
    {
        return m_integrateMeasure.getValue(states.back());
    }

    /// The quantity to integrate. This is invoked throughout the simulation.
    virtual SimTK::Real derivative(const SimTK::State& s) const = 0;

private:

    void constructProperties() override {
        constructProperty_initial_condition(0);
    }

    void addToSystem(SimTK::MultibodySystem& system) const override;

    SimTK::Measure m_integrateMeasure;
 
};

} // namespace PSim

#endif // PSIM_INTEGRATING_OBJECTIVE_H_
