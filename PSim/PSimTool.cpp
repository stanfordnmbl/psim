#include "PSimTool.h"
#include "OptimizerSystem.h"

namespace OpenSim {

PSimTool::PSimTool()
{
    constructProperties();
    checkForUnusedInitialGuesses();
}

void PSimTool::constructProperties() {
    constructProperty_base_model(Model());

    constructProperty_initial_time(0);
    constructProperty_final_time(1);

    constructProperty_parameters();

    constructProperty_objectives();

    PSimParameterValueSet initial_guess;
    constructProperty_initial_guess(initial_guess);

    constructProperty_visualize(false);

    constructProperty_optimization_convergence_tolerance(1e-4);
}

unsigned int PSimTool::numOptimizerParameters() const
{
    unsigned int sum = 0;
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter & param = get_parameters(itp);
        if (param.get_optimize()) {
            sum += get_parameters(itp).numScalarParameters();
        }
    }
    return sum;
}

void PSimTool::applyParameters(const PSimParameterValueSet & paramValues,
        Model &model, SimTK::State& initState) const
{
    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter & param = get_parameters(itp);
        // If the parameter is to be optimized.
        if (param.get_optimize()) {
            const double value = paramValues.get(param.getName()).get_value();
            param.apply(value, model, initState);
        }
        else {
            // Use the default value of the parameter.
            // TODO don't need to do this after the first call.
            param.apply(param.get_default_value(), model, initState);
        }
    }

    /**
    for (unsigned int ip = 0; ip < paramValues.getSize(); ++ip) {
        const PSimParameterValue& pval = paramValues[ip];
        const PSimParameter& param = get_parameters().get(pval.getName());
        param.apply(pval.get_value(), model, initState);
        // TODO this neglects to set default values.
    }
    */
    /**
    // Indexes through optimizer parameters.
    unsigned int iop = 0;

    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {
        const PSimParameter& param = get_parameters(itp);
        // If the parameter is to be optimized.
        if (param.get_optimize()) {
            param.apply(param.unnormalized(optParamValues[iop]),
                    model, initState);
            iop++;
        }
        else {
            // Use the default value of the parameter.
            // TODO don't need to do this after the first call.
            param.apply(param.get_default_value(), model, initState);
        }
    }
    */
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

PSimParameterValueSet PSimTool::run() const
{
    // Get initial guess, and parameter limits.
    // ========================================
    SimTK::Vector results;
    SimTK::Vector lowerLimits;
    SimTK::Vector upperLimits;
    initialOptimizerParameterValuesAndLimits(results, lowerLimits, upperLimits);

    // Create an OptimizerSystem.
    // ==========================
    OptimizerSystem optsys(*this);
    optsys.setParameterLimits(lowerLimits, upperLimits);

    // Create an Optimizer.
    // ====================
    // TODO plug in a solver.
    SimTK::Optimizer opt(optsys);
    opt.setConvergenceTolerance(get_optimization_convergence_tolerance());
    // TODO don't bother when using CMAES.
    opt.useNumericalGradient(true);

    // Optimize!
    // =========
    opt.optimize(results);

    // Return the solution to the optimization.
    // ========================================
    return createParameterValueSet(results);
}

PSimParameterValueSet PSimTool::createParameterValueSet(
        const SimTK::Vector &optParamValues) const
{
    PSimParameterValueSet pvalset;

    // Indexes through optimizer parameters.
    unsigned int iop = 0;

    // Indexes through tool parameters.
    for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp) {

        const PSimParameter & param = get_parameters(itp);

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
        const PSimParameterValue & initialGuess = get_initial_guess().get(ig);
        for (unsigned int itp = 0; itp < getProperty_parameters().size(); ++itp)
        {
            const PSimParameter & param = get_parameters(itp);
            if (initialGuess.getName() == param.getName()) {
                continue;
            }
        }
        // TODO make this into a warning. TODO test this.
        throw OpenSim::Exception("Initial guess '" + initialGuess.getName()
                + "' does not match any parameter.");
    }
}

std::vector<const PSimGoal *> PSimTool::addObjectivesToModel(Model& model) const
{
    std::vector<const PSimGoal *> objectives;
    for (unsigned int io = 0; io < getProperty_objectives().size(); ++io) {
        const PSimGoal &obj = get_objectives(io);
        if (obj.get_enabled()) {
            PSimGoal * clone = obj.clone();
            // The model now owns this clone.
            model.addModelComponent(clone);
            // Append to the return vector.
            objectives.push_back(clone);
        }
    }
    return objectives;
}

SimTK::Real PSimTool::evaluateObjectives(
        const std::vector<const PSimGoal *>& objectives,
        const PSimParameterValueSet & pvalset,
        const Model& model,
        const StateTrajectory& states)
{
    SimTK::Real f = 0;
    for (auto obj : objectives) {
        if (obj->get_enabled()) {
            f += obj->get_weight() * obj->evaluate(pvalset, model, states);
        }
    }
    return f;
}

} // namespace OpenSim

