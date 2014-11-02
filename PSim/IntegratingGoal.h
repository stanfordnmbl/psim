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

    /// The quantity to integrate.
    virtual SimTK::Real integrand(const SimTK::State& s) const = 0;

private:

    SimTK::Real extendEvaluate(const PSimParameterValueSet& pvalset,
            const StateTrajectory& states) const override final {
        return m_integrateMeasure.getValue(states.back());
    }

    void addToSystem(SimTK::MultibodySystem& system) const override;

    SimTK::Measure m_integrateMeasure;
 
};

} // namespace OpenSim

#endif // OPENSIM_PSIM_INTEGRATING_OBJECTIVE_H_
