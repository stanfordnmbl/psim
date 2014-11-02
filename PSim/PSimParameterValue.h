#ifndef OPENSIM_PSIM_PARAMETER_VALUE_H_
#define OPENSIM_PSIM_PARAMETER_VALUE_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimParameterValue.h                          *
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

#include <OpenSim/Common/Object.h>
#include <OpenSim/Common/Set.h>

#include "osimPSimDLL.h"

using namespace OpenSim;

namespace OpenSim {
/**
 * Stores the value of a parameter. This value is *not* scaled.
 * That is, it could be used directly in the model, and is not suited to be
 * used directly in the optimization. The name of the PSimParameterValue object
 * must match the name of a PSimParameter; otherwise, the value might be ignored.
 * TODO help detect unused values.
 */
class OSIMPSIM_API PSimParameterValue : public Object
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimParameterValue, Object);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(value, double, "Value of the PSimParameter.")
    /// @}

    PSimParameterValue()
    {
        constructProperties();
    }

private:
    void constructProperties()
    {
        constructProperty_value(0);
    }

};

// TODO do I need "typename"?
// TODO for serialization, make this a class? or just register it?
typedef typename OpenSim::Set<OpenSim::PSimParameterValue> PSimParameterValueSet;

} // namespace OpenSim
#endif // OPENSIM_PSIM_PARAMETER_VALUE_H_
