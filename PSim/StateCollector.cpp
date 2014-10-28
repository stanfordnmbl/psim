
#include "StatesCollector.h"

#include <OpenSim/Simulation/Model/Model.h>

namespace PSim {

int StatesCollector::end(SimTK::State& s)
{
    // We already have the last state in the trajectory, so we do not need
    // to append it to the trajectory. But, for these states to be useful,
    // we realize them.
    // TODO should only realize to Dynamics? Or allow components to use their
    // realizeReport?
    const SimTK::MultibodySystem& mbs = _model->getMultibodySystem();
    for (const SimTK::State& state : m_states) {
        mbs.realize(state, SimTK::Stage::Report);
    }
    return 0;
}

} // namespace PSim
