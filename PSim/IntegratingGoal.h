#ifndef OPENSIM_PSIM_INTEGRATING_OBJECTIVE_H_
#define OPENSIM_PSIM_INTEGRATING_OBJECTIVE_H_

#include "PSimGoal.h"
#include "StateTrajectory.h"
#include <OpenSim/Simulation/Model/Model.h>

namespace OpenSim {

/// This objective value is obtained by integrating a quantity over the
/// duration of the simulation.
class IntegratingGoal : public PSimGoal
{
OpenSim_DECLARE_ABSTRACT_OBJECT(IntegratingGoal, PSimGoal);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(initial_condition, double,
        "Initial condition for integration.");
    /// @}
    
    IntegratingGoal() {
        constructProperties();
    }

    SimTK::Real evaluate(const PSimParameterValueSet & pvalset,
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

} // namespace OpenSim

#endif // OPENSIM_PSIM_INTEGRATING_OBJECTIVE_H_
