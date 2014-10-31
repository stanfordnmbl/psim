#ifndef OPENSIM_PSIM_SOLVER_H_
#define OPENSIM_PSIM_SOLVER_H_

#include "PSimParameterValue.h"

namespace OpenSim {

class PSimTool;

/** A solver finds the values of the Parameter's that minimize the Goal's
 * values for a given Model.
 */
class PSimSolver : public Object
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSimSolver, Object);

public:
    PSimParameterValueSet solve(const PSimTool& tool) const {
        return extendSolve(tool);
    }
private:
    virtual PSimParameterValueSet extendSolve(const PSimTool& tool) const = 0;
};

} // namespace OpenSim

#endif // OPENSIM_PSIM_SOLVER_H_
