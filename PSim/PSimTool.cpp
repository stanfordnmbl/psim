/* -------------------------------------------------------------------------- *
 *                    OpenSim:  PSimTool.cpp                                  *
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

#include "PSimTool.h"

#include "PSimDynamicOptimizationSolver.h"
#include "StatesCollector.h"
#include "StateTrajectory.h"

#include <OpenSim/Simulation/Manager/Manager.h>

namespace OpenSim {

PSimTool::PSimTool()
{
    constructProperties();
    checkForUnusedInitialGuesses();
}

PSimTool::PSimTool(const std::string& fileName)
    : Object(fileName, false)
{
    constructProperties();
    checkForUnusedInitialGuesses();
    updateFromXMLDocument();
    if (!get_base_model_file().empty()) {
        m_model.reset(new Model(getAbsolutePathname(get_base_model_file())));
    }
}

void PSimTool::constructProperties() {

    constructProperty_base_model_file("");
    constructProperty_solver(PSimDynamicOptimizationSolver());

    constructProperty_results_dir("");
    constructProperty_initial_time(0);
    constructProperty_final_time(1);
    constructProperty_visualize(false);

    constructProperty_parameters();
    constructProperty_goals();

    PSimParameterValueSet initial_guess;
    constructProperty_initial_guess(initial_guess);

}

unsigned int PSimTool::numOptimizerParameters() const
{
    unsigned int sum = 0;
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter & param = get_parameters(itp);
        if (param.get_apply() && param.get_optimize()) {
            sum += get_parameters(itp).numScalarParameters();
        }
    }
    return sum;
}

void PSimTool::applyParametersToModel(const PSimParameterValueSet& paramValues,
        Model& model) const
{
    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter & param = get_parameters(itp);
        // If we should pay attention to this parameter.
        if (param.get_apply()) {
            // If the parameter is to be optimized.
            if (param.get_optimize()) {
                const double value = paramValues.get(param.getName()).get_value();
                param.applyToModel(value, model);
            }
            else {
                // Use the default value of the parameter.
                // TODO if we decide not to create a new copy of the model, 
                // we don't need to do this after the first call.
                param.applyToModel(param.get_default_value(), model);
            }
        }
    }
}

void PSimTool::applyParametersToInitState(
        const PSimParameterValueSet& paramValues,
        const Model& model, SimTK::State& initState) const
{
    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter & param = get_parameters(itp);
        // If we should pay attention to this parameter.
        if (param.get_apply()) {
            // If the parameter is to be optimized.
            if (param.get_optimize()) {
                const double value = paramValues.get(param.getName()).get_value();
                param.applyToInitialState(value, model, initState);
            }
            else {
                // Use the default value of the parameter.
                // TODO if we decide not to create a new copy of the model, 
                // we don't need to do this after the first call.
                param.applyToInitialState(param.get_default_value(), model,
                        initState);
            }
        }
    }
}

void PSimTool::applyParametersToStateCache(
        const PSimParameterValueSet& paramValues,
        const Model& model, const SimTK::State& s) const
{
    // TODO this could be costly?
    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter & param = get_parameters(itp);
        // If we should pay attention to this parameter.
        if (param.get_apply()) {
            // If the parameter is to be optimized.
            if (param.get_optimize()) {
                const double value = paramValues.get(param.getName()).get_value();
                param.applyToStateCache(value, model, s);
            }
            else {
                // Use the default value of the parameter.
                // TODO if we decide not to create a new copy of the model, 
                // we don't need to do this after the first call.
                param.applyToStateCache(param.get_default_value(), model, s);
            }
        }
    }
}

void PSimTool::initialOptimizerParameterValuesAndLimits(
        SimTK::Vector& initOptParams,
        SimTK::Vector& lowerLimits,
        SimTK::Vector& upperLimits) const
{
    initOptParams.resize(numOptimizerParameters());
    lowerLimits.resize(numOptimizerParameters());
    upperLimits.resize(numOptimizerParameters());

    // Indexes through optimizer parameters.
    unsigned int iop = 0;

    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter & param = get_parameters(itp);
        // If the parameter is to be optimized.
        if (param.get_optimize()) {
            // Ignore values in initial_guess for parameters that are not
            // set to be optimized.

            double unnormalized;

            // The user supplied an initial guess.
            if (get_initial_guess().contains(param.getName())) {
                const PSimParameterValue & pval =
                        get_initial_guess().get(param.getName());
                unnormalized = pval.get_value();
            }
            else {
                unnormalized = param.get_default_value();
            }

            // Normalize the parameter, place it in the initial opt. params.
            initOptParams[iop] = param.normalized(unnormalized);

            // Normalize the lower and upper limits.
            lowerLimits[iop] = param.normalized(param.get_lower_limit());
            upperLimits[iop] = param.normalized(param.get_upper_limit());

            iop++;
        }
    }
}

PSimParameterValueSet PSimTool::solve() const
{
    // TODO print the updated model?
    // TODO print the objective values (at each iteration?)
    return get_solver().solve(*this);
}

double PSimTool::simulate(const PSimParameterValueSet& pvalset) const
{
    // Initialize model and apply model parameters.
    // ============================================
    // Create a copy of the base model.
    Model model(getBaseModel());
    if (get_visualize()) model.setUseVisualizer(true);
    applyParametersToModel(pvalset, model);

    // Add PSimGoal's to Model as ModelComponents. Do this after applying the
    // parameters, as the goals may depend on the effect of parameters.
    const auto goals = addGoalsToModel(model);

    // Mechanism to record the trajectory of successful states.
    // --------------------------------------------------------
    StatesCollector* statesCollector = new StatesCollector();
    statesCollector->setName("statesCollector");
    model.addAnalysis(statesCollector);

    // Generate an initial state. Must also be done after applying to model.
    SimTK::State& state = model.initSystem();

    // Now that the model is finalized for this sim., modify initial state.
    // ====================================================================
    applyParametersToInitState(pvalset, model, state);

    // Simulate.
    // =========
    SimTK::RungeKuttaMersonIntegrator integrator(model.getSystem());
    OpenSim::Manager manager(model, integrator);
    manager.setWriteToStorage(false);
    manager.setInitialTime(get_initial_time());
    manager.setFinalTime(get_final_time());
    manager.integrate(state);

    // Compute objective function value with the goals.
    // ================================================
    const StateTrajectory& states = statesCollector->getStateTrajectory();
    return evaluateGoals(goals, pvalset, states);
}

PSimParameterValueSet PSimTool::createParameterValueSet(
        const SimTK::Vector &optParamValues) const
{
    // TODO will have to change if the solver adds its own parameters.
    PSimParameterValueSet pvalset;

    // Indexes through optimizer parameters.
    unsigned int iop = 0;

    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {

        const PSimParameter& param = get_parameters(itp);

        // If the parameter was optimized.
        if (param.get_optimize()) {
            PSimParameterValue * pval = new PSimParameterValue();
            pval->setName(param.getName());
            // TODO get subvector.
            pval->set_value(param.unnormalized(optParamValues(iop)));
            pvalset.adoptAndAppend(pval);
            // TODO increase index by #scalar parameters.
            iop++;
        }
    }
    return pvalset;
}

void PSimTool::checkForUnusedInitialGuesses() const {
    for (unsigned int ig = 0; ig < get_initial_guess().getSize(); ++ig) {
        const PSimParameterValue& initialGuess = get_initial_guess().get(ig);
        for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp)
        {
            const PSimParameter& param = get_parameters(itp);
            if (initialGuess.getName() == param.getName()) {
                continue;
            }
        }
        // TODO make this into a warning. TODO test this.
        throw OpenSim::Exception("Initial guess '" + initialGuess.getName()
                + "' does not match any parameter.");
    }
}

std::vector<const PSimGoal*> PSimTool::addGoalsToModel(Model& model) const
{
    std::vector<const PSimGoal*> goals;
    for (unsigned int ig = 0; ig < getProperty_goals().size(); ++ig) {
        const PSimGoal& goal = get_goals(ig);
        if (goal.get_enabled()) {
            PSimGoal* clone = goal.clone();
            // The model now owns this clone.
            model.addModelComponent(clone);
            // Append to the return vector.
            goals.push_back(clone);
        }
    }
    return goals;
}

SimTK::Real PSimTool::evaluateGoals(
        const std::vector<const PSimGoal*>& objectives,
        const PSimParameterValueSet& pvalset,
        const StateTrajectory& states)
{
    SimTK::Real f = 0;
    for (auto obj : objectives) {
        if (obj->get_enabled()) {
        // TODO move enabled check to PSimGoal.
            f += obj->get_weight() * obj->evaluate(pvalset, states);
        }
    }
    return f;
}

} // namespace OpenSim

