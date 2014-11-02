#ifndef OPENSIM_MAXIMUM_H_
#define OPENSIM_MAXIMUM_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  Maximum.h                                     *
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

#include <OpenSim/Common/Component.h>

#include "osimPSimDLL.h"

namespace OpenSim {

class OSIMPSIM_API Maximum : public Component
{
OpenSim_DECLARE_ABSTRACT_OBJECT(Maximum, Component);
public:

    Maximum() {
        constructInfrastructure();
    }

    double maximum(const SimTK::State& s) const {
        return m_measure.getValue(s);
    }

    double getInput(const SimTK::State& s) const {
        return getInputVirtual(s);
    }

    SimTK::Stage getDependsOnStage() const {
        return getDependsOnStageVirtual();
    }

protected:

    virtual double getInputVirtual(const SimTK::State& s) const = 0;
    virtual SimTK::Stage getDependsOnStageVirtual() const = 0;

private:

    /*
    void constructInputs() override {
        constructInput<double>("input", SimTK::Stage::Model);
    }

    void constructOutputs() override {
        constructOutput<double>("maximum", &Maximum::maximum,
                SimTK::Stage::Velocity);
    }
    */

    void addToSystem(SimTK::MultibodySystem& system) const override;

    SimTK::Measure m_measure;

};

} // namespace OpenSim

#endif // OPENSIM_MAXIMUM_H_
