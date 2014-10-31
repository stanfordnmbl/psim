#ifndef OPENSIM_PSIM_GOAL_H_
#define OPENSIM_PSIM_GOAL_H_

#include "PSimParameterValue.h"
#include "StateTrajectory.h"

#include <OpenSim/Simulation/Model/ModelComponent.h>

namespace OpenSim {

/** A goal that the simulation should achieve, such as maximum distance,
 * minimum energy expenditure, etc. The goal is expressed as a scalar
 * that is added into the objective function for an optimization.
 */
class PSimGoal : public ModelComponent
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSimGoal, ModelComponent);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(enabled, bool,
        "If true, this term is added into the objective function.");
    OpenSim_DECLARE_PROPERTY(weight, double,
        "Coefficient of this term in the sum across objectives.");
    /// @}

    PSimGoal();

    /// Evaluate this objective. This quantity is multiplied by the weight
    /// property of this PSimGoal, and is then added to the objective
    /// function.
    virtual SimTK::Real evaluate(const PSimParameterValueSet & pvalset,
            const Model& model,
            const StateTrajectory& states) const = 0;

private:
    void constructProperties();

};

/// An PSimGoal whose value depends on a Probe in the Model.
/// class ProbeObjective : public PSimGoal
/// {
///     void constructConnectors() {
///         constructConnector<Probe>("probe");
///     }
/// };

} // namespace OpenSim

#endif // OPENSIM_PSIM_GOAL_H_
