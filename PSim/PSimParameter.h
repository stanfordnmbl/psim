#ifndef OPENSIM_PSIM_PARAMETER_H_
#define OPENSIM_PSIM_PARAMETER_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimParameter.h                               *
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
#include <OpenSim/Simulation/Model/Model.h>

#include "osimPSimDLL.h"

// Parameters should not be passed a map, for backwards compatibility. The
// parameter should only apply itself for parameters given to it. If there are
// hard-coded parameter names, then a new version of a class may hard-code in a
// new parameter name. Then, old XML files which don't provide a parameter
// object for that hardcoded name will cause the code to not be able to find
// the parameeter. Thus, individual parameters only apply themselves if
// instructed by the tool, etc.

namespace OpenSim {

// TODO template<int N>
/**
* A description of how an optimizer parameter affects a Model and its
* SimTK::State.
* The lower_opt property is the value used in the optimizer to represent
* the lower_limit value. Likewise for upper_opt. These properties
* are used for normalizing the parameter values so that the optimizer
* can operate with values that have similar ranges.
* For example, consider an upper_limit of 10 and upper_opt of 2,
* with lower_limit and lower_opt as 0.
* In this case, the optimizer is given an upper limit of 5.
* A parameter value of 4 would be given to the optimizer as 2.
* By default, the parameters are not normalized when sent to the optimizer.
* For the parameter to remain in the range [0, 1],
* set the lower_opt to lower_limit, and upper_opt to upper_limit.
* This affects how explorative the optimizer is for this parameter.
* For a fixed standard deviation (CMAES step size, sigma),
* a larger difference (upper_opt - lower_opt) causes more exploration.
*/
class OSIMPSIM_API PSimParameter : public Object
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimParameter, Object);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(optimize, bool,
            "Optimize this parameter (false means default value is used).");
    OpenSim_DECLARE_PROPERTY(lower_limit, double,
            "Lower limit for parameter; for optimizer bounds. Default: -Inf");
    OpenSim_DECLARE_PROPERTY(upper_limit, double,
            "Upper limit for parameter; for optimizer bounds. Default: Inf");
    OpenSim_DECLARE_PROPERTY(lower_opt, double,
            "The value of lower_limit used in the optimizer. Default: 0");
    OpenSim_DECLARE_PROPERTY(upper_opt, double,
            "The value of upper_limit used in the optimizer. Default: 1");
    OpenSim_DECLARE_PROPERTY(default_value, double,
            "Used if optimize=false; & default initial guess. Default: 0.");
    /// @}

    PSimParameter();

    /// The number of scalar parameters represented by this object.
    unsigned int numScalarParameters() const { return 1; }

    /// @name Applying parameter values to a model or state.
    /// These are the core methods of this class.
    /// The way in which these methods are used by the PSimSolver depends on
    /// the solver. Most parameters only apply to one of the three categories
    /// represented by these three methods.
    /// @{

    /// Apply the parameter value to the given writeable model.
    void applyToModel(const double param,
            Model& model) const
    { return extendApplyToModel(param, model); }

    /// Apply the parameter value to the given writeable initial state.
    void applyToInitialState(const double param,
            const Model& model, SimTK::State& initState) const
    { return extendApplyToInitialState(param, model, initState); }

    /// Apply the parameter value to the given state's cache (e.g, controls).
    void applyToStateCache(const double param,
            const Model& model, const SimTK::State& s) const
    { return extendApplyToStateCache(param, model, s); }
    /// @}

    /// This is used to compute a value that can be sent to
    /// to the optimizer (e.g., for initial parameters).
    double normalized(double param) const;

    /// This is used to convert a value coming from an optimizer into one that
    /// can be stored somewhere in the Model or State.
    double unnormalized(double normalizedParam) const;

private:

    /// @name Parameter developer interface.
    /// Implementors of new Parameters must override these methods.
    /// @{
    virtual void extendApplyToModel(const double param,
            Model& model) const {}
    virtual void extendApplyToInitialState(const double param,
            const Model& model, SimTK::State& initState) const {}
    virtual void extendApplyToStateCache(const double param,
            const Model& model, const SimTK::State& s) const {}
    /// @}
     
    void constructProperties();

};

} // namespace OpenSim

/*
class OSIMPSIM_API PSimParameter : public Object {

    OpenSim_DECLARE_PROPERTY(optimize, bool,
            "Optimize this parameter (false means default value is used).");
    OpenSim_DECLARE_PROPERTY(lower_limit, double,
            "Lower limit for parameter; for optimizer bounds. Default: -Inf");
    OpenSim_DECLARE_PROPERTY(upper_limit, double,
            "Upper limit for parameter; for optimizer bounds. Default: Inf");
    OpenSim_DECLARE_PROPERTY(lower_opt, double,
            "The value of lower_limit used in the optimizer. Default: 0");
    OpenSim_DECLARE_PROPERTY(upper_opt, double,
            "The value of upper_limit used in the optimizer. Default: 1");
    OpenSim_DECLARE_PROPERTY(default_value, double,
            "Used if optimize=false. Default initial guess. Default: 0.");

    PSimParameter();

    unsigned int numScalarParameters() const { return 1; }

    double normalized(double param) const;
    double unnormalized(double normalizedParam) const;

private:
    void constructProperties();
};

class OSIMPSIM_API PSimDesigner : public Object {
public:
    OpenSim_DECLARE_LIST_PROPERTY(parameters, PSimParameter);

    void applyToModel(const PSimParameterMap& params,
            Model& model) const
    { return extendApplyToModel(param, model); }

private:

    virtual void extendApplyToModel(const PSimParameterMap& params,
            Model& model) const {}

};
*/

#endif // OPENSIM_PSIM_PARAMETER_H_
