/* -------------------------------------------------------------------------- *
 *                    OpenSim:  Maximum.cpp                                   *
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

#include "Maximum.h"

using namespace OpenSim;

// This Measure returns a derivative only at the Acceleration stage.
// Informed by Probe.
template <class T>
class InputMeasure : public SimTK::Measure_<T> {
public:
    SimTK_MEASURE_HANDLE_PREAMBLE(InputMeasure, SimTK::Measure_<T>);
    InputMeasure(SimTK::Subsystem& sub, const Maximum& max)
        : SimTK::Measure(sub, new Implementation(max),
                SimTK::AbstractMeasure::SetHandle()) {}
    SimTK_MEASURE_HANDLE_POSTSCRIPT(InputMeasure, SimTK::Measure_<T>);
};
template <class T>
class InputMeasure<T>::Implementation
        : public SimTK::Measure_<T>::Implementation {
public:
    Implementation(const Maximum& max)
        : SimTK::Measure_<T>::Implementation(1), m_max(max) {}

    // Implementations of virtual methods.
    Implementation* cloneVirtual() const override
    {   return new Implementation(*this); }
    int getNumTimeDerivativesVirtual() const override { return 0; }

    SimTK::Stage getDependsOnStageVirtual(int order) const override
    {   return m_max.getDependsOnStage(); }

    void calcCachedValueVirtual(const SimTK::State& s, int derivOrder,
            T& value) const override {
        SimTK_ASSERT1_ALWAYS(derivOrder==0,
                "OpenSim::InputMeasure<T>::Implementation::"
                "calcCachedValueVirtual(): "
                "derivOrder %d seen but only 0 allowed.", derivOrder);
        value = m_max.getInput(s);
    }
private:
    const Maximum& m_max;
};

void Maximum::addToSystem(SimTK::MultibodySystem& system) const
{
    Super::addToSystem(system);

    InputMeasure<SimTK::Real> inputMeasure(system, *this);

    const_cast<Maximum*>(this)->m_measure =
        SimTK::Measure::Maximum(system, inputMeasure);

}
