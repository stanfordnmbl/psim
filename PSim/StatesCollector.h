#ifndef OPENSIM_STATES_COLLECTOR_H_
#define OPENSIM_STATES_COLLECTOR_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  StatesCollector.h                             *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2014 Stanford University and the Authors                *
 * Author(s): Chris Dembia                                                    *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "StateTrajectory.h"

#include <OpenSim/Simulation/Model/Analysis.h>

#include "osimPSimDLL.h"

namespace OpenSim {

/** This collects the significant states from a simulation.
* This differs from a StatesReporter because this does not
* print the states to a Storage file. The states are stored as a
* StateTrajectory.
*/
class OSIMPSIM_API StatesCollector : public Analysis
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
