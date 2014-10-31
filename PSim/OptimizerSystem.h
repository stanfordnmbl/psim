#ifndef OPENSIM_PSIM_OPTIMIZER_SYSTEM_H_
#define OPENSIM_PSIM_OPTIMIZER_SYSTEM_H_

#include <Simbody.h>

namespace OpenSim {

class PSimTool;

/**
 * The system being optimized by the OpenSim::PSimTool. Given parameters from the
 * optimizer, the objective function updates the model and initial state and
 * runs a simulation. Then, the objective function evaluates the OpenSim::PSimTool's
 * PSimGoal terms, and sends the resulting objective function value back to
 * the optimizer.
 */
class OptimizerSystem : public SimTK::OptimizerSystem
{
public:
    OptimizerSystem(const PSimTool & tool);
    int objectiveFunc(const SimTK::Vector& parameters, bool new_parameters,
            SimTK::Real& f) const override;

private:
    const PSimTool & m_pstool;
};

} // namespace OpenSim

#endif // OPENSIM_PSIM_OPTIMIZER_SYSTEM_H_
