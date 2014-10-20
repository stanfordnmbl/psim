#ifndef PSIM_TOOL_H_
#define PSIM_TOOL_H_

#include <OpenSim/Common/Object.h>

#include "Parameter.h"
#include "ParameterValue.h"

namespace PSim {

class Tool : public OpenSim::Object
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSim::Tool, Object);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(model_file, std::string,
            "Path to the base model file (.osim) to optimize.");
    OpenSim_DECLARE_PROPERTY(initial_time, double,
            "The time at which all the simulations start (seconds).");
    OpenSim_DECLARE_PROPERTY(final_time, double,
            "The time at which the simulations end (seconds).");
    OpenSim_DECLARE_LIST_PROPERTY(parameters, Parameter,
            "Optimization parameters.");
    OpenSim_DECLARE_PROPERTY(initial_guess, ParameterValueSet,
            "Values of parameters used in initial guess in the optimization "
            "(unnormalized). For parameters that are left out in this object, "
            "the parameter's default value is used. If you supply values "
            "for parameters that are not set to be optimized, those values "
            "are ignored.");
//    OpenSim_DECLARE_PROPERTY(objectives, PSim::Objective,
//            "Terms of the optimization objective function.");
    /// @}

    Tool();

    // TODO print out the new model? that's what is optimized...
    /// Perform the predictive simulation optimization.
    /// @returns solution to the optimization.
    ParameterValueSet run() const;

    /// @name Sending parameters to and from the Optimizer.
    /// These methods implicitly define how optimizer parameters map to and
    /// from tool parameters.
    /// We make a distinction between "tool" and "optimizer" parameters:
    /// tool parameters are those from the <tt>parameters</tt> property; these
    /// can be set to not be optimized and are not normalized.
    /// Optimizer parameters are normalized and don't contain tool
    /// parameters that are not being optimized.
    // These methods handle the mapping between tool and optimizer parameters.
    /// @{

    /// The sum of the number of scalar parameters across the PSim::Parameter's
    /// that are set to be optimized.
    unsigned int numOptimizerParameters() const;

    /// Modify the model according to the given parameters. For parameters
    /// that are not being optimized, use the default value of the parameter.
    /// @param[in] paramValues this is what the optimizer gives to the
    ///     OptimizerSystem.
    /// @param[in,out] model The model to modify with these parameters.
    void applyParameters(const ParameterValueSet& paramValues,
            Model& model, SimTK::State& initState) const;

    /// Initial guess to send to the optimizer, with lower and upper limits.
    /// Uses the values from the <tt>initial_guess</tt> property. For
    /// parameters left out of <tt>initial_guess</tt>, we use the parameter's
    /// default value. The length of the returned Vectors depends
    /// on the number of PSim::Parameter's set to optimize.
    void initialOptimizerParameterValuesAndLimits(
            SimTK::Vector& initOptParams,
            SimTK::Vector& lowerLimits,
            SimTK::Vector& upperLimits) const;

    /** Used to organize the parameters from the optimizer.
     * @param[in] optParamValues Optimizer parameter values.
     * @returns Unnormalized parameter values.
     */
    ParameterValueSet createParameterValueSet(
            const SimTK::Vector& optParamValues) const;

    /// @}

    /// Builds up the objective function using the <tt>objectives</tt>.
    /// @param[in] pvalset The optimizer parameters.
    /// @param[in] model The pvalset has already been applied to this model.
    /// @param[in] finalState The state at the end of the simulation.
    SimTK::Real evaluateObjectives(const ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const;

private:

    void checkForUnusedInitialGuesses() const;

    void constructProperties();

};

} // namespace PSim

#endif // PSIM_TOOL_H_
