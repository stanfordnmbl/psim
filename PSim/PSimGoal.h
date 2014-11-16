#ifndef OPENSIM_PSIM_GOAL_H_
#define OPENSIM_PSIM_GOAL_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimGoal.h                                    *
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

#include "PSimParameterValue.h"
#include "StateTrajectory.h"

#include <OpenSim/Simulation/Model/ModelComponent.h>

#include "osimPSimDLL.h"

namespace OpenSim {

/** A goal that the simulation should achieve, such as maximum distance,
 * minimum energy expenditure, etc. The goal is expressed as a scalar
 * that is added into the objective function for an optimization, and is thus
 * a quantity that is minimized (as opposed to maximized).
 */
class OSIMPSIM_API PSimGoal : public ModelComponent
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSimGoal, ModelComponent);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(enabled, bool,
        "If true, this term is added into the objective function.");
    OpenSim_DECLARE_PROPERTY(weight, double,
        "Coefficient of this term in the sum across objectives.");
    /// @}

    PSimGoal();

    /// Evaluates this goal. If the goal determines that the simulation
    /// is unreasonable in some way and should not even be considered, then this
    /// method returns NaN.
    SimTK::Real evaluate(const PSimParameterValueSet& pvalset,
            const StateTrajectory& states) const {
        // TODO Check if enabled here.
        return extendEvaluate(pvalset, states);
    }

private:

    /// Concrete goals must implement this method. Implementors must not
    /// account for the goal's weight. If the simulation is unreasonable,
    /// NaN, and the subsequent goals will not be evaluated. The solver decides
    /// what to do in this case.
    virtual SimTK::Real extendEvaluate(const PSimParameterValueSet& pvalset,
            const StateTrajectory& states) const = 0;

    void constructProperties();

};

/**
TerminalGoal
evaluate(const PSimParameterValueSet& pvalset, const SimTK::State& finalState);
        
GeneralGoal
evaluate(const PSimParameterValueSet& pvalset, const StateTrajectory& states)

IntegrandGoal
evaluate(const PSimParameterValueSet& pvalset, const SimTK::State& s)

Are objectives necessarily evaluated at the "end"? Do I need to have a simulation completed? sometimes a simulation is not performed. 

FinalTime

AverageGoal

want to evaluate a terminalgoal at the end. evaluate general goal at the end, but internally it may need to do things throughout the simulation, integrand goals: i would loop through the states given to me by the optimizer and evaluate them.

goals can't assume that the optimizer will be performing a forward integration, and can't know anything about "when" the goal is evaluated.

the solver may want to evaluate goals differently (evaluate terminal goals in one way, integrand goals in a different way).

the goal writer can't assume anything about how the goals will be evaluated.

class Goal {

};

class TerminalGoal : Goal {
    evaluate(pvalset, finalState);
};

// certain solvers may add state resources to integrate these.
class IntegratedGoal : Goal {
    evaluate(pvalset, s) {
    }
    // TODO property to average the value.
};

// given a whole state trajectory.
class GeneralGoal : Goal {
};

evaluateIntegratedGoals()

evaluateTerminalGoals()

evaluateGoals()

// MPC solver: doesn't have any outer objectives: MPC is a controller.
//        (s.getQ() - data.getQ()) + (s.getcontrols())
//            MPC parameters are not the same as my parameters...

/// An PSimGoal whose value depends on a Probe in the Model.
/// class ProbeObjective : public PSimGoal
/// {
///     void constructConnectors() {
///         constructConnector<Probe>("probe");
///     }
/// };
*/

} // namespace OpenSim

#endif // OPENSIM_PSIM_GOAL_H_
