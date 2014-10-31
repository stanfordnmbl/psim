#ifndef OPENSIM_STATES_COLLECTOR_H_
#define OPENSIM_STATES_COLLECTOR_H_

#include "StateTrajectory.h"

#include <OpenSim/Simulation/Model/Analysis.h>

namespace OpenSim {

/** This collects the significant states from a simulation.
* This differs from a StatesReporter because this does not
* print the states to a Storage file. The states are stored as a
* StateTrajectory.
*/
class StatesCollector : public Analysis
{
OpenSim_DECLARE_CONCRETE_OBJECT(StatesCollector, Analysis);
public:

    StatesCollector() {}

    int begin(SimTK::State& s) override {
        m_states.push_back(s);
        return 0;
    }
    int step(const SimTK::State& s, int stepNumber) override {
        m_states.push_back(s);
        return 0;
    }

    /// We already have the last state in the trajectory, so we do not need
    /// to append it to the trajectory. But, for these states to be useful,
    /// we realize them.
    int end(SimTK::State& s) override;

    const StateTrajectory& getStateTrajectory() const
    {   return m_states; }

private:

    StateTrajectory m_states;

};

} // namespace OpenSim

#endif // OPENSIM_STATES_COLLECTOR_H_
