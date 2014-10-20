#ifndef PSIM_OBJECTIVE_H_
#define PSIM_OBJECTIVE_H_

#include <OpenSim/Common/Object.h>
#include <OpenSim/Simulation/Model/Model.h>
#include "ParameterValue.h"

using namespace OpenSim;

namespace PSim {

// TODO use lepton!!!

class Objective : public OpenSim::Object
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSim::Objective, Object);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(enabled, bool,
        "This term is added into the objective function.");
    OpenSim_DECLARE_PROPERTY(weight, double,
        "Coefficient of this term in the sum across objectives.");
    /// @}

    Objective();

    virtual SimTK::Real evaluate(const ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const = 0;

private:
    void constructProperties();

};

} // namespace PSim

#endif // PSIM_PARAMETER_H_
