#ifndef PSIM_STATES_COLLECTOR_H_
#define PSIM_STATES_COLLECTOR_H_

#include "StateTrajectory.h"

#include <OpenSim/Simulation/Model/Analysis.h>

namespace PSim {

class StatesCollector : public OpenSim::Analysis
{
OpenSim_DECLARE_CONCRETE_OBJECT(StatesCollector, OpenSim::Analysis);
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
    int end(SimTK::State& s) override;

    const StateTrajectory& getStateTrajectory() const
    {   return m_states; }

private:

    StateTrajectory m_states;

};

} // namespace PSim

#endif // PSIM_STATES_COLLECTOR_H_
