
#include "StatesCollector.h"

#include <OpenSim/Simulation/Model/Model.h>

namespace OpenSim {

int StatesCollector::end(SimTK::State& s)
{
    // TODO should only realize to Dynamics? Or allow components to use their
    // realizeReport?
    const SimTK::MultibodySystem& mbs = _model->getMultibodySystem();
    for (const SimTK::State& state : m_states) {
        mbs.realize(state, SimTK::Stage::Report);
    }
    return 0;
}

} // namespace OpenSim
