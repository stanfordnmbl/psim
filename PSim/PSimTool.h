#ifndef OPENSIM_PSIM_TOOL_H_
#define OPENSIM_PSIM_TOOL_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimTool.h                                    *
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

#include "PSimGoal.h"
#include "PSimParameter.h"
#include "PSimParameterValue.h"
#include "PSimSolver.h"
#include "StateTrajectory.h"

#include "osimPSimDLL.h"

// TODO hierarchy of different types of goals. (terminal, integrating)
// TODO output goals values to file
// TODO clean up the examples.
// TODO replicate the optimal control example of flight from ACADO.
// TODO even if we use the model file name instead, we should store one copy
// locally and just keep making copies of that, instead of reading from the
// file each time (avoid parsing).
// TODO allow replaying a simulation, not running an optimization,
// with a given PSimParameterValueSet. (model and initial state).
// TODO create application.
// TODO a probe objective, or an integratingobjective. (spring osc. freq).
// TODO cmaes
// TODO event handling to opensim-core.

// TODO Ajay talked about pieces/components that could be moved around and used
// anywhere. To this end:
// TODO create an Objective class and a Design/Parameters class. Create another
// layer of separation. Those classes are then responsible for performing
// actions across all objectives or parameters, and the related methods can be
// moved out of the tool itself. The tool simply connects Parameters, and
// Objectives to a Solver. Ajay is saying a model controller could contain its
// own parameters and objectives and solver (MPC). Objectives should be usable
// in general. They should export an interface that says what the interface
// would do, and it should make minimal assumptions about how it's being used.
//
// TODO types of problems: optimizing a model, vs the controller contains an
// optimization.
//
// optimizing a model and simulation:
//  - direct collocation
//  - CMAES
//  - Boyd's thing.
// Parameterization: properties vs control values are substantially different.
//      Could use a control set controller.
//      Applying control parameters might mean altering the state.
// TODO when optimizing a model, how do we apply control parameters, which could be 92 muscles * 100 time steps?


// 1. ModelParameters, InitialStateParameters, StateCacheParameters
// 2. hierarchy of goals
// 3. convert predictivesim code to this framework.

// create application
// convert predictivesim
//      create new solver
//      create parameters & objectives.

namespace OpenSim {

/** A framework for performing predictive simulations. We optimize a model's
 * ability to carry out a specific task by varying certain PSimParameter's of
 * the model and its state, and evaluating the performance of the task
 * via PSimGoal's. You can define your own PSimParameter's, and choose how
 * they affect your model and/or state. Then, you define your PSimGoal's (e.g.
 * maximum jump height).
 */
class OSIMPSIM_API PSimTool : public Object
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimTool, Object);
public:

    /// @name Property declarations
    /// @{
    // TODO results_dir
    OpenSim_DECLARE_PROPERTY(base_model, Model,
            "Path to the base (unmodified) model file (.osim) to optimize.");
    OpenSim_DECLARE_PROPERTY(solver, PSimSolver,
            "The algorithm that alters parameters to achieve goals.");
    OpenSim_DECLARE_PROPERTY(results_dir, std::string,
            "Directory in which results files will be printed.");
    OpenSim_DECLARE_PROPERTY(initial_time, double,
            "The time at which all the simulations start (seconds).");
    OpenSim_DECLARE_PROPERTY(final_time, double,
            "The time at which the simulations end (seconds).");
    // TODO might belong in the solver; may not make sense for some solvers.
    OpenSim_DECLARE_PROPERTY(visualize, bool,
            "During solving, show the motion using the simbody-visualizer.");
    OpenSim_DECLARE_LIST_PROPERTY(parameters, PSimParameter,
            "Optimization parameters.");
    OpenSim_DECLARE_LIST_PROPERTY(goals, PSimGoal,
            "Terms of the optimization objective function. "
            "Only enabled goals are evaluated.");
    OpenSim_DECLARE_PROPERTY(initial_guess, PSimParameterValueSet,
            "Values of parameters used in initial guess in the optimization "
            "(unnormalized). For parameters that are left out in this object, "
            "the parameter's default value is used. If you supply values "
            "for parameters that are not set to be optimized, those values "
            "are ignored.");
    // OpenSim_DECLARE_LIST_PROPERTY(plugin_libraries, std::string,
    //      "List of dynamic libraries (dll's, so's) (OpenSim plugins) to "
    //      "load before running. Separate multiple libraries with spaces.");
    /// @}

    PSimTool();

    /// Construct a PSimTool from an XML file.
    PSimTool(const std::string& fileName);

    // TODO print out the new model? that's what is optimized...
    /// Perform the predictive simulation optimization.
    /// @returns solution to the optimization.
    PSimParameterValueSet run() const;

    /// @name Sending parameters to and from the Solver.
    /// These methods implicitly define how optimizer parameters map to and
    /// from tool parameters.
    /// We make a distinction between "tool" and "optimizer" parameters:
    /// tool parameters are those from the <tt>parameters</tt> property; these
    /// can be set to not be optimized and are not normalized.
    /// Optimizer parameters are normalized and don't contain tool
    /// parameters that are not being optimized.
    /// These methods handle the mapping between tool and optimizer
    /// parameters, and are to be called by the PSimSolver.
    /// @{

    /// The sum of the number of scalar parameters across the
    /// OpenSim::PSimParameter's that are set to be optimized.
    unsigned int numOptimizerParameters() const;

    /// Modify the model according to the given parameters. For parameters
    /// that are not being optimized, the default value of the parameter is
    /// applied.
    /// @param[in] paramValues This comes from the optimizer.
    /// @param[in,out] model The model to modify with these parameters.
    void applyParametersToModel(const PSimParameterValueSet& paramValues,
         Model& model) const;

    /// Modify the initial state according to the given parameters. This should
    /// be called once the model will no longer be modified.
    /// @param[in] paramValues This comes from the optimizer.
    /// @param[in] model Used to access quantities from the state.
    /// @param[in,out] initState A modified initial state.
    void applyParametersToInitState(const PSimParameterValueSet& paramValues,
         const Model& model, SimTK::State& initState) const;

    /// Modify the state's cache according to the given parameters. This can
    /// only be called if the model will no longer be modified, and is likely
    /// called throughout the simulation to modify quantities such as control
    /// values.
    /// @param[in] paramValues This comes from the optimizer.
    /// @param[in] model Used to access quantities from the state.
    /// @param[in,out] s The state's cache is modified.
    void applyParametersToStateCache(const PSimParameterValueSet& paramValues,
         const Model& model, const SimTK::State& s) const;

    /// Initial guess to send to the optimizer, with lower and upper limits.
    /// Uses the values from the <tt>initial_guess</tt> property. For
    /// parameters left out of <tt>initial_guess</tt>, we use the parameter's
    /// default value. The length of the returned Vectors depends
    /// on the number of OpenSim::PSimParameter's set to optimize.
    void initialOptimizerParameterValuesAndLimits(
            SimTK::Vector& initOptParams,
            SimTK::Vector& lowerLimits,
            SimTK::Vector& upperLimits) const;

    /// Used to organize the parameters from the optimizer.
    /// @param[in] optParamValues Optimizer parameter values.
    /// @returns Unnormalized parameter values.
    PSimParameterValueSet createParameterValueSet(
            const SimTK::Vector& optParamValues) const;
    /// @}

    /// @name Sending parameters to and from the Optimizer.
    /// @{

    /// Copies the Objectives into the provided Model. The model then owns
    /// these goals.
    /// @returns A vector of the Objectives thatare in the model.
    std::vector<const PSimGoal*> addGoalsToModel(Model& model) const;

    /// Builds up the objective function using the <tt>goals</tt>.
    /// @param[in] pvalset The optimizer parameters.
    /// @param[in] finalState The state at the end of the simulation.
    static SimTK::Real evaluateGoals(
            const std::vector<const PSimGoal*>& goals,
            const PSimParameterValueSet& pvalset,
            const StateTrajectory& states);
    /// @}

private:

    void checkForUnusedInitialGuesses() const;

    void constructProperties();

};

} // namespace OpenSim

#endif // OPENSIM_PSIM_TOOL_H_
