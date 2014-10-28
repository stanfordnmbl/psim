#ifndef PSIM_OBJECTIVE_H_
#define PSIM_OBJECTIVE_H_

#include "ParameterValue.h"
#include "StateTrajectory.h"

#include <OpenSim/Simulation/Model/Model.h>

using namespace OpenSim;

namespace PSim {

class Objective : public OpenSim::ModelComponent
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSim::Objective, OpenSim::ModelComponent);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(enabled, bool,
        "If true, this term is added into the objective function.");
    OpenSim_DECLARE_PROPERTY(weight, double,
        "Coefficient of this term in the sum across objectives.");
    /// @}

    Objective();

    /// Evaluate this objective. This quantity is multiplied by the weight
    /// property of this Objective, and is then added to the objective
    /// function.
    virtual SimTK::Real evaluate(const ParameterValueSet& pvalset,
            const Model& model,
            const StateTrajectory& states) const = 0;

private:
    void constructProperties();

};

/// An Objective whose value depends on a Probe in the Model.
/// class ProbeObjective : public Objective
/// {
///     void constructConnectors() {
///         constructConnector<Probe>("probe");
///     }
/// };

} // namespace PSim

#endif // PSIM_PARAMETER_H_
