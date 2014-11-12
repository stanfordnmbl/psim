#ifndef OPENSIM_PSIM_CONCRETE_PARAMETERS_H_
#define OPENSIM_PSIM_CONCRETE_PARAMETERS_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  ConcreteParameters.h                          *
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

#include "PSimParameter.h"

#include "osimPSimDLL.h"

namespace OpenSim {

// TODO add statevariable. parameter.

/** The value of a coordinate (translation or rotation) at the start
 * of the motion.
 */
class OSIMPSIM_API PSimCoordInitialValueParameter : public PSimParameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimCoordInitialValueParameter, PSimParameter);
public:
    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(coordinate_name, std::string,
            "Full path name of the coordinate.");
    /// @}

    PSimCoordInitialValueParameter() { constructProperties(); }

    void extendApplyToInitialState(const double param,
            const Model& model, SimTK::State& initState) const override {
        const auto& comp = model.getComponent(get_coordinate_name());
        const auto* coord = static_cast<const Coordinate*>(&comp);
        if (!coord->getLocked(initState)) {
            coord->setValue(initState, param);
        }
    }

private:
    void constructProperties() { constructProperty_coordinate_name(""); }

};

/** The speed for a coordinate at the start of a motion.
 */
class OSIMPSIM_API PSimCoordInitialSpeedParameter : public PSimParameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimCoordInitialSpeedParameter, PSimParameter);
public:
    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(coordinate_name, std::string,
            "Full path name of the coordinate.");
    /// @}

    PSimCoordInitialSpeedParameter() { constructProperties(); }

    void extendApplyToInitialState(const double param,
            const Model& model, SimTK::State& initState) const override {
        const auto& comp = model.getComponent(get_coordinate_name());
        const auto* coord = static_cast<const Coordinate*>(&comp);
        if (!coord->getLocked(initState)) {
            coord->setSpeedValue(initState, param);
        }
    }

private:
    void constructProperties() { constructProperty_coordinate_name(""); }

};

} // namespace OpenSim

#endif // OPENSIM_PSIM_CONCRETE_PARAMETERS_H_
