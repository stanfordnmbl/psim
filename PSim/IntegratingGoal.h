#ifndef OPENSIM_PSIM_INTEGRATING_OBJECTIVE_H_
#define OPENSIM_PSIM_INTEGRATING_OBJECTIVE_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  IntegratingGoal.h                             *
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

#include "PSimGoal.h"
#include "StateTrajectory.h"

#include <OpenSim/Simulation/Model/Model.h>

#include "osimPSimDLL.h"

namespace OpenSim {

/// This objective value is obtained by integrating a quantity over the
/// duration of the simulation.
class OSIMPSIM_API IntegratingGoal : public PSimGoal
{
OpenSim_DECLARE_ABSTRACT_OBJECT(IntegratingGoal, PSimGoal);
public:

    /// The quantity to integrate.
    virtual SimTK::Real integrand(const SimTK::State& s) const = 0;

private:

    SimTK::Real extendEvaluate(const PSimParameterValueSet& pvalset,
            const StateTrajectory& states) const override final {
        return m_integrateMeasure.getValue(states.back());
    }

    void extendAddToSystem(SimTK::MultibodySystem& system) const override;

    SimTK::Measure m_integrateMeasure;
 
};

} // namespace OpenSim

#endif // OPENSIM_PSIM_INTEGRATING_OBJECTIVE_H_
