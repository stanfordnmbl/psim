#ifndef OPENSIM_PSIM_SOLVER_H_
#define OPENSIM_PSIM_SOLVER_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimSolver.h                                  *
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

#include "osimPSimDLL.h"

namespace OpenSim {

class PSimTool;

/** A solver finds the values of the Parameter's that minimize the Goal's
 * values for a given Model.
 */
class OSIMPSIM_API PSimSolver : public Object
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
