/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimParameter.cpp                             *
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

#include <OpenSim/Common/Array.h>

using namespace OpenSim;

PSimParameter::PSimParameter() {
    constructProperties();
}

void PSimParameter::constructProperties() {
    constructProperty_apply(true);
    constructProperty_optimize(true);

    constructProperty_lower_limit(-SimTK::Infinity);
    constructProperty_upper_limit(SimTK::Infinity);
    constructProperty_lower_opt(0);
    constructProperty_upper_opt(1);

    constructProperty_default_value(0);
}

double PSimParameter::normalized(double param) const {
    return (param - get_lower_opt()) / (get_upper_opt() - get_lower_opt());
}

double PSimParameter::unnormalized(double normalizedParam) const {
    return normalizedParam * (get_upper_opt() - get_lower_opt()) +
        get_lower_opt();
}
