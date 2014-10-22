#ifndef PSIM_OPTIMIZER_SYSTEM_H_
#define PSIM_OPTIMIZER_SYSTEM_H_

#include <Simbody.h>

namespace PSim {

class Tool;

/**
 * The system being optimized by the PSim::Tool. Given parameters from the
 * optimizer, the objective function updates the model and initial state and
 * runs a simulation. Then, the objective function evaluates the PSim::Tool's
 * Objective terms, and sends the resulting objective function value back to
 * the optimizer.
 */
class OptimizerSystem : public SimTK::OptimizerSystem
{
public:
    OptimizerSystem(const Tool& tool);
    int objectiveFunc(const SimTK::Vector& parameters, bool new_parameters,
            SimTK::Real& f) const override;

private:
    const Tool& m_pstool;
};

} // namespace PSim

#endif // PSIM_OPTIMIZER_SYSTEM_H_
