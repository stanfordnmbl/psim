#include "PSimTool.h"
#include "PSimDynamicOptimizationSolver.h"

namespace OpenSim {

PSimTool::PSimTool()
{
    constructProperties();
    checkForUnusedInitialGuesses();
}

void PSimTool::constructProperties() {

    constructProperty_base_model(Model());
    constructProperty_solver(PSimDynamicOptimizationSolver());

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
    // TODO print the updated model?
    // TODO print the objective values (at each iteration?)
    return get_solver().solve(*this);
    /*
    // Get initial guess, and parameter limits.
    // ========================================
    SimTK::Vector results;
    SimTK::Vector lowerLimits;
    SimTK::Vector upperLimits;
    initialOptimizerParameterValuesAndLimits(results, lowerLimits, upperLimits);

    // Setup the solver.
    // =================
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
    */
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

std::vector<const PSimGoal *> PSimTool::addGoalsToModel(Model& model) const
{
    std::vector<const PSimGoal *> goals;
    for (unsigned int ig = 0; ig < getProperty_goals().size(); ++ig) {
        const PSimGoal &goal = get_goals(ig);
        if (goal.get_enabled()) {
            PSimGoal * clone = goal.clone();
            // The model now owns this clone.
            model.addModelComponent(clone);
            // Append to the return vector.
            goals.push_back(clone);
        }
    }
    return goals;
}

SimTK::Real PSimTool::evaluateGoals(
        const std::vector<const PSimGoal *> &objectives,
        const PSimParameterValueSet &pvalset,
        const Model &model,
        const StateTrajectory &states)
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

