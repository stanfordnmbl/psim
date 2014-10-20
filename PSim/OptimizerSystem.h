#ifndef PSIM_OPTIMIZER_SYSTEM_H_
#define PSIM_OPTIMIZER_SYSTEM_H_

#include <Simbody.h>

namespace PSim {

class Tool;

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
